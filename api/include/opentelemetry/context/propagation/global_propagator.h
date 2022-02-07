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
  static nostd::shared_ptr<TextMapPropagator> GetGlobalPropagator() noexcept;

  static void SetGlobalPropagator(nostd::shared_ptr<TextMapPropagator> prop) noexcept;

private:
  static nostd::shared_ptr<TextMapPropagator> &GetPropagator() noexcept;

  static common::SpinLockMutex &GetLock() noexcept;
};

}  // namespace propagation
}  // namespace context
OPENTELEMETRY_END_NAMESPACE

#ifdef OTEL_WITH_HEADER_ONLY_API
#  include "opentelemetry/context/propagation/global_propagator.cc"
#endif  // OTEL_WITH_HEADER_ONLY_API
