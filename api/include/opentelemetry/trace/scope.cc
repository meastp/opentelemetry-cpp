// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/scope.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
Scope::Scope(const nostd::shared_ptr<Span> &span) noexcept
    : token_(context::RuntimeContext::Attach(
          context::RuntimeContext::GetCurrent().SetValue(kSpanKey, span)))
{}
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE