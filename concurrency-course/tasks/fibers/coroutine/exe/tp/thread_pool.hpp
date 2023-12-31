#pragma once

#include <exe/tp/task.hpp>
#include <exe/tp/blocking_queue.hpp>

#include <twist/stdlike/thread.hpp>

namespace exe::tp {

// Fixed-size pool of worker threads

class ThreadPool {
 public:
  explicit ThreadPool(size_t workers);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Schedules task for execution in one of the worker threads
  // Do not use directly, use tp::Submit instead
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
  ::tp::UnboundedBlockingQueue<Task> queue;

  twist::stdlike::atomic<uint32_t> queue_size{0};
  twist::stdlike::atomic<uint32_t> working_threads{0};
  twist::stdlike::mutex mutex;
  twist::stdlike::condition_variable cv;

};

}  // namespace exe::tp
