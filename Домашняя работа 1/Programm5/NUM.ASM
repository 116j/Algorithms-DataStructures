format PE GUI
include 'win32wxp.inc'
 
.code
  start:
 
        mov     ecx,    [x]
        mov     eax,    1       ;(edx:eax)=x!
        mov     edx,    0
        .factorial:
                mov     esi,    edx
                mul     ecx
                mov     edi,    eax
                mov     ebx,    edx
                mov     eax,    esi
                mul     ecx
                mov     edx,    eax
                add     edx,    ebx
                mov     eax,    edi
        loop    .factorial
 
        add     eax,    [z]
        adc     edx,    0
 
        div     dword [d]
 
        mul      eax
 
        mov     [y],    eax
 
        cinvoke wsprintf, numstr, '%i', eax
        invoke MessageBox, HWND_DESKTOP, numstr, 'y = ((x! + z) / d)^2', MB_OK
        invoke ExitProcess, 0
 
.data
        numstr  rb      15
        x       dd      35
        z       dd      0
        d       dd      -4686
        y       rd      1233
 
.end start