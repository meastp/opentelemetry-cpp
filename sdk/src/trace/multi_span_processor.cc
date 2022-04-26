// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/multi_span_processor.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
MultiSpanProcessor::MultiSpanProcessor(std::vector<std::unique_ptr<SpanProcessor>> &&processors)
    : head_(nullptr), tail_(nullptr), count_(0)
{
  for (auto &processor : processors)
  {
    AddProcessor(std::move(processor));
  }
}

void MultiSpanProcessor::AddProcessor(std::unique_ptr<SpanProcessor> &&processor)
{
  // Add preocessor to end of the list.
  if (processor)
  {
    ProcessorNode *pNode = new ProcessorNode(std::move(processor), tail_);
    if (count_ > 0)
    {
      tail_->next_ = pNode;
      tail_        = pNode;
    }
    else
    {
      head_ = tail_ = pNode;
    }
    count_++;
  }
}

std::unique_ptr<Recordable> MultiSpanProcessor::MakeRecordable() noexcept
{
  auto recordable       = std::unique_ptr<Recordable>(new MultiRecordable);
  auto multi_recordable = static_cast<MultiRecordable *>(recordable.get());
  ProcessorNode *node   = head_;
  while (node != nullptr)
  {
    auto processor = node->value_.get();
    multi_recordable->AddRecordable(*processor, processor->MakeRecordable());
    node = node->next_;
  }
  return recordable;
}

void MultiSpanProcessor::OnStart(Recordable &span,
                                 const opentelemetry::trace::SpanContext &parent_context) noexcept
{
  auto multi_recordable = static_cast<MultiRecordable *>(&span);
  ProcessorNode *node   = head_;
  while (node != nullptr)
  {
    auto processor   = node->value_.get();
    auto &recordable = multi_recordable->GetRecordable(*processor);
    if (recordable != nullptr)
    {
      processor->OnStart(*recordable, parent_context);
    }
    node = node->next_;
  }
}

void MultiSpanProcessor::OnEnd(std::unique_ptr<Recordable> &&span) noexcept
{
  auto multi_recordable = static_cast<MultiRecordable *>(span.release());
  ProcessorNode *node   = head_;
  while (node != nullptr)
  {
    auto processor  = node->value_.get();
    auto recordable = multi_recordable->ReleaseRecordable(*processor);
    if (recordable != nullptr)
    {
      processor->OnEnd(std::move(recordable));
    }
    node = node->next_;
  }
  delete multi_recordable;
}

bool MultiSpanProcessor::ForceFlush(std::chrono::microseconds timeout) noexcept
{
  bool result         = true;
  ProcessorNode *node = head_;
  while (node != nullptr)
  {
    auto processor = node->value_.get();
    result |= processor->ForceFlush(timeout);
    node = node->next_;
  }
  return result;
}

bool MultiSpanProcessor::Shutdown(std::chrono::microseconds timeout) noexcept
{
  bool result         = true;
  ProcessorNode *node = head_;
  while (node != nullptr)
  {
    auto processor = node->value_.get();
    result |= processor->Shutdown(timeout);
    node = node->next_;
  }
  return result;
}

MultiSpanProcessor::~MultiSpanProcessor()
{
  Shutdown();
  Cleanup();
}

MultiSpanProcessor::ProcessorNode::ProcessorNode(std::unique_ptr<SpanProcessor> &&value,
                                                 ProcessorNode *prev,
                                                 ProcessorNode *next)
    : value_(std::move(value)), next_(next), prev_(prev)
{}

void MultiSpanProcessor::Cleanup()
{
  if (count_)
  {
    ProcessorNode *node = tail_;
    while (node != nullptr)
    {
      if (node->next_ != nullptr)
      {
        delete node->next_;
        node->next_ = nullptr;
      }
      if (node->prev_ != nullptr)
      {
        node = node->prev_;
      }
      else
      {
        delete node;
        node = nullptr;
      }
    }
    head_ = tail_ = nullptr;
    count_        = 0;
  }
}

}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
