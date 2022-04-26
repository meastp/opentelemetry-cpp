// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/tracer.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
Tracer::~Tracer() = default;

Scope Tracer::WithActiveSpan(nostd::shared_ptr<Span> &span) noexcept
{
  return Scope{span};
}

nostd::shared_ptr<Span> Tracer::GetCurrentSpan() noexcept
{
  context::ContextValue active_span = context::RuntimeContext::GetValue(kSpanKey);
  if (nostd::holds_alternative<nostd::shared_ptr<Span>>(active_span))
  {
    return nostd::get<nostd::shared_ptr<Span>>(active_span);
  }
  else
  {
    return nostd::shared_ptr<Span>(new DefaultSpan(SpanContext::GetInvalid()));
  }
}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE