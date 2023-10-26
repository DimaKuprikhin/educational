#if(APPLE)
  #define FUNCTION_NAME(name) _##name
#else
  #define FUNCTION_NAME(name) name
#endif

.globl FUNCTION_NAME(AtomicLoad)
.globl FUNCTION_NAME(AtomicStore)
.globl FUNCTION_NAME(AtomicExchange)

# Solution starts here

FUNCTION_NAME(AtomicLoad):
# %rdi - cell*
#    call _ZStanSt12memory_orderSt23__memory_order_modifier
    movq (%rdi), %rax
    retq

FUNCTION_NAME(AtomicStore):
# %rdi - cell*, %rsi - value
#	  call _ZStanSt12memory_orderSt23__memory_order_modifier
    movq %rsi, (%rdi)
#   mfence
    retq

FUNCTION_NAME(AtomicExchange):
# %rdi - cell*, %rsi - value
    movq %rsi, %rax
    xchgq (%rdi), %rax
#    movq (%rdi), %rax  # Load
#    movq %rsi, (%rdi)  # Store
    retq
