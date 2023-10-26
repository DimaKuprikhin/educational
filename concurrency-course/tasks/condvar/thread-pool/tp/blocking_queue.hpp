#pragma once

#include <twist/stdlike/mutex.hpp>
#include <twist/stdlike/condition_variable.hpp>
#include <twist/stdlike/atomic.hpp>

#include <optional>
#include <list>
#include <mutex>

namespace tp {

// Unbounded blocking multi-producers/multi-consumers queue

template <typename T>
class UnboundedBlockingQueue {
 public:
  bool Put(T value) {
    std::lock_guard<twist::stdlike::mutex> lock(mutex);
    if(closed.load() == 1) {
      return false;
    }
    data.push_back(std::move(value));
    cv.notify_one();
    return true;
  }

  std::optional<T> Take() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex);
    while(data.size() == 0) {
      if(closed.load() == 1) {
        return std::nullopt;
      }
      cv.wait(lock);
    }
    T value = std::move(data.front());
    data.pop_front();
    return std::move(value);
  }

  void Close() {
    CloseImpl(/*clear=*/false);
  }

  void Cancel() {
    CloseImpl(/*clear=*/true);
  }

 private:
  void CloseImpl(bool clear) {
    std::lock_guard<twist::stdlike::mutex> lock(mutex);
    closed.store(1);
    if(clear) {
      data.clear();
    }
    cv.notify_all();
  }

 private:
  std::list<T> data;
  twist::stdlike::mutex mutex;
  twist::stdlike::condition_variable cv;
  twist::stdlike::atomic<uint32_t> closed{0};

};

}  // namespace tp
