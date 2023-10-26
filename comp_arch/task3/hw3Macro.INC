; Макрос для вывода чисел с плавующей запятой.
macro PrintFloat string, [args] {
 reverse
  push dword[args + 4]
  push dword[args]
 common
  push string
  call [printf]
}

; Макрос, инкрементирующий значение переменной.
macro Inc value {
 mov ecx, [value]
 inc ecx
 mov [value], ecx
}

; Макрос, копирующий значение одного числа с плавующей запятой в другое.
macro CopyFloat to, from {
 mov eax, dword[from]
 mov ebx, dword[from + 4]
 mov dword[to], eax
 mov dword[to + 4], ebx
}
