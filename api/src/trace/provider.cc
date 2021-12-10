// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/provider.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
nostd::shared_ptr<TracerProvider> &Provider::GetProvider() noexcept
{
  static nostd::shared_ptr<TracerProvider> provider(new NoopTracerProvider);
  return provider;
}
common::SpinLockMutex &Provider::GetLock() noexcept
{
  static common::SpinLockMutex lock;
  return lock;
}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE