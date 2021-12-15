// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include "opentelemetry/sdk/trace/id_generator.h"
#include "opentelemetry/sdk_export.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{

class OTEL_SDK_API RandomIdGenerator : public IdGenerator
{
public:
  opentelemetry::trace::SpanId GenerateSpanId() noexcept override;

  opentelemetry::trace::TraceId GenerateTraceId() noexcept override;
};

}  // namespace trace

}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE