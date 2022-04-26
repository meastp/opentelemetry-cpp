// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/recordable.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
Recordable::~Recordable() = default;
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
