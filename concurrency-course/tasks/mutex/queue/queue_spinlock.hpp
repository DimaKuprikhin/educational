#pragma once

#include <twist/stdlike/atomic.hpp>
#include <twist/util/spin_wait.hpp>

namespace spinlocks {

/*  Scalable Queue SpinLock
 *
 *  Usage:
 *
 *  QueueSpinLock qspinlock;
 *  {
 *    QueueSpinLock::Guard guard(qspinlock);  // <-- Acquire
 *    // Critical section
 *  }  // <-- Release
 */

class QueueSpinLock {
 public:
  class Guard {
    friend class QueueSpinLock;

   public:
    explicit Guard(QueueSpinLock& spinlock) : spinlock_(spinlock) {
      spinlock_.Acquire(this);
    }

    ~Guard() {
      spinlock_.Release(this);
    }

   private:
    QueueSpinLock& spinlock_;
    twist::stdlike::atomic<Guard*> next{nullptr};
    twist::stdlike::atomic<uint32_t> owner{0};
  };

 private:
  void Acquire(Guard* guard) {
    twist::stdlike::atomic<Guard*> prev_tail(tail.exchange(guard));
    if(prev_tail.load() == nullptr) {
      return;
    }
    prev_tail.load()->next.store(guard);
    while(guard->owner.load() == 0) {}
  }

  void Release(Guard* owner) {
// #define ATOMIC_OWNER
#ifdef ATOMIC_OWNER
    twist::stdlike::atomic<Guard*> atomic_owner{owner};
    if(atomic_owner.load()->next.load() == nullptr) {
      if(tail.compare_exchange_strong(owner, nullptr)) {
        return;
      }
      while(atomic_owner.load()->next.load() == nullptr) {}
    }
    atomic_owner.load()->next.load()->owner.store(1);
#else
    if(owner->next.load() == nullptr) {
      if(tail.compare_exchange_strong(owner, nullptr)) {
        return;
      }
      while(owner->next.load() == nullptr) {}
    }
    owner->next.load()->owner.store(1);
#endif
  }

 private:
  twist::stdlike::atomic<Guard*> tail{nullptr};
};

}  // namespace spinlocks
