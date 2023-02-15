#ifndef SERVERDB_H
#define SERVERDB_H

#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include "clientsession.h"
#include "request_handler.h"

using namespace std;
using namespace boost::posix_time;

class serverdb : private boost::noncopyable
{
public:
  explicit serverdb(std::size_t port, std::size_t thread_pool_size)
    : port_(port), thread_pool_size_(thread_pool_size), request_handler_(context_) {};
  
  void init_db(char *host_ip, std::size_t host_port, char *db_name, string &user_name, string &user_password);

  void run();

private:
  void accept_thread();
  void handle_clients_thread();

  enum { 
    idle_time = 1,
    erase_task_interval = 5000
    };
  std::size_t port_;
  std::size_t thread_pool_size_;
  io_context context_;

  ptime last_tick_count;
  typedef std::vector<client_ptr> clients;
  clients clients_;
  boost::recursive_mutex clients_mutex;

  request_handler request_handler_;
};

#endif // SERVERDB_H
