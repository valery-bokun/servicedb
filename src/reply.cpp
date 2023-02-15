#include "reply.h"
#include "http_status.h"

#include <ostream>

std::ostream &operator<<( std::ostream &stream, http_header &header ) {
    stream << header.name << ": " << header.value << http_status::crlf;
    return stream;
}

void reply::get_stream(std::streambuf &buff)
{
    std::ostream stream(&buff);
    
    stream << "HTTP/1.0 " << std::to_string(status_code) << " ";
    stream << http_status::reason_phrase(status_code) << http_status::crlf;

    for (http_header v : headers)
      stream << v;

    stream << http_status::crlf << content;
}

reply reply::make_reply(int16_t status_code, const std::string &content)
{
  reply rep;
  rep.status_code = status_code;
  rep.content = content;
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/plain";
  return rep;
}

reply reply::reply_status_only(int16_t status_code)
{
  reply rep;
  rep.status_code = status_code;
  rep.content = std::to_string(status_code);
  rep.content.append(" ");
  rep.content.append(http_status::reason_phrase(status_code));
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/plain";
  return rep;
}

