
format PE Console
entry start

include 'win32a.inc'

section '.data' data readable writable

        enterString db 'Enter a size of the array', 10, 0
        enterElemsString db 'Enter the elements separated by a space', 10, 0
        errorString db 'Input is incorrect, the program is finishing', 10, 0
        numberStr db '%d', 0
        spaceStr db ' ', 0
        size dd 0
        sizeInBytes dd 0
        array dd ?
        number dd 0
        maxNumber dd 0xffffffff
        i dd 0

section '.code' code readable executable

        start:
                ; вводим размер массива, больший нул€
                push enterString
                call [printf]
                push size
                push numberStr
                call [scanf]
                cmp eax, 0
                je error
                mov eax, [size]
                cmp eax, 0
                jle start
                ; выдел€ем в куче количество байт, в 4 раза больше введенного
                ; пользователем
                mov [sizeInBytes], eax
                mov eax, 4
                mul [sizeInBytes]
                mov [sizeInBytes], eax
                push [sizeInBytes]
                push 0x00000008
                call [GetProcessHeap]
                push eax
                call [HeapAlloc]
                mov [array], eax

                ; вводим в цикле все элементы массива
                push enterElemsString
                call [printf]
                mov ecx, 0
                mov esi, 0
        inputArrayLoop:
                cmp ecx, [size]
                jge arrayModify
                mov [i], ecx
                push number
                push numberStr
                call [scanf]
                cmp eax, 0
                je error
                mov eax, [array]
                mov ebx, [number]
                mov [eax + 4 * esi], ebx
                mov ecx, [i]
                inc ecx
                inc esi
                jmp inputArrayLoop

                ; ищем максимальное число в массиве
        arrayModify:
                mov ecx, 1
                mov esi, 1
                mov ebx, [eax]
                mov [maxNumber], ebx
        arrayModifyLoop:
                cmp ecx, [size]
                jge outputArray
                mov [i], ecx
                mov ebx, [eax + 4 * esi]
                cmp ebx, [maxNumber]
                jle less
                mov [maxNumber], ebx
        less:   mov ecx, [i]
                inc ecx
                inc esi
                jmp arrayModifyLoop

                ; выводим массив, замен€€ отрицательные элементы
                ; максимальным числом
        outputArray:
                mov ecx, 0
                mov esi, 0
        outputArrayLoop:
                cmp ecx, [size]
                jge finish
                mov [i], ecx
                mov ebx, [eax + 4 * esi]
                cmp ebx, 0
                jge printNumber
                mov ebx, [maxNumber]
        printNumber:
                mov [number], ebx
                push [number]
                push numberStr
                call [printf]
                mov eax, [array]
                push spaceStr
                call [printf]
                mov eax, [array]
                mov ecx, [i]
                inc ecx
                inc esi
                jmp outputArrayLoop

                ; завершаем программу
        finish:
                push 0
                call ExitProcess

        error:  push errorString
                call [printf]
                push 0
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
               scanf, 'scanf'