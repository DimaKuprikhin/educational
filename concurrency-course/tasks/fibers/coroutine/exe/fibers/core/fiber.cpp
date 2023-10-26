#include <exe/fibers/core/fiber.hpp>
#include <exe/fibers/core/stacks.hpp>

#include <twist/util/thread_local.hpp>

namespace exe::fibers {

static twist::util::ThreadLocalPtr<Fiber> currentFiberPtr;

//////////////////////////////////////////////////////////////////////

Fiber::Fiber(Scheduler* scheduler, Routine routine, context::Stack stack)
  : scheduler(scheduler)
  , coroutineStack(std::move(stack))
  , coroutine(std::move(routine), coroutineStack.View())
  {}

void Fiber::Schedule() {
  scheduler->Submit([this]() {
    currentFiberPtr = this;
    Step();
    if(currentFiberPtr != nullptr) {
      Schedule();
    }
  });
}

void Fiber::Yield() {
  coroutine.Suspend();
}

void Fiber::Step() {
  coroutine.Resume();
  if(coroutine.IsCompleted()) {
    ReleaseStack(std::move(coroutineStack));
    delete this;
    currentFiberPtr = nullptr;
  }
}

Fiber& Fiber::Self() {
  return *currentFiberPtr;
}

//////////////////////////////////////////////////////////////////////

// API Implementation

void Go(Scheduler& scheduler, Routine routine) {
  Fiber* fiber = new Fiber(&scheduler, std::move(routine), AllocateStack());
  fiber->Schedule();
}

void Go(Routine sroutine) {
  Go(*tp::ThreadPool::Current(), std::move(sroutine));
}

namespace self {

void Yield() {
  Fiber::Self().Yield();
}

}  // namespace self

}  // namespace exe::fibers
