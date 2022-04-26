// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/default_span.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
DefaultSpan DefaultSpan::GetInvalid()
{
  return DefaultSpan(SpanContext::GetInvalid());
}

trace::SpanContext DefaultSpan::GetContext() const noexcept
{
  return span_context_;
}

bool DefaultSpan::IsRecording() const noexcept
{
  return false;
}

void DefaultSpan::SetAttribute(nostd::string_view /* key */,
                               const common::AttributeValue & /* value */) noexcept
{}

void DefaultSpan::AddEvent(nostd::string_view /* name */) noexcept {}

void DefaultSpan::AddEvent(nostd::string_view /* name */,
                           common::SystemTimestamp /* timestamp */) noexcept
{}

void DefaultSpan::AddEvent(nostd::string_view /* name */,
                           common::SystemTimestamp /* timestamp */,
                           const common::KeyValueIterable & /* attributes */) noexcept
{}

void DefaultSpan::AddEvent(nostd::string_view name,
                           const common::KeyValueIterable &attributes) noexcept
{
  this->AddEvent(name, std::chrono::system_clock::now(), attributes);
}

void DefaultSpan::SetStatus(StatusCode /* status */, nostd::string_view /* description */) noexcept
{}

void DefaultSpan::UpdateName(nostd::string_view /* name */) noexcept {}

void DefaultSpan::End(const EndSpanOptions & /* options */) noexcept {}

nostd::string_view DefaultSpan::ToString()
{
  return "DefaultSpan";
}

DefaultSpan::DefaultSpan(SpanContext span_context) : span_context_(span_context) {}

// movable and copiable
DefaultSpan::DefaultSpan(DefaultSpan &&spn) : span_context_(spn.GetContext()) {}
DefaultSpan::DefaultSpan(const DefaultSpan &spn) : span_context_(spn.GetContext()) {}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE