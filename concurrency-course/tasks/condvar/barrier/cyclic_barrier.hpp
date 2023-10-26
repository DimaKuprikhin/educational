#pragma once

#include <twist/stdlike/mutex.hpp>
#include <twist/stdlike/condition_variable.hpp>

// std::lock_guard, std::unique_lock
#include <mutex>
#include <cstdint>

namespace solutions {

// CyclicBarrier allows a set of threads to all wait for each other
// to reach a common barrier point

// The barrier is called cyclic because
// it can be re-used after the waiting threads are released.

class CyclicBarrier {
 public:
  explicit CyclicBarrier(size_t participants)
    : participants(participants) 
  {}

  // Blocks until all participants have invoked Arrive()
  void Arrive() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex);
    while(!isArriving) {
      cv.wait(lock);
    }

    ++currentParticipants;
    if(currentParticipants == participants) {
      isArriving = false;
      --currentParticipants;
      // in case of participants == 1
      if(currentParticipants == 0) {
        isArriving = true;
        return;
      }
      lock.unlock();
      cv.notify_all();
      return;
    }
    while(isArriving) {
      cv.wait(lock);
    }
    --currentParticipants;
    if(currentParticipants == 0) {
      isArriving = true;
      lock.unlock();
      cv.notify_all();
    }
  }

 private:
  const size_t participants;
  size_t currentParticipants = 0;
  bool isArriving = true;
  twist::stdlike::condition_variable cv;
  twist::stdlike::mutex mutex;
};

}  // namespace solutions
