#ifndef REQUEST_H
#define REQUEST_H

#include <vector>
#include "http_header.h"

struct request
{
  std::string method;
  std::string uri;
  std::string http_version;
  std::vector<http_header> headers;
  std::string content;
};

#endif // REQUEST_H
