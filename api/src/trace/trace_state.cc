// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/trace_state.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
nostd::shared_ptr<TraceState> TraceState::GetDefault()
{
  static nostd::shared_ptr<TraceState> ts{new TraceState()};
  return ts;
}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE