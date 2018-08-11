

.data
realVal REAL8 +1.5		; this stores a real number in 8 bytes
Temprax qword 0
Temprbx qword 0
Temprcx qword 0
Temprdx qword 0
TempR8  qword 0
TempR9  qword 0

.code
PUBLIC CombineA
CombineA PROC
ADD	ECX, DWORD PTR [RSP+28H]		;ECX par 1 (a) a int and add from the stack this is the no 5 par (e) a int
ADD ECX, R9D						;add to ECX the dword at R9d this is the 4 par (d) a int
ADD ECX, R8D						;add to ECX the dword at R8d this is the 3 par (c) a int
ADD ECX, EDX						;add to ECX the dword at EDX this is the 2 par (b) a int
MOVD XMM0, ECX						;now move this addtion into XMM0 up this point all ints
CVTDQ2PD XMM0, XMM0					;convert packed dwords integers to packed Double precision Floats
MOVSD XMM1, realVal					;move DoubleWord(real8)realVal set to +1.5 to XMM1
ADDSD XMM1, MMWORD PTR [RSP+30H]	;add to XMM1 the MMWORD PTR at [RSP+30h] this C++ double (f) a double
DIVSD  XMM0, XMM1					;now div XMM0 with XMM1 storing the result into XMM0 this the value back to C
RET									;DIVSD divide Scalar Double Precision Floating point Value
CombineA ENDP

Public FunCpuid
FunCpuid proc
mov Temprax,rax						;copy values on entry to Temp*** Eax,Ebx,Ecx,Edx also R9d and R8d
mov Temprbx,rbx
mov Temprcx,rcx						;this is par 1
mov Temprdx,rdx						;this is par 2
mov TempR8,r8						;this is par 3
mov TempR9,r9						;this is par 4 (the rest will be on the stack if more than 4) 
lea r10,Temprax
mov rax,1234h
ret
FunCpuid endp
End