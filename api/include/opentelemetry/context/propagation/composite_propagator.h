// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include <initializer_list>
#include <memory>
#include <vector>
#include "opentelemetry/context/propagation/text_map_propagator.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace context
{
namespace propagation
{

class CompositePropagator : public TextMapPropagator
{
public:
  CompositePropagator(std::vector<std::unique_ptr<TextMapPropagator>> propagators)
      : propagators_(std::move(propagators))
  {}

  /**
   * Run each of the configured propagators with the given context and carrier.
   * Propagators are run in the order they are configured, so if multiple
   * propagators write the same carrier key, the propagator later in the list
   * will "win".
   *
   * @param carrier Carrier into which context will be injected
   * @param context Context to inject
   *
   */

  void Inject(TextMapCarrier &carrier, const context::Context &context) noexcept override
  {
    for (auto &p : propagators_)
    {
      p->Inject(carrier, context);
    }
  }

  /**
   * Run each of the configured propagators with the given context and carrier.
   * Propagators are run in the order they are configured, so if multiple
   * propagators write the same context key, the propagator later in the list
   * will "win".
   *
   * @param carrier Carrier from which to extract context
   * @param context Context to add values to
   */
  context::Context Extract(const TextMapCarrier &carrier,
                           context::Context &context) noexcept override
  {
    auto first = true;
    context::Context tmp_context;
    for (auto &p : propagators_)
    {
      if (first)
      {
        tmp_context = p->Extract(carrier, context);
        first       = false;
      }
      else
      {
        tmp_context = p->Extract(carrier, tmp_context);
      }
    }
    return propagators_.size() ? tmp_context : context;
  }

private:
  std::vector<std::unique_ptr<TextMapPropagator>> propagators_;
};
}  // namespace propagation
}  // namespace context
OPENTELEMETRY_END_NAMESPACE;
