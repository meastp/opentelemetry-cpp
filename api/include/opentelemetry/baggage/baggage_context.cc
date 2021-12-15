// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/baggage/baggage_context.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace baggage
{

OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<opentelemetry::baggage::Baggage> GetBaggage(
    const opentelemetry::context::Context &context)
{
  context::ContextValue context_value = context.GetValue(kBaggageHeader);
  if (nostd::holds_alternative<nostd::shared_ptr<opentelemetry::baggage::Baggage>>(context_value))
  {
    return nostd::get<nostd::shared_ptr<opentelemetry::baggage::Baggage>>(context_value);
  }
  static nostd::shared_ptr<opentelemetry::baggage::Baggage> empty_baggage{
      new opentelemetry::baggage::Baggage()};
  return empty_baggage;
}

}  // namespace baggage

OPENTELEMETRY_END_NAMESPACE