// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/id_generator.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
IdGenerator::~IdGenerator() = default;
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
