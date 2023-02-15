#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "http_status.h"
#include "request_handler.h"
#include "request_parser.h"

BOOST_AUTO_TEST_SUITE(test_request_handler)
  io_context context;
  request_handler handler(context);
  request req;
  reply rep;

  BOOST_AUTO_TEST_CASE(method_get_root) {
    req.method = "GET";
    req.uri = "/";
    handler.handle_request(req, rep);
    BOOST_REQUIRE(rep.status_code==http_status::ok);
    BOOST_REQUIRE(rep.content=="Hello!\ntry type: http://127.0.0.1:3300/test");
  }

  BOOST_AUTO_TEST_CASE(method_get_test) {
    req.method = "GET";
    req.uri = "/test";
    handler.handle_request(req, rep);
    BOOST_REQUIRE(rep.status_code==http_status::ok);
    BOOST_REQUIRE(rep.content=="TEST OK");
  }

  BOOST_AUTO_TEST_CASE(method_post) {
    req.method = "POST";
    handler.handle_request(req, rep);
    BOOST_REQUIRE(rep.status_code!=http_status::not_implemented);
  }

  BOOST_AUTO_TEST_CASE(method_put) {
    req.method = "PUT";
    handler.handle_request(req, rep);
    BOOST_REQUIRE(rep.status_code!=http_status::not_implemented);
  }

  BOOST_AUTO_TEST_CASE(method_patch) {
    req.method = "PATCH";
    handler.handle_request(req, rep);
    BOOST_REQUIRE(rep.status_code!=http_status::not_implemented);
  }

  BOOST_AUTO_TEST_CASE(method_delete) {
    req.method = "DELETE";
    handler.handle_request(req, rep);
    BOOST_REQUIRE(rep.status_code!=http_status::not_implemented);
  }

  BOOST_AUTO_TEST_CASE(method_options) {
    req.method = "OPTIONS";
    handler.handle_request(req, rep);
    BOOST_REQUIRE(rep.status_code!=http_status::not_implemented);
  }
BOOST_AUTO_TEST_SUITE_END()
  
BOOST_AUTO_TEST_SUITE(test_request_parser)
  bool result; 

  BOOST_AUTO_TEST_CASE(parse_crlf) {
    request_parser parser;
    request req;
    char buff[] = "GET / HTTP/1.1\r\nHost: alizar.habrahabr.ru\r\n\r\n";
    result = parser.parse(req, buff, strlen(buff));
    BOOST_REQUIRE(result==true); 
    BOOST_REQUIRE(req.method=="GET");
    BOOST_REQUIRE(req.uri=="/");
    BOOST_REQUIRE(req.http_version=="HTTP/1.1");
    BOOST_REQUIRE(req.headers.size()==1);
    BOOST_REQUIRE(req.headers[0].name=="Host");
    BOOST_REQUIRE(req.headers[0].value=="alizar.habrahabr.ru");
  }

  BOOST_AUTO_TEST_CASE(parse_crlf_tab) {
    request_parser parser;
    request req;
    char buff[] = "GET\t/\tHTTP/1.1\r\nHost:\talizar.habrahabr.ru\r\n\r\n";
    result = parser.parse(req, buff, strlen(buff));
    BOOST_REQUIRE(result==true); 
    BOOST_REQUIRE(req.method=="GET");
    BOOST_REQUIRE(req.uri=="/");
    BOOST_REQUIRE(req.http_version=="HTTP/1.1");
    BOOST_REQUIRE(req.headers.size()==1);
    BOOST_REQUIRE(req.headers[0].name=="Host");
    BOOST_REQUIRE(req.headers[0].value=="alizar.habrahabr.ru");
  }

  BOOST_AUTO_TEST_CASE(parse_lf) {
    request_parser parser;
    request req;
    char buff[] = "GET / HTTP/1.1\nHost: alizar.habrahabr.ru\n\n";
    result = parser.parse(req, buff, strlen(buff));
    BOOST_REQUIRE(result==true);
    BOOST_REQUIRE(req.method=="GET");
    BOOST_REQUIRE(req.uri=="/");
    BOOST_REQUIRE(req.http_version=="HTTP/1.1");
    BOOST_REQUIRE(req.headers.size()==1);
    BOOST_REQUIRE(req.headers[0].name=="Host");
    BOOST_REQUIRE(req.headers[0].value=="alizar.habrahabr.ru");
  }

  BOOST_AUTO_TEST_CASE(parse_crlf_content) {
    request_parser parser;
    request req;
    char buff[] = "GET / HTTP/1.0\r\n\r\nTEST";
    result = parser.parse(req, buff, strlen(buff));
    BOOST_REQUIRE(result==true);
    BOOST_REQUIRE(req.method=="GET");
    BOOST_REQUIRE(req.uri=="/");
    BOOST_REQUIRE(req.http_version=="HTTP/1.0");
    BOOST_REQUIRE(req.headers.size()==0);
    BOOST_REQUIRE(req.content=="TEST");
  }

  BOOST_AUTO_TEST_CASE(parse_crlf_partial) {
    request_parser parser;
    request req;
    char buff[] = "GET / HTTP/1.1\r\nHost: alizar.habrahabr.ru\r\n\r\n";
    
    result = parser.parse(req, buff, 10);
    BOOST_REQUIRE(result==false); 

    result = parser.parse(req, buff + 10, 12);
    BOOST_REQUIRE(result==false); 

    result = parser.parse(req, buff + 10 + 12, strlen(buff) - 10 - 12);
    BOOST_REQUIRE(result==true);    

    BOOST_REQUIRE(req.method=="GET");
    BOOST_REQUIRE(req.uri=="/");
    BOOST_REQUIRE(req.http_version=="HTTP/1.1");
    BOOST_REQUIRE(req.headers.size()==1);
    BOOST_REQUIRE(req.headers[0].name=="Host");
    BOOST_REQUIRE(req.headers[0].value=="alizar.habrahabr.ru");
  }

BOOST_AUTO_TEST_SUITE_END()