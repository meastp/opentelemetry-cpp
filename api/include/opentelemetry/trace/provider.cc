// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/provider.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TracerProvider> &Provider::GetProvider() noexcept
{
  static nostd::shared_ptr<TracerProvider> provider(new NoopTracerProvider);
  return provider;
}
OTEL_HEADER_ONLY_API_INLINE common::SpinLockMutex &Provider::GetLock() noexcept
{
  static common::SpinLockMutex lock;
  return lock;
}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE