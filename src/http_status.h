#ifndef HTTPSTATUS_H
#define HTTPSTATUS_H

namespace http_status {

const char crlf[] = "\r\n";

enum code
{
  // 1xx - Informational
  continue_           = 100,
  switching_protocols = 101,
  processing          = 102,
  early_hints         = 103,

  // 2xx - Successful
  ok                            = 200,
  created                       = 201,
  accepted                      = 202,
  non_authoritative_information = 203,
  no_content                    = 204,
  reset_content                 = 205,
  partial_content               = 206,
  multi_status                  = 207,
  already_reported              = 208,
  im_used                       = 226,

  // 3xx - Redirection
  multiple_choices   = 300,
  moved_permanently  = 301,
  found              = 302,
  see_other          = 303,
  not_modified       = 304,
  use_proxy          = 305,
  temporary_redirect = 307,
  permanent_redirect = 308,

  // 4xx - Client Error
  bad_request                   = 400,
  unauthorized                  = 401,
  payment_required              = 402,
  forbidden                     = 403,
  not_found                     = 404,
  method_not_allowed            = 405,
  not_acceptable                = 406,
  proxy_authentication_required = 407,
  request_timeout               = 408,
  conflict                      = 409,
  gone                          = 410,
  length_required               = 411,
  precondition_failed           = 412,
  content_too_large             = 413,
  payload_too_large             = 413,
  uri_too_long                  = 414,
  unsupported_media_type        = 415,
  range_not_satisfiable         = 416,
  expectation_failed            = 417,
  im_a_teapot                   = 418,
  misdirected_request           = 421,
  unprocessable_content         = 422,
  unprocessable_entity          = 422,
  locked                        = 423,
  failed_dependency             = 424,
  too_early                     = 425,
  upgrade_required              = 426,
  precondition_required         = 428,
  too_many_requests             = 429,
  request_headerfields_toolarge = 431,
  unavailable_for_legal_reasons = 451,

  // 5xx - Server Error
  internal_server_error           = 500,
  not_implemented                 = 501,
  bad_gateway                     = 502,
  service_unavailable             = 503,
  gateway_timeout                 = 504,
  http_version_not_supported      = 505,
  variant_also_negotiates         = 506,
  insufficient_storage            = 507,
  loop_detected                   = 508,
  not_extended                    = 510,
  network_authentication_required = 511,
  xxx_max = 1023
};

char isInformational(int code);
char isSuccessful(int code);
char isRedirection(int code);
char isClientError(int code);
char isServerError(int code);
char isError(int code);

const char* reason_phrase(int code);

} // namespace http_status


#endif // HTTPSTATUS_H
