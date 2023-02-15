#include "request_parser.h"
#include <boost/algorithm/string.hpp>

request_parser::request_parser() : state_(method) {}

bool request_parser::parse(request &req, char *buff, std::size_t length)
{
    if (!length) return state_ == content;

    for(char *in = buff; in < buff + length; ++in) 
        if (*in == ':') 
          parse_semicolon(req, *in);
        else if (*in == ' ' || *in == '\t' ) 
          parse_spaceortab(req, *in);
        else if (*in == '\n') 
          parse_linefeed(req, *in);
        else strbuff.push_back(*in);     

    return state_ == content;
}

void request_parser::init(std::size_t buff_reserve_size)
{
    strbuff.reserve(buff_reserve_size);
}

void request_parser::parse_semicolon(request &req, char in)
{
    switch (state_)
    {
    case header_name:
        state_ = header_value;
        req.headers.push_back(http_header());
        req.headers.back().name = strbuff;
        strbuff.clear();
        break;                 
    default:
        strbuff.push_back(in); 
        break;
    }    
}

void request_parser::parse_spaceortab(request &req, char in)
{
    switch (state_)
    {
    case method:
        state_ = uri;
        req.method = strbuff;
        strbuff.clear();
        break;
    case uri:
        state_ = http;
        req.uri = strbuff;
        strbuff.clear();
        break;                 
    default:
        strbuff.push_back(in); 
        break;
    }
}

void request_parser::parse_linefeed(request &req, char in)
{
    switch (state_)
    {
    case http:
        state_ = header_name;
        req.http_version = strbuff;
        boost::trim_right(req.http_version);
        strbuff.clear();
        break;         
    case header_name:
        state_ = content;
        // TODO: make hadling request content
        strbuff.clear();
        break;
    case header_value:
        boost::trim(strbuff);
        state_ = header_name;
        req.headers.back().value = strbuff;
        strbuff.clear();
        break;                      
    default:
        strbuff.push_back(in); 
        break;
    }
}
