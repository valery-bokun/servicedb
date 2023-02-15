#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include "request.h"
#include "reply.h"

using namespace std;
using namespace boost::asio;

class request_handler
{
public:
  request_handler(io_context &context);
  void init_db(char *host_ip, std::size_t host_port, char *db_name, string &user_name, string &user_password);
  void handle_request(const request &req, reply &rep);

private:
  void handle_get(const request &req, reply &rep);
  void handle_head(const request &, reply &rep);
  void handle_post(const request &, reply &rep);
  void handle_put(const request &, reply &rep);
  void handle_patch(const request &, reply &rep);
  void handle_delete(const request &, reply &rep);
  void handle_options(const request &, reply &rep);

  reply get_select_view(const string &sql);

  io_context &context_;
  string database_;
  std::size_t db_host_port;
  string db_host;
  string db_user_name;
  string db_password;

};

#endif // REQUEST_HANDLER_H
