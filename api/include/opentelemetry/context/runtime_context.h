// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/context/context.h"
#include "opentelemetry/export.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace context
{
// The Token object provides is returned when attaching objects to the
// RuntimeContext object and is associated with a context object, and
// can be provided to the RuntimeContext Detach method to remove the
// associated context from the RuntimeContext.
class OTEL_API Token
{
public:
  bool operator==(const Context &other) const noexcept { return context_ == other; }

  ~Token();

private:
  friend class RuntimeContextStorage;

  Token() noexcept;

  // A constructor that sets the token's Context object to the
  // one that was passed in.
  Token(const Context &context);

  const Context context_;
};

/**
 * RuntimeContextStorage is used by RuntimeContext to store Context frames.
 *
 * Custom context management strategies can be implemented by deriving from
 * this class and passing an initialized RuntimeContextStorage object to
 * RuntimeContext::SetRuntimeContextStorage.
 */
class OTEL_API RuntimeContextStorage
{
public:
  /**
   * Return the current context.
   * @return the current context
   */
  virtual Context GetCurrent() noexcept = 0;

  /**
   * Set the current context.
   * @param the new current context
   * @return a token for the new current context. This never returns a nullptr.
   */
  virtual nostd::unique_ptr<Token> Attach(const Context &context) noexcept = 0;

  /**
   * Detach the context related to the given token.
   * @param token a token related to a context
   * @return true if the context could be detached
   */
  virtual bool Detach(Token &token) noexcept = 0;

  virtual ~RuntimeContextStorage();

protected:
  nostd::unique_ptr<Token> CreateToken(const Context &context) noexcept;
};

/**
 * Construct and return the default RuntimeContextStorage
 * @return a ThreadLocalContextStorage
 */
static RuntimeContextStorage *GetDefaultStorage() noexcept;

// Provides a wrapper for propagating the context object globally.
//
// By default, a thread-local runtime context storage is used.
class OTEL_API RuntimeContext
{
public:
  // Return the current context.
  static Context GetCurrent() noexcept;

  // Sets the current 'Context' object. Returns a token
  // that can be used to reset to the previous Context.
  static nostd::unique_ptr<Token> Attach(const Context &context) noexcept;

  // Resets the context to a previous value stored in the
  // passed in token. Returns true if successful, false otherwise
  static bool Detach(Token &token) noexcept;

  // Sets the Key and Value into the passed in context or if a context is not
  // passed in, the RuntimeContext.
  // Should be used to SetValues to the current RuntimeContext, is essentially
  // equivalent to RuntimeContext::GetCurrent().SetValue(key,value). Keep in
  // mind that the current RuntimeContext will not be changed, and the new
  // context will be returned.
  static Context SetValue(nostd::string_view key,
                          const ContextValue &value,
                          Context *context = nullptr) noexcept;

  // Returns the value associated with the passed in key and either the
  // passed in context* or the runtime context if a context is not passed in.
  // Should be used to get values from the current RuntimeContext, is
  // essentially equivalent to RuntimeContext::GetCurrent().GetValue(key).
  static ContextValue GetValue(nostd::string_view key, Context *context = nullptr) noexcept;

  /**
   * Provide a custom runtime context storage.
   *
   * This provides a possibility to override the default thread-local runtime
   * context storage. This has to be set before any spans are created by the
   * application, otherwise the behavior is undefined.
   *
   * @param storage a custom runtime context storage
   */
  static void SetRuntimeContextStorage(nostd::shared_ptr<RuntimeContextStorage> storage) noexcept;

private:
  static nostd::shared_ptr<RuntimeContextStorage> GetRuntimeContextStorage() noexcept;

  static nostd::shared_ptr<RuntimeContextStorage> &GetStorage() noexcept;
};

// The ThreadLocalContextStorage class is a derived class from
// RuntimeContextStorage and provides a wrapper for propagating context through
// cpp thread locally. This file must be included to use the RuntimeContext
// class if another implementation has not been registered.
class OTEL_API ThreadLocalContextStorage : public RuntimeContextStorage
{
public:
  ThreadLocalContextStorage() noexcept;

  // Return the current context.
  Context GetCurrent() noexcept override;

  // Resets the context to the value previous to the passed in token. This will
  // also detach all child contexts of the passed in token.
  // Returns true if successful, false otherwise.
  bool Detach(Token &token) noexcept override;

  // Sets the current 'Context' object. Returns a token
  // that can be used to reset to the previous Context.
  nostd::unique_ptr<Token> Attach(const Context &context) noexcept override;

private:
  // A nested class to store the attached contexts in a stack.
  class Stack
  {
    friend class ThreadLocalContextStorage;

    Stack() noexcept;

    // Pops the top Context off the stack.
    void Pop() noexcept;

    bool Contains(const Token &token) const noexcept;

    // Returns the Context at the top of the stack.
    Context Top() const noexcept;

    // Pushes the passed in context pointer to the top of the stack
    // and resizes if necessary.
    void Push(const Context &context) noexcept;

    // Reallocates the storage array to the pass in new capacity size.
    void Resize(size_t new_capacity) noexcept;

    ~Stack() noexcept;

    size_t size_;
    size_t capacity_;
    Context *base_;
  };

  Stack &GetStack();
};
}  // namespace context
OPENTELEMETRY_END_NAMESPACE

#ifdef OTEL_WITH_HEADER_ONLY_API
#  include "opentelemetry/context/runtime_context.cc"
#endif  // OTEL_WITH_HEADER_ONLY_API
