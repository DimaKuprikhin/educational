#pragma once

#include <exe/coroutine/impl.hpp>
#include <context/stack.hpp>
#include <twist/util/thread_local.hpp>

#include <optional>

namespace exe::coroutine::generators {

template<typename T>
class Generator;

template<typename T>
static twist::util::ThreadLocalPtr<Generator<T>> currentGenerator;

template <typename T>
class Generator {
 public:
  explicit Generator(Routine routine)
    : coroutineStack(AllocateStack())
    , coroutine(std::move(routine), coroutineStack.View())
  {}

  // Pull
  std::optional<T> Receive() {
    if(coroutine.IsCompleted()) {
      return std::nullopt;
    }
    Generator<T>* prev = currentGenerator<T>.Exchange(this);
    coroutine.Resume();
    currentGenerator<T> = prev;
    if(coroutine.IsCompleted()) {
      return std::nullopt;
    }
    return std::move(value_);
  }

  static void Send(T value) {
    currentGenerator<T>->value_ = std::move(value);
    currentGenerator<T>->coroutine.Suspend();
  }

 private:
  // Intentionally naive and inefficient
  static context::Stack AllocateStack() {
    static const size_t kStackPages = 16;  // 16 * 4KB = 64KB
    return context::Stack::AllocatePages(kStackPages);
  }

 private:
  context::Stack coroutineStack;
  coroutine::CoroutineImpl coroutine;
  std::optional<T> value_;
};

// Shortcut
template <typename T>
void Send(T value) {
  Generator<T>::Send(std::move(value));
}

}  // namespace exe::coroutine::generators
