// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/baggage/baggage.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace baggage
{

nostd::shared_ptr<Baggage> Baggage::GetDefault()
{
  static nostd::shared_ptr<Baggage> baggage{new Baggage()};
  return baggage;
}

}  // namespace baggage

OPENTELEMETRY_END_NAMESPACE