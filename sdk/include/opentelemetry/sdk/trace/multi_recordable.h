// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/common/timestamp.h"
#include "opentelemetry/sdk/trace/processor.h"
//#include "opentelemetry/sdk/trace/recordable.h"
#include "opentelemetry/sdk_export.h"
#include "opentelemetry/version.h"

#include <map>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
class OTEL_SDK_API MultiRecordable : public opentelemetry::sdk::trace::Recordable
{
public:
  MultiRecordable();

  MultiRecordable(const MultiRecordable &) = delete;
  MultiRecordable &operator=(const MultiRecordable &) = delete;

  void AddRecordable(const SpanProcessor &processor,
                     std::unique_ptr<opentelemetry::sdk::trace::Recordable> recordable) noexcept;

  const std::unique_ptr<opentelemetry::sdk::trace::Recordable> &GetRecordable(
      const SpanProcessor &processor) const noexcept;

  std::unique_ptr<opentelemetry::sdk::trace::Recordable> ReleaseRecordable(
      const SpanProcessor &processor) noexcept;

  void SetIdentity(const opentelemetry::trace::SpanContext &span_context,
                   opentelemetry::trace::SpanId parent_span_id) noexcept override;

  void SetAttribute(nostd::string_view key,
                    const opentelemetry::common::AttributeValue &value) noexcept override;

  void AddEvent(nostd::string_view name,
                opentelemetry::common::SystemTimestamp timestamp,
                const opentelemetry::common::KeyValueIterable &attributes) noexcept override;

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
  std::map<std::size_t, std::unique_ptr<opentelemetry::sdk::trace::Recordable>> recordables_;
};
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE