// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/span_data.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
SpanDataEvent::SpanDataEvent(std::string name,
                             opentelemetry::common::SystemTimestamp timestamp,
                             const opentelemetry::common::KeyValueIterable &attributes)
    : name_(name), timestamp_(timestamp), attribute_map_(attributes)
{}

/**
 * Get the name for this event
 * @return the name for this event
 */
std::string SpanDataEvent::GetName() const noexcept
{
  return name_;
}

/**
 * Get the timestamp for this event
 * @return the timestamp for this event
 */
opentelemetry::common::SystemTimestamp SpanDataEvent::GetTimestamp() const noexcept
{
  return timestamp_;
}

/**
 * Get the attributes for this event
 * @return the attributes for this event
 */
const std::unordered_map<std::string, common::OwnedAttributeValue> &SpanDataEvent::GetAttributes()
    const noexcept
{
  return attribute_map_.GetAttributes();
}

SpanDataLink::SpanDataLink(opentelemetry::trace::SpanContext span_context,
                           const opentelemetry::common::KeyValueIterable &attributes)
    : span_context_(span_context), attribute_map_(attributes)
{}

/**
 * Get the attributes for this link
 * @return the attributes for this link
 */
const std::unordered_map<std::string, common::OwnedAttributeValue> &SpanDataLink::GetAttributes()
    const noexcept
{
  return attribute_map_.GetAttributes();
}

/**
 * Get the span context for this link
 * @return the span context for this link
 */
const opentelemetry::trace::SpanContext &SpanDataLink::GetSpanContext() const noexcept
{
  return span_context_;
}

SpanData::SpanData() : resource_{nullptr}, instrumentation_library_{nullptr} {}
/**
 * Get the trace id for this span
 * @return the trace id for this span
 */
opentelemetry::trace::TraceId SpanData::GetTraceId() const noexcept
{
  return span_context_.trace_id();
}

/**
 * Get the span id for this span
 * @return the span id for this span
 */
opentelemetry::trace::SpanId SpanData::GetSpanId() const noexcept
{
  return span_context_.span_id();
}

/**
 * Get the span context for this span
 * @return the span context for this span
 */
const opentelemetry::trace::SpanContext &SpanData::GetSpanContext() const noexcept
{
  return span_context_;
}

/**
 * Get the parent span id for this span
 * @return the span id for this span's parent
 */
opentelemetry::trace::SpanId SpanData::GetParentSpanId() const noexcept
{
  return parent_span_id_;
}

/**
 * Get the name for this span
 * @return the name for this span
 */
opentelemetry::nostd::string_view SpanData::GetName() const noexcept
{
  return name_;
}

/**
 * Get the kind of this span
 * @return the kind of this span
 */
opentelemetry::trace::SpanKind SpanData::GetSpanKind() const noexcept
{
  return span_kind_;
}

/**
 * Get the status for this span
 * @return the status for this span
 */
opentelemetry::trace::StatusCode SpanData::GetStatus() const noexcept
{
  return status_code_;
}

/**
 * Get the status description for this span
 * @return the description of the the status of this span
 */
opentelemetry::nostd::string_view SpanData::GetDescription() const noexcept
{
  return status_desc_;
}

/**
 * Get the attributes associated with the resource
 * @returns the attributes associated with the resource configured for TracerProvider
 */

const opentelemetry::sdk::resource::Resource &SpanData::GetResource() const noexcept
{
  if (resource_ == nullptr)
  {
    // this shouldn't happen as TraceProvider provides default resources
    static opentelemetry::sdk::resource::Resource resource =
        opentelemetry::sdk::resource::Resource::GetEmpty();
    return resource;
  }
  return *resource_;
}

/**
 * Get the attributes associated with the resource
 * @returns the attributes associated with the resource configured for TracerProvider
 */

const opentelemetry::sdk::trace::InstrumentationLibrary &SpanData::GetInstrumentationLibrary()
    const noexcept
{
  if (instrumentation_library_ == nullptr)
  {
    // this shouldn't happen as Tracer ensures there is valid default instrumentation library.
    static std::unique_ptr<opentelemetry::sdk::instrumentationlibrary::InstrumentationLibrary>
        instrumentation_library =
            opentelemetry::sdk::instrumentationlibrary::InstrumentationLibrary::Create(
                "unknown_service");
    return *instrumentation_library;
  }
  return *instrumentation_library_;
}

/**
 * Get the start time for this span
 * @return the start time for this span
 */
opentelemetry::common::SystemTimestamp SpanData::GetStartTime() const noexcept
{
  return start_time_;
}

/**
 * Get the duration for this span
 * @return the duration for this span
 */
std::chrono::nanoseconds SpanData::GetDuration() const noexcept
{
  return duration_;
}

/**
 * Get the attributes for this span
 * @return the attributes for this span
 */
const std::unordered_map<std::string, common::OwnedAttributeValue> &SpanData::GetAttributes()
    const noexcept
{
  return attribute_map_.GetAttributes();
}

/**
 * Get the events associated with this span
 * @return the events associated with this span
 */
const std::vector<SpanDataEvent> &SpanData::GetEvents() const noexcept
{
  return events_;
}

/**
 * Get the links associated with this span
 * @return the links associated with this span
 */
const std::vector<SpanDataLink> &SpanData::GetLinks() const noexcept
{
  return links_;
}

void SpanData::SetIdentity(const opentelemetry::trace::SpanContext &span_context,
                           opentelemetry::trace::SpanId parent_span_id) noexcept
{
  span_context_   = span_context;
  parent_span_id_ = parent_span_id;
}

void SpanData::SetAttribute(nostd::string_view key,
                            const opentelemetry::common::AttributeValue &value) noexcept
{
  attribute_map_.SetAttribute(key, value);
}

void SpanData::AddEvent(nostd::string_view name,
                        opentelemetry::common::SystemTimestamp timestamp,
                        const opentelemetry::common::KeyValueIterable &attributes) noexcept
{
  SpanDataEvent event(std::string(name), timestamp, attributes);
  events_.push_back(event);
}

void SpanData::AddLink(const opentelemetry::trace::SpanContext &span_context,
                       const opentelemetry::common::KeyValueIterable &attributes) noexcept
{
  SpanDataLink link(span_context, attributes);
  links_.push_back(link);
}

void SpanData::SetStatus(opentelemetry::trace::StatusCode code,
                         nostd::string_view description) noexcept
{
  status_code_ = code;
  status_desc_ = std::string(description);
}

void SpanData::SetName(nostd::string_view name) noexcept
{
  name_ = std::string(name.data(), name.length());
}

void SpanData::SetSpanKind(opentelemetry::trace::SpanKind span_kind) noexcept
{
  span_kind_ = span_kind;
}

void SpanData::SetResource(const opentelemetry::sdk::resource::Resource &resource) noexcept
{
  resource_ = &resource;
}

void SpanData::SetStartTime(opentelemetry::common::SystemTimestamp start_time) noexcept
{
  start_time_ = start_time;
}

void SpanData::SetDuration(std::chrono::nanoseconds duration) noexcept
{
  duration_ = duration;
}

void SpanData::SetInstrumentationLibrary(
    const InstrumentationLibrary &instrumentation_library) noexcept
{
  instrumentation_library_ = &instrumentation_library;
}

}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
