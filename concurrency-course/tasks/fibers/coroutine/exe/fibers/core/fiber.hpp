#pragma once

#include <exe/fibers/core/api.hpp>
#include <exe/coroutine/impl.hpp>
#include <context/stack.hpp>
#include <twist/util/thread_local.hpp>

namespace exe::fibers {

// Fiber = Stackful coroutine + Scheduler (Thread pool)

class Fiber {
public:
  Fiber(Scheduler* scheduler, Routine routine, context::Stack stack);

  // ~ System calls
  void Schedule();
  void Yield();

  static Fiber& Self();

private:
  // Task
  void Step();

private:
  Scheduler* scheduler;
  context::Stack coroutineStack;
  coroutine::CoroutineImpl coroutine;
};

}  // namespace exe::fibers
