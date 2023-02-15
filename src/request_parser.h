#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include "request.h"
#include <string>

class request_parser
{
public:
  request_parser();
  void init(std::size_t buff_reserve_size);
  bool parse(request& req, char* buff, std::size_t length);
  
private:
  inline void parse_semicolon(request &req, char in);
  inline void parse_spaceortab(request &req, char in);
  inline void parse_linefeed(request &req, char in);
  
private:
  std::string strbuff; 

  enum state
  {
    method,
    uri,
    http,
    header_name,
    header_value,
    content
  } state_;
};

#endif // REQUEST_PARSER_H
