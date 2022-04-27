// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/ext/http/client/curl/http_client_curl.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace ext
{
namespace http
{
namespace client
{
namespace curl
{
Request::Request() : method_(opentelemetry::ext::http::client::Method::Get), uri_("/") {}

void Request::SetMethod(opentelemetry::ext::http::client::Method method) noexcept
{
  method_ = method;
}

void Request::SetBody(opentelemetry::ext::http::client::Body &body) noexcept
{
  body_ = std::move(body);
}

void Request::AddHeader(nostd::string_view name, nostd::string_view value) noexcept
{
  headers_.insert(std::pair<std::string, std::string>(static_cast<std::string>(name),
                                                      static_cast<std::string>(value)));
}

void Request::ReplaceHeader(nostd::string_view name, nostd::string_view value) noexcept
{
  // erase matching headers
  auto range = headers_.equal_range(static_cast<std::string>(name));
  headers_.erase(range.first, range.second);
  AddHeader(name, value);
}

void Request::SetUri(nostd::string_view uri) noexcept
{
  uri_ = static_cast<std::string>(uri);
}

void Request::SetTimeoutMs(std::chrono::milliseconds timeout_ms) noexcept
{
  timeout_ms_ = timeout_ms;
}

Response::Response() : status_code_(Http_Ok) {}

const opentelemetry::ext::http::client::Body &Response::GetBody() const noexcept
{
  return body_;
}

bool Response::ForEachHeader(
    nostd::function_ref<bool(nostd::string_view name, nostd::string_view value)> callable)
    const noexcept
{
  for (const auto &header : headers_)
  {
    if (!callable(header.first, header.second))
    {
      return false;
    }
  }
  return true;
}

bool Response::ForEachHeader(
    const nostd::string_view &name,
    nostd::function_ref<bool(nostd::string_view name, nostd::string_view value)> callable)
    const noexcept
{
  auto range = headers_.equal_range(static_cast<std::string>(name));
  for (auto it = range.first; it != range.second; ++it)
  {
    if (!callable(it->first, it->second))
    {
      return false;
    }
  }
  return true;
}

opentelemetry::ext::http::client::StatusCode Response::GetStatusCode() const noexcept
{
  return status_code_;
}

Session::Session(HttpClient &http_client,
                 std::string scheme,
                 const std::string &host,
                 uint16_t port)
    : http_client_(http_client), is_session_active_(false)
{
  host_ = scheme + "://" + host + ":" + std::to_string(port) + "/";
}

std::shared_ptr<opentelemetry::ext::http::client::Request> Session::CreateRequest() noexcept
{
  http_request_.reset(new Request());
  return http_request_;
}

void Session::SendRequest(opentelemetry::ext::http::client::EventHandler &callback) noexcept
{
  is_session_active_ = true;
  std::string url    = host_ + std::string(http_request_->uri_);
  auto callback_ptr  = &callback;
  curl_operation_.reset(new HttpOperation(http_request_->method_, url, callback_ptr,
                                          RequestMode::Async, http_request_->headers_,
                                          http_request_->body_, false, http_request_->timeout_ms_));
  curl_operation_->SendAsync([this, callback_ptr](HttpOperation &operation) {
    if (operation.WasAborted())
    {
      // Manually cancelled
      callback_ptr->OnEvent(opentelemetry::ext::http::client::SessionState::Cancelled, "");
    }

    if (operation.GetResponseCode() >= CURL_LAST)
    {
      // we have a http response
      auto response          = std::unique_ptr<Response>(new Response());
      response->headers_     = operation.GetResponseHeaders();
      response->body_        = operation.GetResponseBody();
      response->status_code_ = operation.GetResponseCode();
      callback_ptr->OnResponse(*response);
    }
    is_session_active_ = false;
  });
}

bool opentelemetry::ext::http::client::curl::Session::CancelSession() noexcept
{
  curl_operation_->Abort();
  http_client_.CleanupSession(session_id_);
  return true;
}

bool opentelemetry::ext::http::client::curl::Session::FinishSession() noexcept
{
  curl_operation_->Finish();
  http_client_.CleanupSession(session_id_);
  return true;
}

bool Session::IsSessionActive() noexcept
{
  return is_session_active_;
}

void Session::SetId(uint64_t session_id)
{
  session_id_ = session_id;
}

/**
 * Returns the base URI.
 * @return the base URI as a string consisting of scheme, host and port.
 */
const std::string &Session::GetBaseUri() const
{
  return host_;
}

HttpClientSync::HttpClientSync()
{
  curl_global_init(CURL_GLOBAL_ALL);
}

opentelemetry::ext::http::client::Result HttpClientSync::Get(
    const nostd::string_view &url,
    const opentelemetry::ext::http::client::Headers &headers) noexcept
{
  opentelemetry::ext::http::client::Body body;
  HttpOperation curl_operation(opentelemetry::ext::http::client::Method::Get, url.data(), nullptr,
                               RequestMode::Sync, headers, body);
  curl_operation.SendSync();
  auto session_state = curl_operation.GetSessionState();
  if (curl_operation.WasAborted())
  {
    session_state = opentelemetry::ext::http::client::SessionState::Cancelled;
  }
  auto response = std::unique_ptr<Response>(new Response());
  if (curl_operation.GetResponseCode() >= CURL_LAST)
  {
    // we have a http response

    response->headers_     = curl_operation.GetResponseHeaders();
    response->body_        = curl_operation.GetResponseBody();
    response->status_code_ = curl_operation.GetResponseCode();
  }
  return opentelemetry::ext::http::client::Result(std::move(response), session_state);
}

opentelemetry::ext::http::client::Result HttpClientSync::Post(
    const nostd::string_view &url,
    const Body &body,
    const opentelemetry::ext::http::client::Headers &headers) noexcept
{
  HttpOperation curl_operation(opentelemetry::ext::http::client::Method::Post, url.data(), nullptr,
                               RequestMode::Sync, headers, body);
  curl_operation.SendSync();
  auto session_state = curl_operation.GetSessionState();
  if (curl_operation.WasAborted())
  {
    session_state = opentelemetry::ext::http::client::SessionState::Cancelled;
  }
  auto response = std::unique_ptr<Response>(new Response());
  if (curl_operation.GetResponseCode() >= CURL_LAST)
  {
    // we have a http response

    response->headers_     = curl_operation.GetResponseHeaders();
    response->body_        = curl_operation.GetResponseBody();
    response->status_code_ = curl_operation.GetResponseCode();
  }

  return opentelemetry::ext::http::client::Result(std::move(response), session_state);
}

HttpClientSync::~HttpClientSync()
{
  curl_global_cleanup();
}

HttpClient::HttpClient() : next_session_id_{0}
{
  curl_global_init(CURL_GLOBAL_ALL);
}

std::shared_ptr<opentelemetry::ext::http::client::Session> HttpClient::CreateSession(
    nostd::string_view url) noexcept
{
  auto parsedUrl = common::UrlParser(std::string(url));
  if (!parsedUrl.success_)
  {
    return std::make_shared<Session>(*this);
  }
  auto session =
      std::make_shared<Session>(*this, parsedUrl.scheme_, parsedUrl.host_, parsedUrl.port_);
  auto session_id = ++next_session_id_;
  session->SetId(session_id);
  sessions_.insert({session_id, session});
  return session;
}

bool HttpClient::CancelAllSessions() noexcept
{
  for (auto &session : sessions_)
  {
    session.second->CancelSession();
  }
  return true;
}

bool HttpClient::FinishAllSessions() noexcept
{
  for (auto &session : sessions_)
  {
    session.second->FinishSession();
  }
  return true;
}

void HttpClient::CleanupSession(uint64_t session_id)
{
  // TBD = Need to be thread safe
  sessions_.erase(session_id);
}

HttpClient::~HttpClient()
{
  curl_global_cleanup();
}

}  // namespace curl
}  // namespace client
}  // namespace http
}  // namespace ext
OPENTELEMETRY_END_NAMESPACE