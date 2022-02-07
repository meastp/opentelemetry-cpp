// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/context/context.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace context
{
Context::Context()  = default;
Context::~Context() = default;
}  // namespace context
OPENTELEMETRY_END_NAMESPACE