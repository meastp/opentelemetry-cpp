// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/span.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
Span::Span() = default;

Span ::~Span() = default;
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE