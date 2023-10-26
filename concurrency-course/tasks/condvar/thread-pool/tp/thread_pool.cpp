#include <tp/thread_pool.hpp>

#include <twist/util/thread_local.hpp>
#include <twist/test/test.hpp>

#include <mutex>

namespace tp {

////////////////////////////////////////////////////////////////////////////////

static twist::util::ThreadLocalPtr<ThreadPool> pool;

////////////////////////////////////////////////////////////////////////////////

ThreadPool::ThreadPool(size_t workers) {
  for(size_t i = 0; i < workers; ++i) {
    threads.emplace_back([this]() {
      pool = this;
      while(true) {
        auto opt = queue.Take();
        if(opt) {
          try {
            opt.value()();
          }
          catch(...) {}
          std::lock_guard<twist::stdlike::mutex> lock(mutex);
          queue_size.fetch_add(-1);
          cv.notify_all();
        }
        else {
          std::lock_guard<twist::stdlike::mutex> lock(mutex);
          working_threads.fetch_add(-1);
          cv.notify_all();
          return;
        }
      }
    });
    working_threads.fetch_add(1);
  }
}

ThreadPool::~ThreadPool() {
  ASSERT_TRUE(working_threads.load() == 0);
}

void ThreadPool::Submit(Task task) {
  std::lock_guard<twist::stdlike::mutex> lock(mutex);
  queue.Put(std::move(task));
  queue_size.fetch_add(1);
}

void ThreadPool::WaitIdle() {
  std::unique_lock<twist::stdlike::mutex> lock(mutex);
  while(queue_size.load() != 0) {
    cv.wait(lock);
  }
}

void ThreadPool::Stop() {
  queue.Cancel();
  std::unique_lock<twist::stdlike::mutex> lock(mutex);
  while(working_threads.load() != 0) {
    cv.wait(lock);
  }
  for(auto& thread : threads) {
    thread.join();
  }
}

ThreadPool* ThreadPool::Current() {
  return pool;
}

}  // namespace tp
