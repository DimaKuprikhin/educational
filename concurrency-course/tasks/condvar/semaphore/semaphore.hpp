#pragma once

#include <twist/stdlike/mutex.hpp>
#include <twist/stdlike/condition_variable.hpp>

// std::lock_guard, std::unique_lock
#include <mutex>
#include <cstdint>

namespace solutions {

// A Counting semaphore

// Semaphores are often used to restrict the number of threads
// than can access some (physical or logical) resource

class Semaphore {
 public:
  // Creates a Semaphore with the given number of permits
  explicit Semaphore(size_t initial)
    : tokens(initial)
    {}

  // Acquires a permit from this semaphore,
  // blocking until one is available
  void Acquire() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex);
    while(tokens == 0) {
      cv.wait(lock);
    }
    --tokens;
  }

  // Releases a permit, returning it to the semaphore
  void Release() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex);
    ++tokens;
    lock.unlock();
    cv.notify_one();
  }

 private:
  size_t tokens;
  twist::stdlike::mutex mutex;
  twist::stdlike::condition_variable cv;
};

}  // namespace solutions
