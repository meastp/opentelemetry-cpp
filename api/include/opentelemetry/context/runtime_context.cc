// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/context/runtime_context.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace context
{

OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<RuntimeContextStorage>
    &RuntimeContext::GetStorage() noexcept
{
  static nostd::shared_ptr<RuntimeContextStorage> context(GetDefaultStorage());
  return context;
}

OTEL_HEADER_ONLY_API_INLINE ThreadLocalContextStorage::Stack &ThreadLocalContextStorage::GetStack()
{
  static thread_local Stack stack_ = Stack();
  return stack_;
}
}  // namespace context
OPENTELEMETRY_END_NAMESPACE