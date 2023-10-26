#include <exe/coroutine/impl.hpp>

#include <wheels/support/assert.hpp>
#include <wheels/support/compiler.hpp>

namespace exe::coroutine {

CoroutineImpl::CoroutineImpl(Routine routine,
                             wheels::MutableMemView stack)
  : routine(std::move(routine))
  , stack(std::move(stack))
  , isCompleted(false)
{
  routineContext.Setup(stack, this);
}

void CoroutineImpl::Run() {
  try {
    routine();
  } catch(...) {
    exceptionPtr = std::current_exception();
  }
  isCompleted = true;
  routineContext.ExitTo(mainContext);
}

void CoroutineImpl::Resume() {
  mainContext.SwitchTo(routineContext);
  if(exceptionPtr) {
    std::rethrow_exception(exceptionPtr);
  }
}

void CoroutineImpl::Suspend() {
  routineContext.SwitchTo(mainContext);
}

bool CoroutineImpl::IsCompleted() const {
  return isCompleted;
}

}  // namespace exe::coroutine
