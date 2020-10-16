format PE console
entry start

include 'win32a.inc'

section '.data' data readable writable
        fmt_string db 'PI with an accuracy %lf%% by the Riemann zeta function: %fl',13, 10, 0

        r_pi dq 0.0  ; (pi^2)/6  - результат дзета-функции Римана
        pi dq 0.0 ; само число Пи, полученное с помощью дзета-функции Римана
        prev_pi dq 0.0 ; предыдущее число Пи, полученное с помощью дзета-функции Римана
        ac dq 0.0 ; точность вычисления Пи
        i dd 1 ; количество итераций цикла
        tmp dq ? ; переменная для хранения впеменных значений
        n_ac dq 0.001 ; точность, не больше которой должно быть число Пи
        six dd 6 ; число 6
        hun dd 100 ; число 100
        stk dd ? ; переменная для хранения стека


PI:
      mov [stk], esp

      lp:
         fld [pi]
         fstp [prev_pi] ; prev_pi=pi

         fild [i]
         fimul [i]
         fstp [tmp] ; tmp=i*i

         fld1
         fdiv [tmp]
         fstp [tmp] ; tmp=1/i*i

         fld [r_pi]
         fadd [tmp]
         fst [r_pi] ; r_pi=r_pi+1/i*i

         fimul [six]
         fsqrt
         fst [pi]; pi= sqrt(r_pi*6)

         fsub [prev_pi]
         fabs
         fstp [ac] ; ac = |pi(i) - pi(i-1)|

         fld [ac]
         fcomp [n_ac] ; |pi(i) - pi(i-1)| > 0.001?

         fstsw ax
         sahf ; перемещаем результат сравнения в регистр

         inc [i] ; i++
      ja lp

      fld [ac]
      fimul [hun]
      fstp [ac] ; ac=ac*100(процентная точность)

      mov esp, [stk]
ret


section '.code' code readable executable
start:
      call PI

      invoke printf, fmt_string, dword[ac], \
                    dword[ac+4], dword[pi], dword[pi+4]

      invoke getch
      invoke ExitProcess, 0


section '.idata' import data readable
        library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll'

        import kernel,\
           ExitProcess, 'ExitProcess'

        include 'api\kernel32.inc'

        import msvcrt,\
           printf, 'printf',\
           getch, '_getch'
