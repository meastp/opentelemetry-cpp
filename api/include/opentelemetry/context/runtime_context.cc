// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/context/runtime_context.h"
#include "opentelemetry/export.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE

namespace context
{

Token::Token() noexcept = default;
Token::Token(const Context &context) : context_(context) {}

OTEL_HEADER_ONLY_API_INLINE Token::~Token()
{
  context::RuntimeContext::Detach(*this);
}

OTEL_HEADER_ONLY_API_INLINE nostd::unique_ptr<Token> RuntimeContextStorage::CreateToken(
    const Context &context) noexcept
{
  return nostd::unique_ptr<Token>(new Token(context));
}

RuntimeContextStorage::~RuntimeContextStorage() {}

OTEL_HEADER_ONLY_API_INLINE nostd::shared_ptr<RuntimeContextStorage>
    &RuntimeContext::GetStorage() noexcept
{
  static nostd::shared_ptr<RuntimeContextStorage> context(GetDefaultStorage());
  return context;
}

RuntimeContextStorage *GetDefaultStorage() noexcept
{
  return new ThreadLocalContextStorage();
}

Context RuntimeContext::GetCurrent() noexcept
{
  return GetRuntimeContextStorage()->GetCurrent();
}

nostd::unique_ptr<Token> RuntimeContext::Attach(const Context &context) noexcept
{
  return GetRuntimeContextStorage()->Attach(context);
}

bool RuntimeContext::Detach(Token &token) noexcept
{
  return GetRuntimeContextStorage()->Detach(token);
}

Context RuntimeContext::SetValue(nostd::string_view key,
                                 const ContextValue &value,
                                 Context *context) noexcept
{
  Context temp_context;
  if (context == nullptr)
  {
    temp_context = GetCurrent();
  }
  else
  {
    temp_context = *context;
  }
  return temp_context.SetValue(key, value);
}

ContextValue RuntimeContext::GetValue(nostd::string_view key, Context *context) noexcept
{
  Context temp_context;
  if (context == nullptr)
  {
    temp_context = GetCurrent();
  }
  else
  {
    temp_context = *context;
  }
  return temp_context.GetValue(key);
}

void RuntimeContext::SetRuntimeContextStorage(
    nostd::shared_ptr<RuntimeContextStorage> storage) noexcept
{
  GetStorage() = storage;
}

nostd::shared_ptr<RuntimeContextStorage> RuntimeContext::GetRuntimeContextStorage() noexcept
{
  return GetStorage();
}

ThreadLocalContextStorage::ThreadLocalContextStorage() noexcept = default;

Context ThreadLocalContextStorage::GetCurrent() noexcept
{
  return GetStack().Top();
}

bool ThreadLocalContextStorage::Detach(Token &token) noexcept
{
  // In most cases, the context to be detached is on the top of the stack.
  if (token == GetStack().Top())
  {
    GetStack().Pop();
    return true;
  }

  if (!GetStack().Contains(token))
  {
    return false;
  }

  while (!(token == GetStack().Top()))
  {
    GetStack().Pop();
  }

  GetStack().Pop();

  return true;
}

nostd::unique_ptr<Token> ThreadLocalContextStorage::Attach(const Context &context) noexcept
{
  GetStack().Push(context);
  return CreateToken(context);
}

ThreadLocalContextStorage::Stack::Stack() noexcept : size_(0), capacity_(0), base_(nullptr){};

void ThreadLocalContextStorage::Stack::Pop() noexcept
{
  if (size_ == 0)
  {
    return;
  }
  // Store empty Context before decrementing `size`, to ensure
  // the shared_ptr object (if stored in prev context object ) are released.
  // The stack is not resized, and the unused memory would be reutilised
  // for subsequent context storage.
  base_[size_ - 1] = Context();
  size_ -= 1;
}

bool ThreadLocalContextStorage::Stack::Contains(const Token &token) const noexcept
{
  for (size_t pos = size_; pos > 0; --pos)
  {
    if (token == base_[pos - 1])
    {
      return true;
    }
  }

  return false;
}

Context ThreadLocalContextStorage::Stack::Top() const noexcept
{
  if (size_ == 0)
  {
    return Context();
  }
  return base_[size_ - 1];
}

void ThreadLocalContextStorage::Stack::Push(const Context &context) noexcept
{
  size_++;
  if (size_ > capacity_)
  {
    Resize(size_ * 2);
  }
  base_[size_ - 1] = context;
}

void ThreadLocalContextStorage::Stack::Resize(size_t new_capacity) noexcept
{
  size_t old_size = size_ - 1;
  if (new_capacity == 0)
  {
    new_capacity = 2;
  }
  Context *temp = new Context[new_capacity];
  if (base_ != nullptr)
  {
    // vs2015 does not like this construct considering it unsafe:
    // - std::copy(base_, base_ + old_size, temp);
    // Ref.
    // https://stackoverflow.com/questions/12270224/xutility2227-warning-c4996-std-copy-impl
    for (size_t i = 0; i < (std::min)(old_size, new_capacity); i++)
    {
      temp[i] = base_[i];
    }
    delete[] base_;
  }
  base_     = temp;
  capacity_ = new_capacity;
}

ThreadLocalContextStorage::Stack::~Stack() noexcept
{
  delete[] base_;
}

OTEL_HEADER_ONLY_API_INLINE ThreadLocalContextStorage::Stack &ThreadLocalContextStorage::GetStack()
{
  static thread_local Stack stack_ = Stack();
  return stack_;
}
}  // namespace context
OPENTELEMETRY_END_NAMESPACE