#include <exe/fibers/core/stacks.hpp>
#include <twist/stdlike/mutex.hpp>
#include <stack>
#include <mutex>
#include <atomic>

using context::Stack;

namespace exe::fibers {

//////////////////////////////////////////////////////////////////////

class StackAllocator {
 public:
  Stack Allocate() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex);
    if(stackPool.size() > 0) {
      Stack result = std::move(stackPool.top());
      stackPool.pop();
      return result;
    }
    lock.unlock();
    return AllocateNewStack();
  }

  void Release(Stack stack) {
    std::lock_guard<twist::stdlike::mutex> lock(mutex);
    stackPool.push(std::move(stack));
  }

 private:
  static Stack AllocateNewStack() {
    static const size_t kStackPages = 16;  // 16 * 4KB = 64KB
    return Stack::AllocatePages(kStackPages);
  }

 private:
  twist::stdlike::mutex mutex;
  std::stack<Stack> stackPool;
};

//////////////////////////////////////////////////////////////////////

StackAllocator allocator;

context::Stack AllocateStack() {
  return allocator.Allocate();
}

void ReleaseStack(context::Stack stack) {
  allocator.Release(std::move(stack));
}

}  // namespace exe::fibers
