#pragma once

#include <memory>
#include <cassert>
#include <futures/shared_state.hpp>

namespace stdlike {

using namespace detail;

template <typename T>
class Future {
  template <typename U>
  friend class Promise;

public:
  // Non-copyable
  Future(const Future&) = delete;
  Future& operator=(const Future&) = delete;

  // Movable
  Future(Future&&) = default;
  Future& operator=(Future&&) = default;

  // One-shot
  // Wait for result (value or exception)
  T Get() {
    return std::move(state->Get());
  }

private:
  Future(std::shared_ptr<SharedState<T>> state)
    : state(std::move(state))
  {}

private:
  std::shared_ptr<SharedState<T>> state;

};

}  // namespace stdlike
