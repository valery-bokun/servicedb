#include "clientsession.h"
#include "logger.h"

using namespace logs;
using namespace boost::posix_time;

clientsession::clientsession(io_context &context, request_handler &handler)
  : sock_(context), is_reading_(false), request_handler_(handler)
{
  LOG(logTRACE) << "session NEW " << this;
  request_parser_.init(buff_size * 4);
}

clientsession::~clientsession()
{
  LOG(logTRACE) << "session DEL " << this;
}

void clientsession::init()
{
  last_read_time = microsec_clock::local_time();

  LOG(logINFO) << "session INIT " << this << " "
             << sock_.remote_endpoint().address().to_string();
}

bool clientsession::set_reading()
{
  boost::mutex::scoped_lock lock(is_reading_mutex);
  if ( is_reading_) return false;
  if ( !sock_.is_open()) return false;
  if ( !sock_.available()) return false;
  is_reading_ = true;
  return true;
}

bool clientsession::is_done()
{
  if (is_reading_) return false;
  
  ptime now = microsec_clock::local_time();
  long long ms = (now - last_read_time).total_milliseconds();
  return ms > reading_timeout;
}

void clientsession::unset_reading()
{
  boost::mutex::scoped_lock lock(is_reading_mutex);
  is_reading_ = false;
}

void clientsession::do_read()
{
  LOG(logINFO) << "session do_read() " << this << " "
             << sock_.remote_endpoint().address().to_string();

  boost::system::error_code ec;
  std::size_t transferred = sock_.read_some( buffer(buff_, buff_size), ec);

  if (ec) {
    LOG(logERROR) << "session::do_read() sock read" << ec;
    return;
  }

  if ( !transferred)
    LOG(logWARN) << "session::do_read() transferred=0 " << this << " "
               << sock_.remote_endpoint().address().to_string();
  
  bool parse_done = request_parser_.parse( request_, buff_, transferred);

  last_read_time = microsec_clock::local_time();
  
  if ( parse_done) {
      LOG(logINFO) << "session parse_done " << this << " "
                 << sock_.remote_endpoint().address().to_string();

      request_handler_.handle_request( request_, reply_);
      boost::asio::streambuf buf;
      reply_.get_stream(buf);
      write( sock_, buf, transfer_all(), ec);

      if (ec) 
        LOG(logERROR) << "session::do_read() sock write " << ec;
      
      sock_.close();
    }
}
