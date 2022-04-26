// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/tracer_provider.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
TracerProvider::~TracerProvider() = default;
}  // namespace trace
OPENTELEMETRY_END_NAMESPACE