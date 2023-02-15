#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "request.h"
#include "request_parser.h"
#include "request_handler.h"
#include "reply.h"

using namespace boost::asio;

struct clientsession : boost::enable_shared_from_this<clientsession>
{
  explicit clientsession(io_context &context, request_handler &handler);
  ~clientsession();

  void init();

  ip::tcp::socket &sock() { return sock_; }

  bool set_reading();
  bool is_done();
  void unset_reading();
  void do_read();

private:
  ip::tcp::socket sock_;
  enum { 
    buff_size = 256, 
    reading_timeout = 5000 
    };
  char buff_[buff_size];
  bool is_reading_;
  boost::posix_time::ptime last_read_time;
  boost::mutex is_reading_mutex;
  request request_;
  reply reply_;
  request_parser request_parser_;
  request_handler &request_handler_;
};

typedef boost::shared_ptr<clientsession> client_ptr;

#endif // CLIENTSESSION_H
