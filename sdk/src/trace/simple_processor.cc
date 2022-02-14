// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/simple_processor.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
SimpleSpanProcessor::SimpleSpanProcessor(std::unique_ptr<SpanExporter> &&exporter) noexcept
    : exporter_(std::move(exporter))
{}

std::unique_ptr<Recordable> SimpleSpanProcessor::MakeRecordable() noexcept
{
  return exporter_->MakeRecordable();
}

void SimpleSpanProcessor::OnStart(Recordable &span,
                                  const opentelemetry::trace::SpanContext &parent_context) noexcept
{}

void SimpleSpanProcessor::OnEnd(std::unique_ptr<Recordable> &&span) noexcept
{
  nostd::span<std::unique_ptr<Recordable>> batch(&span, 1);
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  if (exporter_->Export(batch) == sdk::common::ExportResult::kFailure)
  {
    /* Once it is defined how the SDK does logging, an error should be
     * logged in this case. */
  }
}

bool SimpleSpanProcessor::ForceFlush(std::chrono::microseconds timeout) noexcept
{
  return true;
}

bool SimpleSpanProcessor::Shutdown(std::chrono::microseconds timeout) noexcept
{
  // We only call shutdown ONCE.
  if (exporter_ != nullptr && !shutdown_latch_.test_and_set(std::memory_order_acquire))
  {
    return exporter_->Shutdown(timeout);
  }
  return true;
}

SimpleSpanProcessor::~SimpleSpanProcessor()
{
  Shutdown();
}
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
