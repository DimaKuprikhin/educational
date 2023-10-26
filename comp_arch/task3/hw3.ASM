; Программа для вычисления гиперболического косинуса (вариант 14) с помощью
; степенного ряда с точностью не хуже 0.05%.
; cosh(x) = 1 + (x^2 / 2!) + (x^4 / 4!) + ...
; Автор: Куприхин Дима, БПИ199


format PE Console
entry start

include 'win32a.inc'
include 'hw3Macro.inc'

section '.data' data readable writable

        enterStr db 'This program calculates cosh(x). Enter x (-700 <= x <= 700)', 10, 0
        numberStr db '%lf', 0
        debugStr db '%lf', 10, 0
        iterationStr db 'Iteration %d: ', 0
        currentValuesStr db 'current term = %lf, sum = %lf', 10, 0
        resultStr db 'Result: cosh(%lf) = %lf', 10, 0
        ; Вводимый пользователем аргумент вычисляемой функции
        x dq ?
        ; Квадрат введеного значения.
        sqrX dq ?
        ; Переменная, хранящая значение текущего члена ряда.
        term dq 1.0
        ; Переменная, в которой вычисляется значение следущего члена ряда.
        nextTerm dq ?
        ; Счетчик значения факториала.
        factorialCounter dd 1
        ; Счетчик итерации.
        iterationCounter dd 1
        ; Переменная, хранящая сумму ряда.
        sum dq 1.0
        ; Переменная, хранящая точность, с которыми производятся вычисления.
        epsilon dq 0.0005

section '.code' code readable executable

        start:
                ; Вводим аргумент функции.
                invoke printf, enterStr
                invoke scanf, numberStr, x

                FINIT
                ; Считаем x^2, так как каждый следующий член ряда в
                ; числителе увеличивается на x^2
                fld [x]
                fmul [x]
                fstp [sqrX]

        mainLoop:

                ; Выводим промежуточный результат.
                invoke printf, iterationStr,[iterationCounter]
                PrintFloat currentValuesStr, term, sum
                Inc iterationCounter

                ; Вычисляем следующий член:
                ; nextTerm = term; nextTerm *= x^2;
                fld [term]
                fmul [sqrX]
                ;fstp [nextTerm]
                ;PrintFloat debugStr, nextTerm
                ; nextTerm /= factorialCounter++;
                fidiv [factorialCounter]
                Inc factorialCounter
                ; nextTerm /= factorialCounter++;
                fidiv [factorialCounter]
                Inc factorialCounter

                ; Прибавляем к сумме вычисленный член. sum += nextTerm;
                fstp [nextTerm]
                fld [sum]
                fadd [nextTerm]
                fstp [sum]

                ; Проверяем на остановку алгоритма.
                ; Если новый член меньше 0.05% от суммы, то заканчиваем
                ; вычисления и выводим результат.
                fld [sum]
                fmul [epsilon]
                fld [nextTerm]
                fcompp
                fstsw ax
                sahf
                jb outputResult

                ; term = nextTerm;
                CopyFloat term, nextTerm
                jmp mainLoop

        outputResult:
                ; Выводим результат последней итерации и итоговый результат.
                invoke printf, iterationStr,[iterationCounter]
                PrintFloat currentValuesStr, nextTerm, sum
                PrintFloat resultStr, x, sum

        finish:
                push 0
                invoke getch
                call ExitProcess


section '.idata' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess',\
               GetProcessHeap, 'GetProcessHeap',\
               HeapAlloc, 'HeapAlloc'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'
