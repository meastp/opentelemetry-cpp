#pragma once

#include "opentelemetry/baggage/baggage.h"
#include "opentelemetry/context/propagation/text_map_propagator.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace baggage
{
namespace propagation
{

static const nostd::string_view kBaggageHeader = "baggage";

OTEL_API nostd::shared_ptr<baggage::Baggage> GetBaggage(const context::Context &context);

inline context::Context SetBaggage(context::Context &context,
                                   nostd::shared_ptr<baggage::Baggage> baggage)
{
  return context.SetValue(kBaggageHeader, baggage);
}

class BaggagePropagator : public context::propagation::TextMapPropagator
{
public:
  void Inject(context::propagation::TextMapCarrier &carrier,
              const context::Context &context) noexcept override
  {
    auto baggage = GetBaggage(context);
    carrier.Set(kBaggageHeader, baggage->ToHeader());
  }

  context::Context Extract(const context::propagation::TextMapCarrier &carrier,
                           context::Context &context) noexcept override
  {
    nostd::string_view baggage_str = carrier.Get(kBaggageHeader);
    auto baggage                   = Baggage::FromHeader(baggage_str);
    return SetBaggage(context, baggage);
  }
};
}  // namespace propagation
}  // namespace baggage
OPENTELEMETRY_END_NAMESPACE