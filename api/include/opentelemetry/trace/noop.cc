// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/noop.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<Span> NoopTracer::StartSpan(
    nostd::string_view /*name*/,
    const common::KeyValueIterable & /*attributes*/,
    const SpanContextKeyValueIterable & /*links*/,
    const StartSpanOptions & /*options*/) noexcept
{
  // Don't allocate a no-op span for every StartSpan call, but use a static
  // singleton for this case.
  static nostd::shared_ptr<trace_api::Span> noop_span(
      new trace_api::NoopSpan{this->shared_from_this()});

  return noop_span;
}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE