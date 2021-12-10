// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/context/propagation/global_propagator.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace context
{
namespace propagation
{

nostd::shared_ptr<TextMapPropagator> &GlobalTextMapPropagator::GetPropagator() noexcept
{
  static nostd::shared_ptr<TextMapPropagator> propagator(new NoOpPropagator());
  return propagator;
}
common::SpinLockMutex &GlobalTextMapPropagator::GetLock() noexcept
{
  static common::SpinLockMutex lock;
  return lock;
}
}  // namespace propagation
}  // namespace context
OPENTELEMETRY_END_NAMESPACE