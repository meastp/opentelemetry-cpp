// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/export.h"
#include "opentelemetry/ext/http/client/curl/http_operation_curl.h"
#include "opentelemetry/ext/http/client/http_client.h"
#include "opentelemetry/ext/http/common/url_parser.h"
#include "opentelemetry/version.h"

#include <map>
#include <string>
#include <vector>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace ext
{
namespace http
{
namespace client
{
namespace curl
{

const opentelemetry::ext::http::client::StatusCode Http_Ok = 200;

class OTEL_API Request : public opentelemetry::ext::http::client::Request
{
public:
  Request();

  void SetMethod(opentelemetry::ext::http::client::Method method) noexcept override;

  void SetBody(opentelemetry::ext::http::client::Body &body) noexcept override;

  void AddHeader(nostd::string_view name, nostd::string_view value) noexcept override;

  void ReplaceHeader(nostd::string_view name, nostd::string_view value) noexcept override;

  virtual void SetUri(nostd::string_view uri) noexcept override;

  void SetTimeoutMs(std::chrono::milliseconds timeout_ms) noexcept override;

public:
  opentelemetry::ext::http::client::Method method_;
  opentelemetry::ext::http::client::Body body_;
  opentelemetry::ext::http::client::Headers headers_;
  std::string uri_;
  std::chrono::milliseconds timeout_ms_{5000};  // ms
};

class OTEL_API Response : public opentelemetry::ext::http::client::Response
{
public:
  Response();

  virtual const opentelemetry::ext::http::client::Body &GetBody() const noexcept override;

  virtual bool ForEachHeader(
      nostd::function_ref<bool(nostd::string_view name, nostd::string_view value)> callable)
      const noexcept override;

  virtual bool ForEachHeader(
      const nostd::string_view &name,
      nostd::function_ref<bool(nostd::string_view name, nostd::string_view value)> callable)
      const noexcept override;

  virtual opentelemetry::ext::http::client::StatusCode GetStatusCode() const noexcept override;

public:
  Headers headers_;
  opentelemetry::ext::http::client::Body body_;
  opentelemetry::ext::http::client::StatusCode status_code_;
};

class HttpClient;

class OTEL_API Session : public opentelemetry::ext::http::client::Session
{
public:
  Session(HttpClient &http_client,
          std::string scheme      = "http",
          const std::string &host = "",
          uint16_t port           = 80);

  std::shared_ptr<opentelemetry::ext::http::client::Request> CreateRequest() noexcept override;

  virtual void SendRequest(
      opentelemetry::ext::http::client::EventHandler &callback) noexcept override;

  virtual bool CancelSession() noexcept override;

  virtual bool FinishSession() noexcept override;

  virtual bool IsSessionActive() noexcept override;

  void SetId(uint64_t session_id);

  /**
   * Returns the base URI.
   * @return the base URI as a string consisting of scheme, host and port.
   */
  const std::string &GetBaseUri() const;

private:
  std::shared_ptr<Request> http_request_;
  std::string host_;
  std::unique_ptr<HttpOperation> curl_operation_;
  uint64_t session_id_;
  HttpClient &http_client_;
  bool is_session_active_;
};

class OTEL_API HttpClientSync : public opentelemetry::ext::http::client::HttpClientSync
{
public:
  HttpClientSync();

  opentelemetry::ext::http::client::Result Get(
      const nostd::string_view &url,
      const opentelemetry::ext::http::client::Headers &headers) noexcept override;

  opentelemetry::ext::http::client::Result Post(
      const nostd::string_view &url,
      const Body &body,
      const opentelemetry::ext::http::client::Headers &headers) noexcept override;

  ~HttpClientSync();
};

class OTEL_API HttpClient : public opentelemetry::ext::http::client::HttpClient
{
public:
  // The call (curl_global_init) is not thread safe. Ensure this is called only once.
  HttpClient();

  std::shared_ptr<opentelemetry::ext::http::client::Session> CreateSession(
      nostd::string_view url) noexcept override;

  bool CancelAllSessions() noexcept override;

  bool FinishAllSessions() noexcept override;

  void CleanupSession(uint64_t session_id);

  ~HttpClient();

private:
  std::atomic<uint64_t> next_session_id_;
  std::map<uint64_t, std::shared_ptr<Session>> sessions_;
};

}  // namespace curl
}  // namespace client
}  // namespace http
}  // namespace ext
OPENTELEMETRY_END_NAMESPACE
