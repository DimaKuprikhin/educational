#pragma once

#include <twist/stdlike/atomic.hpp>

#include <cstdint>
#include <algorithm>

namespace stdlike {

class CondVar {
 public:
  // Mutex - BasicLockable
  // https://en.cppreference.com/w/cpp/named_req/BasicLockable
  template <class Mutex>
  void Wait(Mutex& mutex) {
    const uint32_t wait = wait_counter.fetch_add(1);
    uint32_t local_wake_counter;
    while((local_wake_counter = wake_counter.load()) <= wait) {
      mutex.unlock();
      wake_counter.FutexWait(local_wake_counter);
      mutex.lock();
    }
  }

  void NotifyOne() {
    uint32_t local_wake_counter = wake_counter.load();
    while(!wake_counter.compare_exchange_strong(
                          local_wake_counter,
                          std::min(wait_counter.load(), local_wake_counter + 1)))
    {
      local_wake_counter = wake_counter.load();
    }
    wake_counter.FutexWakeOne();
  }

  void NotifyAll() {
    wake_counter.store(wait_counter.load());
    wake_counter.FutexWakeAll();
  }

 private:
  twist::stdlike::atomic<uint32_t> wait_counter{0};
  twist::stdlike::atomic<uint32_t> wake_counter{0};
};

}  // namespace stdlike
