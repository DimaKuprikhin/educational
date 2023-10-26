#pragma once

#include <exe/coroutine/impl.hpp>
#include <context/stack.hpp>
#include <twist/util/thread_local.hpp>

#include <optional>

namespace exe::coroutine::processors {

template<typename T>
class Processor;

template<typename T>
static twist::util::ThreadLocalPtr<Processor<T>> currentProcessor;

template <typename T>
class Processor {
 public:
  explicit Processor(Routine routine)
    : coroutineStack(AllocateStack())
    , coroutine(std::move(routine), coroutineStack.View())
  {
    ResumeCoroutine();
  }

  void Send(T value) {
    value_ = std::move(value);
    ResumeCoroutine();
  }

  void Close() {
    isClosed = true;
    ResumeCoroutine();
  }

  static std::optional<T> Receive() {
    currentProcessor<T>->coroutine.Suspend();
    if(currentProcessor<T>->isClosed) {
      return std::nullopt;
    }
    return std::move(currentProcessor<T>->value_);
  }

 private:

  void ResumeCoroutine() {
    Processor<T>* prevProcessor = currentProcessor<T>.Exchange(this);
    coroutine.Resume();
    currentProcessor<T> = prevProcessor;
  }

  static context::Stack AllocateStack() {
    static const size_t kStackPages = 16;  // 16 * 4KB = 64KB
    return context::Stack::AllocatePages(kStackPages);
  }

 private:
  bool isClosed = false;
  context::Stack coroutineStack;
  coroutine::CoroutineImpl coroutine;
  std::optional<T> value_;
};

// Shortcut
template <typename T>
std::optional<T> Receive() {
  return Processor<T>::Receive();
}

}  // namespace exe::coroutine::processors
