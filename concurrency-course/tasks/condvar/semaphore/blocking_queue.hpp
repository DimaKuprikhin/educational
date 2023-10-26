#pragma once

#include "tagged_semaphore.hpp"

#include <deque>

namespace solutions {

// Bounded Blocking Multi-Producer/Multi-Consumer (MPMC) Queue

template <typename T>
class BlockingQueue {
 public:
  explicit BlockingQueue(size_t capacity)
    : putSemaphore(capacity)
    , takeSemaphore(0)
    , dequeSemaphore(1)
    {}

  // Inserts the specified element into this queue,
  // waiting if necessary for space to become available.
  void Put(T value) {
    auto putTakeToken = putSemaphore.Acquire();
    auto dequeToken = dequeSemaphore.Acquire();
    buffer.push_back(std::move(value));
    dequeSemaphore.Release(std::move(dequeToken));
    takeSemaphore.Release(std::move(putTakeToken));
  }

  // Retrieves and removes the head of this queue,
  // waiting if necessary until an element becomes available
  T Take() {
    auto putTakeToken = takeSemaphore.Acquire();
    auto dequeToken = dequeSemaphore.Acquire();
    T result = std::move(buffer.front());
    buffer.pop_front();
    dequeSemaphore.Release(std::move(dequeToken));
    putSemaphore.Release(std::move(putTakeToken));
    return result;
  }

 private:
  std::deque<T> buffer;

  struct PutTakeSemaphoreTag {};
  TaggedSemaphore<PutTakeSemaphoreTag> putSemaphore;
  TaggedSemaphore<PutTakeSemaphoreTag> takeSemaphore;

  TaggedSemaphore<T> dequeSemaphore;
};

}  // namespace solutions
