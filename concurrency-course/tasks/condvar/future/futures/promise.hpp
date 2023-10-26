#pragma once

#include <futures/future.hpp>
#include <futures/shared_state.hpp>

#include <memory>

namespace stdlike {

template <typename T>
class Promise {
public:
  Promise()
    : state(std::make_shared<SharedState<T>>())
  {}

  // Non-copyable
  Promise(const Promise&) = delete;
  Promise& operator=(const Promise&) = delete;

  // Movable
  Promise(Promise&&) = default;
  Promise& operator=(Promise&&) = default;

  // One-shot
  Future<T> MakeFuture() {
    Future<T> future(state);
    return std::move(future);
  }

  // One-shot
  // Fulfill promise with value
  void SetValue(T&& value) {
    state->SetValue(std::move(value));
  }

  // One-shot
  // Fulfill promise with exception
  void SetException(std::exception_ptr ex) {
    state->SetException(std::move(ex));
  }

private:
  std::shared_ptr<SharedState<T>> state;

};

}  // namespace stdlike
