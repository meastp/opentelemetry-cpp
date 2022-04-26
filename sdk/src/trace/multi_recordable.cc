// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/multi_recordable.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
namespace
{
std::size_t MakeKey(const SpanProcessor &processor)
{
  return reinterpret_cast<std::size_t>(&processor);
}
}  // namespace

MultiRecordable::MultiRecordable() = default;

void MultiRecordable::AddRecordable(const SpanProcessor &processor,
                                    std::unique_ptr<Recordable> recordable) noexcept
{
  recordables_[MakeKey(processor)] = std::move(recordable);
}

const std::unique_ptr<Recordable> &MultiRecordable::GetRecordable(
    const SpanProcessor &processor) const noexcept
{
  // TODO - return nullptr ref on failed lookup?
  auto i = recordables_.find(MakeKey(processor));
  if (i != recordables_.end())
  {
    return i->second;
  }
  static std::unique_ptr<Recordable> empty(nullptr);
  return empty;
}

std::unique_ptr<Recordable> MultiRecordable::ReleaseRecordable(
    const SpanProcessor &processor) noexcept
{
  auto i = recordables_.find(MakeKey(processor));
  if (i != recordables_.end())
  {
    std::unique_ptr<Recordable> result(i->second.release());
    recordables_.erase(MakeKey(processor));
    return result;
  }
  return std::unique_ptr<Recordable>(nullptr);
}

void MultiRecordable::SetIdentity(const opentelemetry::trace::SpanContext &span_context,
                                  opentelemetry::trace::SpanId parent_span_id) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetIdentity(span_context, parent_span_id);
  }
}

void MultiRecordable::SetAttribute(nostd::string_view key,
                                   const opentelemetry::common::AttributeValue &value) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetAttribute(key, value);
  }
}

void MultiRecordable::AddEvent(nostd::string_view name,
                               opentelemetry::common::SystemTimestamp timestamp,
                               const opentelemetry::common::KeyValueIterable &attributes) noexcept
{

  for (auto &recordable : recordables_)
  {
    recordable.second->AddEvent(name, timestamp, attributes);
  }
}

void MultiRecordable::AddLink(const opentelemetry::trace::SpanContext &span_context,
                              const opentelemetry::common::KeyValueIterable &attributes) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->AddLink(span_context, attributes);
  }
}

void MultiRecordable::SetStatus(opentelemetry::trace::StatusCode code,
                                nostd::string_view description) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetStatus(code, description);
  }
}

void MultiRecordable::SetName(nostd::string_view name) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetName(name);
  }
}

void MultiRecordable::SetSpanKind(opentelemetry::trace::SpanKind span_kind) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetSpanKind(span_kind);
  }
}

void MultiRecordable::SetResource(const opentelemetry::sdk::resource::Resource &resource) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetResource(resource);
  }
}

void MultiRecordable::SetStartTime(opentelemetry::common::SystemTimestamp start_time) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetStartTime(start_time);
  }
}

void MultiRecordable::SetDuration(std::chrono::nanoseconds duration) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetDuration(duration);
  }
}

void MultiRecordable::SetInstrumentationLibrary(
    const InstrumentationLibrary &instrumentation_library) noexcept
{
  for (auto &recordable : recordables_)
  {
    recordable.second->SetInstrumentationLibrary(instrumentation_library);
  }
}
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
