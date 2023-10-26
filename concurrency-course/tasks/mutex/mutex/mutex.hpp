#pragma once

#include <twist/stdlike/atomic.hpp>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdint>
#include <cassert>

namespace stdlike {

class Mutex {
public:
  void Lock() {
    uint32_t c = 0;
    state.compare_exchange_strong(c, 1);
    if(c != 0) {
      while(true) {
        if(c == 2 || state.compare_exchange_strong(c, 2)) {
          state.FutexWait(2);
        }

        c = 0;
        if(state.compare_exchange_strong(c, 2)) {
          break;
        }
      }
    }
  }
  void Unlock() {
    if(state.fetch_add(-1) != 1) {
      state.store(0);
      state.FutexWakeOne();
    }
  }

#ifdef BAD_MUTEX
  void Lock() {
    uint32_t expected = 0;
    while(!state.compare_exchange_strong(expected, 1)) {
      expected = 0;
      // failed to acquire lock, sleep.
      state.FutexWait(1);
    }
  }
  void Unlock() {
    uint32_t expected = 1;
    state.compare_exchange_strong(expected, 0);
    // unnecessary syscall.
    state.FutexWakeOne();
  }
#endif

 private:
  twist::stdlike::atomic<uint32_t> state{0};
};

}  // namespace stdlike

