// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/trace_state.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TraceState> TraceState::GetDefault()
{
  static nostd::shared_ptr<TraceState> ts{new TraceState()};
  return ts;
}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE