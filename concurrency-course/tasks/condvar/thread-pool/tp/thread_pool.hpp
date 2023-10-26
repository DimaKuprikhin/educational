#pragma once

#include <tp/blocking_queue.hpp>
#include <tp/task.hpp>

#include <twist/stdlike/thread.hpp>
#include <twist/stdlike/condition_variable.hpp>
#include <twist/stdlike/mutex.hpp>
#include <twist/stdlike/atomic.hpp>

namespace tp {

// Fixed-size pool of worker threads

class ThreadPool {
public:
  explicit ThreadPool(size_t workers);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Schedules task for execution in one of the worker threads
  void Submit(Task task);

  // Waits until outstanding work count has reached zero
  void WaitIdle();

  // Stops the worker threads as soon as possible
  // Pending tasks will be discarded
  void Stop();

  // Locates current thread pool from worker thread
  static ThreadPool* Current();

private:
  std::vector<twist::stdlike::thread> threads;
  UnboundedBlockingQueue<Task> queue;

  twist::stdlike::atomic<uint32_t> queue_size{0};
  twist::stdlike::atomic<uint32_t> working_threads{0};
  twist::stdlike::mutex mutex;
  twist::stdlike::condition_variable cv;

};

inline ThreadPool* Current() {
  return ThreadPool::Current();
}

}  // namespace tp
