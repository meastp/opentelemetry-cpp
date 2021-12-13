// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/common/kv_properties.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace common
{

OTEL_HEADER_ONLY_API_INLINE nostd::string_view KeyValueStringTokenizer::GetDefaultKeyOrValue()
{
  static std::string default_str = "";
  return default_str;
}

}  // namespace common

OPENTELEMETRY_END_NAMESPACE