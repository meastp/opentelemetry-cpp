// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/export.h"
#include "opentelemetry/ext/http/client/http_client.h"
#include "opentelemetry/version.h"

#include <curl/curl.h>
#include <future>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#ifdef _WIN32
#  include <io.h>
#else
#  include <unistd.h>
#endif

OPENTELEMETRY_BEGIN_NAMESPACE
namespace ext
{
namespace http
{
namespace client
{
namespace curl
{
const std::chrono::milliseconds default_http_conn_timeout(5000);  // ms
const std::string http_status_regexp = "HTTP\\/\\d\\.\\d (\\d+)\\ .*";
const std::string http_header_regexp = "(.*)\\: (.*)\\n*";

enum class RequestMode
{
  Sync,
  Async
};

class OTEL_API HttpOperation
{
public:
  void DispatchEvent(opentelemetry::ext::http::client::SessionState type, std::string reason = "");

  std::atomic<bool> is_aborted_;   // Set to 'true' when async callback is aborted
  std::atomic<bool> is_finished_;  // Set to 'true' when async callback is finished.

  /**
   * Create local CURL instance for url and body
   * @param method // HTTP Method
   * @param url    // HTTP URL
   * @param callback
   * @param request_mode // sync or async
   * @param request  Request Headers
   * @param body  Reques Body
   * @param raw_response whether to parse the response
   * @param httpConnTimeout   HTTP connection timeout in seconds
   */
  HttpOperation(opentelemetry::ext::http::client::Method method,
                std::string url,
                opentelemetry::ext::http::client::EventHandler *callback,
                RequestMode request_mode = RequestMode::Async,
                // Default empty headers and empty request body
                const opentelemetry::ext::http::client::Headers &request_headers =
                    opentelemetry::ext::http::client::Headers(),
                const opentelemetry::ext::http::client::Body &request_body =
                    opentelemetry::ext::http::client::Body(),
                // Default connectivity and response size options
                bool is_raw_response                        = false,
                std::chrono::milliseconds http_conn_timeout = default_http_conn_timeout);

  /**
   * Destroy CURL instance
   */
  virtual ~HttpOperation();

  /**
   * Finish CURL instance
   */
  virtual void Finish();

  /**
   * Send request synchronously
   */
  long Send();

  std::future<long> &SendAsync(std::function<void(HttpOperation &)> callback = nullptr);

  void SendSync();

  /**
   * Get HTTP response code. This function returns CURL error code if HTTP response code is invalid.
   */
  uint16_t GetResponseCode();

  /**
   * Get last session state.
   */
  opentelemetry::ext::http::client::SessionState GetSessionState();

  /**
   * Get whether or not response was programmatically aborted
   */
  bool WasAborted();

  /**
   * Return a copy of resposne headers
   *
   * @return
   */
  Headers GetResponseHeaders();

  /**
   * Return a copy of response body
   *
   * @return
   */
  std::vector<uint8_t> GetResponseBody();

  /**
   * Return a raw copy of response headers+body
   *
   * @return
   */
  std::vector<uint8_t> GetRawResponse();

  /**
   * Release memory allocated for response
   */
  void ReleaseResponse();

  /**
   * Abort request in connecting or reading state.
   */
  void Abort();

  CURL *GetHandle();

protected:
  const bool is_raw_response_;  // Do not split response headers from response body
  const std::chrono::milliseconds http_conn_timeout_;  // Timeout for connect.  Default: 5000ms
  RequestMode request_mode_;

  CURL *curl_;    // Local curl instance
  CURLcode res_;  // Curl result OR HTTP status code if successful

  opentelemetry::ext::http::client::EventHandler *callback_;

  // Request values
  opentelemetry::ext::http::client::Method method_;
  std::string url_;
  const Headers &request_headers_;
  const opentelemetry::ext::http::client::Body &request_body_;
  struct curl_slist *headers_chunk_ = nullptr;
  opentelemetry::ext::http::client::SessionState session_state_;

  // Processed response headers and body
  std::vector<uint8_t> resp_headers_;
  std::vector<uint8_t> resp_body_;
  std::vector<uint8_t> raw_response_;

  // Socket parameters
  curl_socket_t sockfd_;

  curl_off_t nread_;
  size_t sendlen_ = 0;  // # bytes sent by client
  size_t acklen_  = 0;  // # bytes ack by server

  std::future<long> result_;

  /**
   * Helper routine to wait for data on socket
   *
   * @param sockfd
   * @param for_recv
   * @param timeout_ms
   * @return
   */
  static int WaitOnSocket(curl_socket_t sockfd, int for_recv, long timeout_ms);

  /**
   * Old-school memory allocator
   *
   * @param contents
   * @param size
   * @param nmemb
   * @param userp
   * @return
   */
  static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

  /**
   * C++ STL std::vector allocator
   *
   * @param ptr
   * @param size
   * @param nmemb
   * @param data
   * @return
   */
  static size_t WriteVectorCallback(void *ptr,
                                    size_t size,
                                    size_t nmemb,
                                    std::vector<uint8_t> *data);
};
}  // namespace curl
}  // namespace client
}  // namespace http
}  // namespace ext
OPENTELEMETRY_END_NAMESPACE
