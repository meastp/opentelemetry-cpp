// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>
#include <unordered_map>
#include <vector>
#include "opentelemetry/common/attribute_value.h"
#include "opentelemetry/common/timestamp.h"
#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/sdk/common/attribute_utils.h"
#include "opentelemetry/sdk/trace/recordable.h"
#include "opentelemetry/sdk_export.h"
#include "opentelemetry/trace/canonical_code.h"
#include "opentelemetry/trace/span.h"
#include "opentelemetry/trace/span_id.h"
#include "opentelemetry/trace/trace_id.h"

#include <string>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
/**
 * Class for storing events in SpanData.
 */
class OTEL_SDK_API SpanDataEvent
{
public:
  SpanDataEvent(std::string name,
                opentelemetry::common::SystemTimestamp timestamp,
                const opentelemetry::common::KeyValueIterable &attributes);

  /**
   * Get the name for this event
   * @return the name for this event
   */
  std::string GetName() const noexcept;

  /**
   * Get the timestamp for this event
   * @return the timestamp for this event
   */
  opentelemetry::common::SystemTimestamp GetTimestamp() const noexcept;

  /**
   * Get the attributes for this event
   * @return the attributes for this event
   */
  const std::unordered_map<std::string, common::OwnedAttributeValue> &GetAttributes()
      const noexcept;

private:
  std::string name_;
  opentelemetry::common::SystemTimestamp timestamp_;
  common::AttributeMap attribute_map_;
};

/**
 * Class for storing links in SpanData.
 */
class OTEL_SDK_API SpanDataLink
{
public:
  SpanDataLink(opentelemetry::trace::SpanContext span_context,
               const opentelemetry::common::KeyValueIterable &attributes);

  /**
   * Get the attributes for this link
   * @return the attributes for this link
   */
  const std::unordered_map<std::string, common::OwnedAttributeValue> &GetAttributes()
      const noexcept;

  /**
   * Get the span context for this link
   * @return the span context for this link
   */
  const opentelemetry::trace::SpanContext &GetSpanContext() const noexcept;

private:
  opentelemetry::trace::SpanContext span_context_;
  common::AttributeMap attribute_map_;
};

/**
 * SpanData is a representation of all data collected by a span.
 */
class OTEL_SDK_API SpanData final : public Recordable
{
public:
  SpanData();
  /**
   * Get the trace id for this span
   * @return the trace id for this span
   */
  opentelemetry::trace::TraceId GetTraceId() const noexcept;

  /**
   * Get the span id for this span
   * @return the span id for this span
   */
  opentelemetry::trace::SpanId GetSpanId() const noexcept;

  /**
   * Get the span context for this span
   * @return the span context for this span
   */
  const opentelemetry::trace::SpanContext &GetSpanContext() const noexcept;

  /**
   * Get the parent span id for this span
   * @return the span id for this span's parent
   */
  opentelemetry::trace::SpanId GetParentSpanId() const noexcept;

  /**
   * Get the name for this span
   * @return the name for this span
   */
  opentelemetry::nostd::string_view GetName() const noexcept;

  /**
   * Get the kind of this span
   * @return the kind of this span
   */
  opentelemetry::trace::SpanKind GetSpanKind() const noexcept;

  /**
   * Get the status for this span
   * @return the status for this span
   */
  opentelemetry::trace::StatusCode GetStatus() const noexcept;

  /**
   * Get the status description for this span
   * @return the description of the the status of this span
   */
  opentelemetry::nostd::string_view GetDescription() const noexcept;

  /**
   * Get the attributes associated with the resource
   * @returns the attributes associated with the resource configured for TracerProvider
   */

  const opentelemetry::sdk::resource::Resource &GetResource() const noexcept;

  /**
   * Get the attributes associated with the resource
   * @returns the attributes associated with the resource configured for TracerProvider
   */

  const opentelemetry::sdk::trace::InstrumentationLibrary &GetInstrumentationLibrary()
      const noexcept;

  /**
   * Get the start time for this span
   * @return the start time for this span
   */
  opentelemetry::common::SystemTimestamp GetStartTime() const noexcept;

  /**
   * Get the duration for this span
   * @return the duration for this span
   */
  std::chrono::nanoseconds GetDuration() const noexcept;

  /**
   * Get the attributes for this span
   * @return the attributes for this span
   */
  const std::unordered_map<std::string, common::OwnedAttributeValue> &GetAttributes()
      const noexcept;

  /**
   * Get the events associated with this span
   * @return the events associated with this span
   */
  const std::vector<SpanDataEvent> &GetEvents() const noexcept;

  /**
   * Get the links associated with this span
   * @return the links associated with this span
   */
  const std::vector<SpanDataLink> &GetLinks() const noexcept;

  void SetIdentity(const opentelemetry::trace::SpanContext &span_context,
                   opentelemetry::trace::SpanId parent_span_id) noexcept override;

  void SetAttribute(nostd::string_view key,
                    const opentelemetry::common::AttributeValue &value) noexcept override;

  void AddEvent(nostd::string_view name,
                opentelemetry::common::SystemTimestamp timestamp =
                    opentelemetry::common::SystemTimestamp(std::chrono::system_clock::now()),
                const opentelemetry::common::KeyValueIterable &attributes =
                    opentelemetry::common::KeyValueIterableView<std::map<std::string, int>>(
                        {})) noexcept override;

  void AddLink(const opentelemetry::trace::SpanContext &span_context,
               const opentelemetry::common::KeyValueIterable &attributes) noexcept override;

  void SetStatus(opentelemetry::trace::StatusCode code,
                 nostd::string_view description) noexcept override;

  void SetName(nostd::string_view name) noexcept override;

  void SetSpanKind(opentelemetry::trace::SpanKind span_kind) noexcept override;

  void SetResource(const opentelemetry::sdk::resource::Resource &resource) noexcept override;

  void SetStartTime(opentelemetry::common::SystemTimestamp start_time) noexcept override;

  void SetDuration(std::chrono::nanoseconds duration) noexcept override;

  void SetInstrumentationLibrary(
      const InstrumentationLibrary &instrumentation_library) noexcept override;

private:
  opentelemetry::trace::SpanContext span_context_{false, false};
  opentelemetry::trace::SpanId parent_span_id_;
  opentelemetry::common::SystemTimestamp start_time_;
  std::chrono::nanoseconds duration_{0};
  std::string name_;
  opentelemetry::trace::StatusCode status_code_{opentelemetry::trace::StatusCode::kUnset};
  std::string status_desc_;
  common::AttributeMap attribute_map_;
  std::vector<SpanDataEvent> events_;
  std::vector<SpanDataLink> links_;
  opentelemetry::trace::SpanKind span_kind_{opentelemetry::trace::SpanKind::kInternal};
  const opentelemetry::sdk::resource::Resource *resource_;
  const InstrumentationLibrary *instrumentation_library_;
};
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
