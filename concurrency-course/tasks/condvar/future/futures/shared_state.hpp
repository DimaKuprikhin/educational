#pragma once

#include <twist/stdlike/atomic.hpp>

#include <variant>
#include <exception>

namespace stdlike::detail {

template<typename T>
class SharedState {
public:
  void SetValue(T&& value_) {
    value = std::move(value_);
    Notify();
  }

  void SetException(std::exception_ptr pointer) {
    ExceptionPointer wrapped_pointer(pointer);
    value = wrapped_pointer;
    Notify();
  }

  T Get() {
    while(state.load() == 0) {
      state.FutexWait(0);
    }
    if(std::holds_alternative<ExceptionPointer>(value)) {
      std::rethrow_exception(std::get<ExceptionPointer>(value).pointer);
    }
    return std::move(std::get<T>(value));
  }

private:
  void Notify() {
    state.store(1);
    state.FutexWakeOne();
  }

private:
  struct ExceptionPointer {
    std::exception_ptr pointer;

    ExceptionPointer() : pointer(nullptr) {}
    ExceptionPointer(std::exception_ptr ptr) : pointer(ptr) {}
  };

  std::variant<ExceptionPointer, T> value;
  twist::stdlike::atomic<uint32_t> state{0};

};

} // namespace stdlike::detail
