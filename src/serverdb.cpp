#include "serverdb.h"
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <vector>
#include <algorithm>

#include "logger.h"
using namespace logs;

using namespace boost::asio;

void serverdb::init_db(char *host_ip, std::size_t host_port, char *db_name, string &user_name, string &user_password)
{
  request_handler_.init_db(host_ip, host_port, db_name, user_name, user_password);
}

void serverdb::run()
{
  LOG(logTRACE) << "serverdb::run() START";

  last_tick_count = microsec_clock::local_time();

  boost::thread_group threads;

  threads.create_thread( boost::bind(&serverdb::accept_thread, this));

  for (std::size_t i = 0; i < thread_pool_size_; ++i)
    threads.create_thread( boost::bind(&serverdb::handle_clients_thread, this));
    
  threads.join_all();
}

void serverdb::accept_thread()
{
  LOG(logTRACE) << "accept_thread() START";

  ip::tcp::acceptor acceptor(context_, ip::tcp::endpoint(ip::tcp::v4(), port_));

  while ( true)
  {
    boost::this_thread::sleep( millisec( (long long)idle_time));
    
    client_ptr newsession(new clientsession(context_, request_handler_));
    
    boost::system::error_code ec;
    
    acceptor.accept(newsession->sock(), ec);

    if (ec) {
      LOG(logERROR) << "accept_thread() accept " << ec;
      continue;
    }

    newsession->init();

    boost::recursive_mutex::scoped_lock lock(clients_mutex);
    clients_.push_back(newsession);
  }
}

void serverdb::handle_clients_thread()
{
  LOG(logTRACE) << "handle_clients_thread() START";

  while ( true)
  {
    boost::this_thread::sleep( millisec( (long long)idle_time));
    client_ptr found_for_reading;
    {
      boost::recursive_mutex::scoped_lock lock(clients_mutex);
      for (clients::iterator b = clients_.begin(), e = clients_.end(); b != e; ++b)
        if ((*b)->set_reading())
        {
          found_for_reading = (*b);
          break;
        }
    }

    if (found_for_reading != nullptr)
    {
      found_for_reading->do_read();
      found_for_reading->unset_reading();
    }

    ptime now = microsec_clock::local_time();
    long long interval = (now - last_tick_count).total_milliseconds();

    if (interval > erase_task_interval)
    {
      boost::recursive_mutex::scoped_lock lock(clients_mutex);
      for (clients::iterator b = clients_.begin(); b != clients_.end();)
      {
        if ((*b)->is_done())
          b = clients_.erase(b);
        else
          b++;
      }
      last_tick_count = microsec_clock::local_time();
    }

  } // while
}

