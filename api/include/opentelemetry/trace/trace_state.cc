// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/trace/trace_state.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace trace
{
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TraceState> TraceState::GetDefault()
{
  static nostd::shared_ptr<TraceState> ts{new TraceState()};
  return ts;
}
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TraceState> TraceState::FromHeader(
    nostd::string_view header)
{

  common::KeyValueStringTokenizer kv_str_tokenizer(header);
  size_t cnt = kv_str_tokenizer.NumTokens();  // upper bound on number of kv pairs
  if (cnt > kMaxKeyValuePairs)
  {
    cnt = kMaxKeyValuePairs;
  }

  nostd::shared_ptr<TraceState> ts(new TraceState(cnt));
  bool kv_valid;
  nostd::string_view key, value;
  while (kv_str_tokenizer.next(kv_valid, key, value) && ts->kv_properties_->Size() < cnt)
  {
    if (kv_valid == false)
    {
      return GetDefault();
    }

    if (!IsValidKey(key) || !IsValidValue(value))
    {
      // invalid header. return empty TraceState
      ts->kv_properties_.reset(new opentelemetry::common::KeyValueProperties());
      break;
    }

    ts->kv_properties_->AddEntry(key, value);
  }

  return ts;
}

OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TraceState> TraceState::Set(
    const nostd::string_view &key,
    const nostd::string_view &value)
{
  auto curr_size = kv_properties_->Size();
  if (!IsValidKey(key) || !IsValidValue(value))
  {
    // max size reached or invalid key/value. Returning empty TraceState
    return TraceState::GetDefault();
  }
  auto allocate_size = curr_size;
  if (curr_size < kMaxKeyValuePairs)
  {
    allocate_size += 1;
  }
  nostd::shared_ptr<TraceState> ts(new TraceState(allocate_size));
  if (curr_size < kMaxKeyValuePairs)
  {
    // add new field first
    ts->kv_properties_->AddEntry(key, value);
  }
  // add rest of the fields.
  kv_properties_->GetAllEntries([&ts](nostd::string_view key, nostd::string_view value) {
    ts->kv_properties_->AddEntry(key, value);
    return true;
  });
  return ts;
}
OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<TraceState> TraceState::Delete(
    const nostd::string_view &key)
{
  if (!IsValidKey(key))
  {
    return TraceState::GetDefault();
  }
  auto curr_size     = kv_properties_->Size();
  auto allocate_size = curr_size;
  std::string unused;
  if (kv_properties_->GetValue(key, unused))
  {
    allocate_size -= 1;
  }
  nostd::shared_ptr<TraceState> ts(new TraceState(allocate_size));
  kv_properties_->GetAllEntries([&ts, &key](nostd::string_view e_key, nostd::string_view e_value) {
    if (key != e_key)
      ts->kv_properties_->AddEntry(e_key, e_value);
    return true;
  });
  return ts;
}
TraceState::TraceState() : kv_properties_(new opentelemetry::common::KeyValueProperties()){};
TraceState::TraceState(size_t size)
    : kv_properties_(new opentelemetry::common::KeyValueProperties(size)){};

}  // namespace trace
OPENTELEMETRY_END_NAMESPACE