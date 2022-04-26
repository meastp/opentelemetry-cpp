// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <mutex>
#include <vector>

#include "opentelemetry/sdk/trace/multi_recordable.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk_export.h"

#include <iostream>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{

/** Instantiation options. */
struct MultiSpanProcessorOptions
{};

/**
 * Span processor allow hooks for span start and end method invocations.
 *
 * Built-in span processors are responsible for batching and conversion of
 * spans to exportable representation and passing batches to exporters.
 */
class OTEL_SDK_API MultiSpanProcessor : public SpanProcessor
{
public:
  MultiSpanProcessor(std::vector<std::unique_ptr<SpanProcessor>> &&processors);

  void AddProcessor(std::unique_ptr<SpanProcessor> &&processor);

  std::unique_ptr<Recordable> MakeRecordable() noexcept override;

  virtual void OnStart(Recordable &span,
                       const opentelemetry::trace::SpanContext &parent_context) noexcept override;

  virtual void OnEnd(std::unique_ptr<Recordable> &&span) noexcept override;

  bool ForceFlush(
      std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept override;

  bool Shutdown(
      std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept override;

  ~MultiSpanProcessor();

private:
  struct ProcessorNode
  {
    std::unique_ptr<SpanProcessor> value_;
    ProcessorNode *next_, *prev_;
    ProcessorNode(std::unique_ptr<SpanProcessor> &&value,
                  ProcessorNode *prev = nullptr,
                  ProcessorNode *next = nullptr);
  };

  void Cleanup();

  ProcessorNode *head_, *tail_;
  size_t count_;
};
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE