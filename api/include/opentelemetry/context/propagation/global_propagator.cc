// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/context/propagation/global_propagator.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace context
{
namespace propagation
{
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TextMapPropagator>
GlobalTextMapPropagator::GetGlobalPropagator() noexcept
{
  std::lock_guard<common::SpinLockMutex> guard(GetLock());
  return nostd::shared_ptr<TextMapPropagator>(GetPropagator());
}

OTEL_HEADER_ONLY_API_INLINE void GlobalTextMapPropagator::SetGlobalPropagator(
    nostd::shared_ptr<TextMapPropagator> prop) noexcept
{
  std::lock_guard<common::SpinLockMutex> guard(GetLock());
  GetPropagator() = prop;
}
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TextMapPropagator>
    &GlobalTextMapPropagator::GetPropagator() noexcept
{
  static nostd::shared_ptr<TextMapPropagator> propagator(new NoOpPropagator());
  return propagator;
}
OTEL_HEADER_ONLY_API_INLINE common::SpinLockMutex &GlobalTextMapPropagator::GetLock() noexcept
{
  static common::SpinLockMutex lock;
  return lock;
}
}  // namespace propagation
}  // namespace context
OPENTELEMETRY_END_NAMESPACE