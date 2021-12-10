// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/context/runtime_context.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace context
{

nostd::shared_ptr<RuntimeContextStorage> &RuntimeContext::GetStorage() noexcept
{
  static nostd::shared_ptr<RuntimeContextStorage> context(GetDefaultStorage());
  return context;
}

ThreadLocalContextStorage::Stack &ThreadLocalContextStorage::GetStack()
{
  static thread_local Stack stack_ = Stack();
  return stack_;
}
}  // namespace context
OPENTELEMETRY_END_NAMESPACE