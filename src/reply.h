#ifndef REPLY_H
#define REPLY_H

#include <vector>
#include <string>
#include <streambuf>
#include "http_header.h"

struct reply
{
  int16_t status_code;
  std::vector<http_header> headers;
  std::string content;

  void get_stream(std::streambuf &buff);
  static reply make_reply(int16_t status_code, const std::string &content);
  static reply reply_status_only(int16_t status_cod);
};

#endif // REPLY_H
