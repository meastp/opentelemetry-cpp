// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/exporter.h"
#include "src/common/random.h"

#include "opentelemetry/context/runtime_context.h"
#include "opentelemetry/trace/trace_flags.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
SpanExporter::~SpanExporter() = default;
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
