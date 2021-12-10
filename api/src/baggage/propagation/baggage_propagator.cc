#include "opentelemetry/baggage/propagation/baggage_propagator.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace baggage
{
namespace propagation
{
nostd::shared_ptr<baggage::Baggage> GetBaggage(const context::Context &context)
{
  context::ContextValue context_value = context.GetValue(kBaggageHeader);
  if (nostd::holds_alternative<nostd::shared_ptr<baggage::Baggage>>(context_value))
  {
    return nostd::get<nostd::shared_ptr<baggage::Baggage>>(context_value);
  }
  static nostd::shared_ptr<baggage::Baggage> empty_baggage{new baggage::Baggage()};
  return empty_baggage;
}
}  // namespace propagation
}  // namespace baggage
OPENTELEMETRY_END_NAMESPACE