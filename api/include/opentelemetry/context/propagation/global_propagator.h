// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <mutex>

#include "opentelemetry/context/propagation/noop_propagator.h"
#include "opentelemetry/context/propagation/text_map_propagator.h"

#include "opentelemetry/common/spin_lock_mutex.h"
#include "opentelemetry/nostd/shared_ptr.h"

#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace context
{
namespace propagation
{

/* Stores the singleton TextMapPropagator */

class OTEL_API GlobalTextMapPropagator
{
public:
  static nostd::shared_ptr<TextMapPropagator> GetGlobalPropagator() noexcept
  {
    std::lock_guard<common::SpinLockMutex> guard(GetLock());
    return nostd::shared_ptr<TextMapPropagator>(GetPropagator());
  }

  static void SetGlobalPropagator(nostd::shared_ptr<TextMapPropagator> prop) noexcept
  {
    std::lock_guard<common::SpinLockMutex> guard(GetLock());
    GetPropagator() = prop;
  }

private:
  static nostd::shared_ptr<TextMapPropagator> &GetPropagator() noexcept;

  static common::SpinLockMutex &GetLock() noexcept;
};

}  // namespace propagation
}  // namespace context
OPENTELEMETRY_END_NAMESPACE