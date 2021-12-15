// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace metrics
{
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<MeterProvider> &Provider::GetProvider() noexcept
{
  static nostd::shared_ptr<MeterProvider> provider(new NoopMeterProvider);
  return provider;
}
OTEL_HEADER_ONLY_API_INLINE common::SpinLockMutex &Provider::GetLock() noexcept
{
  static common::SpinLockMutex lock;
  return lock;
}
}  // namespace metrics
OPENTELEMETRY_END_NAMESPACE