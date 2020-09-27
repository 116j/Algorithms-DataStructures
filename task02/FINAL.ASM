; Домашнее задание 2. Вариант 9
; Программа создает массив А и считывает его элементы из консоли.
; Затем из нечетных элементов массива А создается массив В и выводится в консоли.      

format PE Console

include 'win32a.inc'

entry start

section '.data' data readable writable
        strN db 'Enter size of array (1 - 100): ',0
        strEl db 'Enter array A of size %d: ',0
        strRes db 'Array B of odd numbers: ',0
        strErrorG db 'Size cant be greater than 100',0
        strErrorL db 'Size cant be less than 1',0

        inputeStr db ' %d',0
        emptyStr db '%d',0
        outputStr db '%d ',0

        sizeA dd ?
        sizeB dd ?
        A rb 100
        B rb 100
        el dd ?
        tmp dd ?

        NULL=0

; Cчитывает размер массива и проверяет его корректность 
input_size:
           mov [tmp], esp

           push strN
           call [printf]

           push sizeA
           push emptyStr
           call [scanf]

           ; если больше 100 или меньше 1 - переходит к метке ошибки   
           cmp [sizeA], 100
           jg errorG

           cmp [sizeA], 1
           jl errorL

           mov esp, [tmp]
ret

; Считывает в цикле элементы массива А 
input_A:
        mov [tmp], esp

        push [sizeA]
        push strEl
        call [printf]

        mov ebx,0
        lp_A:

             push el
             push inputeStr
             call [scanf]

             mov eax, [el]
             mov edx, A
             mov [edx+ebx*4], eax

             inc ebx
             cmp ebx, [sizeA]
        jne lp_A

        mov esp, [tmp]
ret

; Создает массив В из нечетных элементов массива А 
create_B:
         mov [tmp], esp

         mov ebx,0
         mov ecx,0
         lp_A_B:
                mov eax, A
                mov edx,[eax+ebx*4]
                test edx,1
                jne Odd
                    inc ebx
                    cmp ebx, [sizeA]
                jne lp_A_B
                je F
                Odd:
                    mov eax,B
                    mov [eax+ecx*4], edx

                    inc ebx
                    inc ecx
                    cmp ebx, [sizeA]
                jne lp_A_B
                je F

         F:
         mov [sizeB], ecx

         mov esp, [tmp]
ret

; Вывод массива В  
output:
       mov [tmp], esp

       mov ebx,0

       ; если маасив пуст, то для избежания незаполненной памяти переходим к завершению программы  
       push strRes
       call [printf]

       cmp [sizeB], 0
       je finish

       lp_B:
            mov eax, B
            mov edx,[eax+ebx*4]

            push edx
            push outputStr
            call [printf]

            inc ebx
            cmp ebx, [sizeB]
       jne lp_B

       mov esp, [tmp]
ret

section '.code' code readable executable
        start:
                call input_size
                call input_A
                call create_B
                call output
                jmp finish

                ; обработка некорректности размера массива А  
                errorG:
                      push strErrorG
                      call [printf]

                      jmp finish

                errorL:
                      push strErrorL
                      call [printf]
                finish:
                      call [getch]

                      push NULL
                      call [ExitProcess]


section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'
