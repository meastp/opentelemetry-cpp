// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/baggage/baggage.h"
#include "opentelemetry/context/context.h"
#include "opentelemetry/export.h"
#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace baggage
{

static const std::string kBaggageHeader = "baggage";

OTEL_API nostd::shared_ptr<opentelemetry::baggage::Baggage> GetBaggage(
    const opentelemetry::context::Context &context);

inline context::Context SetBaggage(opentelemetry::context::Context &context,
                                   nostd::shared_ptr<opentelemetry::baggage::Baggage> baggage)
{
  return context.SetValue(kBaggageHeader, baggage);
}

}  // namespace baggage
OPENTELEMETRY_END_NAMESPACE

#ifdef OTEL_WITH_HEADER_ONLY_API
#  include "opentelemetry/baggage/baggage_context.cc"
#endif  // OTEL_WITH_HEADER_ONLY_API
