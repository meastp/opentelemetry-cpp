// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW

#  include <mutex>

#  include "opentelemetry/common/spin_lock_mutex.h"
#  include "opentelemetry/metrics/meter_provider.h"
#  include "opentelemetry/metrics/noop.h"
#  include "opentelemetry/nostd/shared_ptr.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace metrics
{
/**
 * Stores the singleton global MeterProvider.
 */
class OTEL_API Provider
{
public:
  /**
   * Returns the singleton MeterProvider.
   *
   * By default, a no-op MeterProvider is returned. This will never return a
   * nullptr MeterProvider.
   */
  static nostd::shared_ptr<MeterProvider> GetMeterProvider() noexcept
  {
    std::lock_guard<common::SpinLockMutex> guard(GetLock());
    return nostd::shared_ptr<MeterProvider>(GetProvider());
  }

  /**
   * Changes the singleton MeterProvider.
   */
  static void SetMeterProvider(nostd::shared_ptr<MeterProvider> tp) noexcept
  {
    std::lock_guard<common::SpinLockMutex> guard(GetLock());
    GetProvider() = tp;
  }

private:
  static nostd::shared_ptr<MeterProvider> &GetProvider() noexcept;

  static common::SpinLockMutex &GetLock() noexcept;
};

}  // namespace metrics
OPENTELEMETRY_END_NAMESPACE

#  ifdef OTEL_WITH_HEADER_ONLY_API
#    include "opentelemetry/metrics/provider.cc"
#  endif  // OTEL_WITH_HEADER_ONLY_API
#endif