extern Showcpu:proc				;tell masm that there is a external proc
extern CreateThread:proc
extern malloc:proc
extern free:proc
extern printf:proc
include AsmStructures.inc

.data
realVal real8 +1.5				; this stores a real number in 8 bytes
RealAdd real8 +21.1
real_hd		real8 +1560.34561
real_ld     real8 +6430.7678
real4_a     real4 +12.5
real4_b     real4 +208.6
real4_c     real4 -1204.5
real4_d     real4 +2082.65
bytes16     db 16 dup(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)
avg16		db 16 dup(10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160)
;*************************** used for temp storing the regs **************

Temprax		qword 0
Temprbx		qword 0
Temprcx		qword 0
Temprdx		qword 0
TempR8		qword 0
TempR9		qword 0
TempR10		qword 0
TempR11		qword 0
TempR12		qword 0
TempR13		qword 0
TempR14		qword 0
TempR15		qword 0
Tempeax		dword 0
Tempebx		dword 0
Tempecx		dword 0
Tempedx		dword 0
TSstart     qword 0
TSend       qword 0
TSdiff      qword 0
;****************************************************************************
mes1		db 'This Hello is From mes1 ASM Brian',0
mes2		db 'This Hello Is From mes2 Brian %8.6f and %8.2f',0dh,0ah,0
MaxString   db 40h
EntryEax	dword 0
align(16)
myinxmm0	real8 0.0
myinxmm1    real8 0.0
myMemory    qword 0
align 16
oldxmm0		real8 ?
oldxmm1		real8 ?
amountcpu	dword 0
amountex	dword 0
mutex		dw    0
xmmred      xmmword ?
cpulist     CpuData{}


;********************************************************************************
;CombineC(1, 2, 3, 4, 5, 6.1)) rcx = a,rdx = b, r8 = c, r9 = d, stack = e, stack = f 
;using the RBP to access the stack 
.code
Public CombineA
CombineA proc
call storeall
push rbp						;save the caller bp on the stack rsp now rsp - 8
mov rbp,rsp						;now set the rbp to our rsp points to rbp on the stack
								;stack = RBP(8):Return Address(10h):Shadow space(30h):e:f
push r15						;this only effects the rsp NOT the rbp so the the above line correct
lea r14,localdata
add	ecx,dword ptr [rbp+30h]		;ECX par 1 (a) a int so add from the stack no 5 par (e) a int
add ecx,r9d						;add to ECX the dword at R9d this is the 4 par (d) a int
add ecx,r8d						;add to ECX the dword at R8d this is the 3 par (c) a int
add ecx,edx						;add to ECX the dword at EDX this is the 2 par (b) a int
movd xmm0,ecx					;now move this addtion into XMM0 up this point all ints
cvtdq2pd xmm0,xmm0				;convert packed dwords integers to packed Double precision Floats
movlpd xmm1,realVal				;move DoubleWord(real8)realVal set to +1.5 to XMM1
addsd xmm1,qword ptr [rbp+38h]	;add to XMM1 the MMWORD PTR at [RSP+30h] this C++ double (f) a double
divsd xmm0,xmm1					;now div XMM0 with XMM1 storing the result into XMM0 this the value back to C
call restoreall
pop r15							;restore r15 correct stack
pop rbp							;restore the caller's rbp
ret								;DIVSD divide Scalar Double Precision Floating point Value
localdata qword 012345678h
CombineA ENDP
;*************************************************************************************
Public FunCpuid
FunCpuid proc
call storeall
mov eax,dword ptr temprcx			;Temprcx is par 1 (in this case was the eax value for cpuid ins)
cpuid								;do couid this will change regs eax,ebx,ecx and edx
mov tempeax,eax						;store the eax that cpuid ins gave us in (dword)tempeax 
mov tempebx,ebx						;store the ebx that cpuid ins gave us in (dword)tempebx 
mov tempecx,ecx						;store the ecx that cpuid ins gave us in (dword)tempecx 
mov tempedx,edx						;store the edx that cpuid ins gave us in (dword)tempedx 
mov ecx,tempeax						;now set ecx to par 1 
mov edx,tempebx						;now set edx to par 2 
mov r8d,tempecx						;now set r8d to par 3 
mov r9d,tempedx						;now set r9d to par 4 
sub rsp,20h							;setup shadow space for 4 Qwords 8 * 4 = 32 = 20h
call Showcpu						;now call the extern proc
add rsp,20h							;restore stack pointer
call restoreall
ret
FunCpuid endp
;****************************************************************************
Public FuncPassPtr
FuncPassPtr proc
call storeall
mov rbx,Temprcx						;this is a Qword ptr from C++ to First par (regs[0] )
mov eax,1							;send 1 to regs[0] eax = 1
mov [rbx],eax						;now set regs[0] eax
add rbx,4							;mov ptr 4 bytes size of int
inc eax								;eax = eax + 1 eax = 2
mov [rbx],eax						;now send 2 to regs[1]
add rbx,4							;mov ptr 4 bytes size of int
inc eax								;eax = eax + 1 eax = 3
mov [rbx],eax						;now send 3 to regs[2]
add rbx,4							;mov ptr 4 bytes size of int
mov eax,1234h						;eax = eax + 1 eax = 4
mov [rbx],eax						;now send 4 to regs[3]
call restoreall
ret
FuncPassPtr endp
;*****************************************************************************
Public FuncCpuPtr
FuncCpuPtr proc
call storeall
mov r10,Temprcx						;this is a Qword ptr from C++ to First par (regs[0] ) r10 points here
mov eax,[r10]						;regs[0] is used to set eax before cpuid
mov EntryEax,eax					;value used for eax before cpuid
cpuid
mov [r10],eax						;eax now set from Cpuid send to regs[0]
mov [r10 + 4],ebx					;ebx now set from Cpuid send to regs[1]
mov [r10 + 8],ecx					;ecx now set from Cpuid send to regs[2]
mov [r10 + 12],edx					;edx now set from Cpuid send to regs[3]
call restoreall
mov eax,EntryEax					;return eax that was sent to the cpuid
ret
FuncCpuPtr endp
;**********************************************************************************
;enters with two doubles one in xmm0 first One(d3) and second in xmm1 (d1)
;returns (d2)in xmm0
public FuncXmm
FuncXmm proc
call storeall
minsd xmm0,xmm1						;xmm0 countains par 1 (d3) and xmm1 contains par 2 (d1) xmm0 now set to minimum
lea rbx,oldxmm0						;get address of oldmm into the rbx note define as xmmword ?
movdqa[rbx],xmm0					;mov the xmm0 into the xmmword this must be align 16 bytes see data section
push r9								;save r9 because malloc trashs the xmm0 and r9 maybe others
sub rsp,20h							;using 1 par rcx
mov rcx,1024						;rcx = amount of bytes to ask malloc for
call malloc							;destroys the xmm0 register?
pop r9								;restore R9
mov myMemory,rax					;rax = 64 bit pointer to it
add rsp,20h							;restore rsp
mov rbx,qword ptr myMemory			;place into the rbx the pointer given to us by malloc
mov rcx,128							;rcx counter place 1024 / 8 because there is 8 bytes in rax
mov rax,'BABABABA'					;place it our menory "BABABA"
Myloop:								;start of loop
mov [rbx],rax						;place the contends of the rax into [rbp]
add rbx,8							;move rbx along to next set of 8 bytes
loop Myloop							;dex rxc counter and if is not zero go round loop untill it is
sub rsp,20h							;we are about to call free with 1 par so making sure stack and place set
mov rcx,myMemory					;rcx is what free needs to free the memory given by malloc
call free							;now do it
add rsp,20h							;restore the rsp back
lea rbx,oldxmm0						;set the rbx to point to the xmm pointer to oldmm where xmm0 was stored
movdqa xmm0,[rbx]					;restore the old value of the xmm0 register before calling malloc/free
call restoreall
ret
FuncXmm endp

;**********************************************************************************
;enters with two sturtures one for cpu standard and one for extended
;returns 0 = false OR 1 = true
public FuncCpuAll
FuncCpuAll proc
call storeall
mov rax,0
cpuid
mov amountcpu,eax
mov eax,80000000h
cpuid
sub eax,80000000h
mov amountex,eax
mov Tempeax,eax
mov Tempebx,ebx
mov Tempecx,ecx
mov Tempedx,edx
mov rax,0
mov R10,Temprcx
mov R11,0
mov rcx,0
mov ecx,dword ptr amountcpu
inc ecx
Myloop:
push rcx
push rax
push r11
cpuid
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpueax,eax
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuebx,ebx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuecx,ecx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuedx,edx
pop r11
pop rax
pop rcx
add r11,16
inc rax
loopnz Myloop
mov rax,80000000h
mov R10,Temprdx
mov R11,0
mov rcx,0
mov ecx,dword ptr amountex
inc ecx
Myloop2:
push rcx
push rax
push r11
cpuid
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpueax,eax
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuebx,ebx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuecx,ecx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuedx,edx
pop r11
pop rax
pop rcx
add r11,16
inc rax
loopnz Myloop2
call restoreall
mov rax,1							;true
ret
FuncCpuAll endp
;**********************************************************************************
;********************** FuncMemory *************************************************
;enters with The Address Of Memory Allocated From C++ With New Array Of 1024 Dword's
;C++ will also delete[] this memory;also using printf with double float and single float
public FuncMemory
FuncMemory proc
call storeall
push rsi
push rdi
lea rsi,mes1
mov rdi,Temprcx						;Address Of Memory From C++ (1024 DWORDs)
lea rbx,mes1
call GetStrLen						;on entry rbx points to string on exit rcx = lenght of string
MyMemloop:
mov al,byte ptr[rsi]
cmp al,0
je Loopstop
mov byte ptr[rdi],al
inc rsi
inc rdi
loopnz MyMemloop
loopstop:
mov byte ptr[rdi],al
pop rdi
pop rsi
sub rsp,38h							;shadow (32): + rcx (8): + Rdx  (8): + R8 (8) = 56 = 38h
lea rcx,mes2						;xmm0,xmm1 are returned in their registers no space needed on stack
movq xmm0,qword ptr[real_ld]		;displays dp float(real_ld) and sp float (real4_c)
movq rdx,xmm0
cvtss2sd xmm1,dword ptr[real4_d]	;now convert it to a dp float using floats (dp) in xmm0 and xmm1
movq r8,xmm1						;place the integer into next reg (Rcx,Rdx,R8,R9)
call printf
xor eax,eax
add rsp,38h
call restoreall
ret
FuncMemory endp
;**********************************************************************************
;returns a structure of CpuData type enters with ints 1,2,3,4
public Funcretinfo					;note caps are mixed only to show not needed
FuncRetInfo proc
call storeall
cmp rcx,1
je CpuNormal
mov rax,80000000h
cpuid
sub eax,080000000h
mov dword ptr amountex,eax
lea r10,cpulist
mov rcx,0
mov ecx,dword ptr amountex
inc ecx
mov rax,0
mov eax,080000000h
Myloop2:
push rcx
push rax
cpuid
mov dword ptr [r10].CpuData[0].Cpu.Cpueax,eax
mov dword ptr [r10].CpuData[0].Cpu.Cpuebx,ebx
mov dword ptr [r10].CpuData[0].Cpu.Cpuecx,ecx
mov dword ptr [r10].CpuData[0].Cpu.Cpuedx,edx
pop rax
pop rcx
add r10,16
inc rax
loopnz Myloop2
jmp alldone_ext
CpuNormal:
lea r10,cpulist
mov eax,0
cpuid
mov amountcpu,eax

mov rax,0
mov rcx,0
mov ecx,dword ptr amountcpu
inc ecx
Myloop:
push rcx
push rax
cpuid
mov dword ptr [r10].CpuData[0].Cpu.Cpueax,eax
mov dword ptr [r10].CpuData[0].Cpu.Cpuebx,ebx
mov dword ptr [r10].CpuData[0].Cpu.Cpuecx,ecx
mov dword ptr [r10].CpuData[0].Cpu.Cpuedx,edx
pop rax
pop rcx
add r10,16
inc rax
loopnz Myloop				
call restoreall
lea rax,cpulist
mov ecx,dword ptr amountcpu
mov dword ptr[rdx], ecx
ret 
alldone_ext:
call restoreall
lea rax,cpulist
mov ecx,dword ptr amountex
mov dword ptr[rdx],ecx
ret 
Funcretinfo endp


;**********************************************************************************
;test function to test x87 and sse instruction return double precision float
;accepts XMM0 as first par and XMM1 as second
FuncSums proc
call StoreAll
movd real_hd,xmm0
movhpd xmm4,real_hd
movd real_ld,xmm1
movlpd xmm4,real_ld
haddpd xmm4,xmm4
movdqa xmm0,xmm4

call RestoreAll

ret
FuncSums endp
;**********************************************************************************
;Creating a Thread in ASM 
public FuncThreadStart
FuncThreadStart proc
call StoreAll
mov rax,5000000
lea rdx,mutex
spinloop:
lock bts mutex,0				;this will inc mutex(Was 0) the carry is not set because note the lock
jc spinloop						;(All othet threads will be blocked untill its completes) it only went for 0 to 1 (No carry)
loophead:						;other threads will spinlock here untill first thread completes

inc dword ptr[rcx]
dec rax
jnz loophead
call RestoreAll
mov mutex,0						;allow other threads to their work because mutex = zero so lock bts mutex,0 = No carry
ret
FuncThreadStart endp
;**********************************************************************************
;The Func that call win CreateThread **********************************************
public CreateThreadInAsm
CreateThreadInAsm proc
call StoreAll
push rbp						;save callers rbp then set our rbp to the rsp
mov rbp,rsp						;CreateThread(Security,StackSize,Func Address,Pars for Func,Creation,ThreadID)
push 0							;On Stack 6th par for CreateThread threadid = None
push 0							;On Stack 5th par for CreateThread 0 = Creation Flag 
sub rsp,20h
mov r9,rcx						;R9 is Function(par) 4th par address in R9 
mov rcx,0						;RCX is par 1 Security attributes
mov rdx,0						;RDX is par 2th StackSize 
mov r8,FuncThreadStart			;R8 is par 3th par Function address
call CreateThread
mov rsp,rbp						;resets the rsp back to where it should be
pop rbp							;restore the rbp that was entered with
push rax						;rax was the thread's handle					
call RestoreAll
pop rax							;return the thread handle to be used with WaitFor*** Object 
ret
CreateThreadInAsm endp

;**********************************************************************************
;test function to test x87 and sse instruction return double precision float
;accepts XMM0 as first par and XMM1 as second
FuncFloats proc
call StoreAll
movups xmm3,xmmword ptr[rcx]
movups xmm4,xmmword ptr[rdx]
addps xmm3,xmm4
movups xmmword ptr[r8],xmm3
insertps xmm6,real4_a,00000000b
insertps xmm6,real4_b,00010000b
insertps xmm6,real4_c,00100000b
insertps xmm6,real4_d,00110000b
lea r10,bytes16							;or the quick way same result 
movdqu xmm1,[r10]
lea r10,avg16	
movdqu xmm0,[r10]
pavgb xmm0,xmm1
call RestoreAll
ret
FuncFloats endp
;************* The Followig Subs are used in the asm Func's ***********************
;copy values on entry from Registers to TempR*x also R8,9,10,11,12,13,14,15
public GetStrLen
GetStrLen proc
mov rcx,0								;maxium string len
loop1:
mov al,byte ptr[rbx]
cmp al,0
je loopok
inc rcx
inc rbx
cmp cl,MaxString
jb loop1
mov byte ptr[rbx],0
ret
loopok:
inc rcx
ret
GetStrLen endp

;********************************** StoreAll **************************************
;**********************************************************************************
;copy values on entry from Registers to TempR*x also R8,9,10,11,12,13,14,15
public StoreAll
StoreAll proc
mov Temprax,rax						
mov Temprbx,rbx			
mov Temprcx,rcx				
mov Temprdx,rdx					
mov TempR8,r8						
mov TempR9,r9						
mov TempR10,r10
mov TempR11,r11
mov TempR12,r12					
mov TempR13,r13						
mov TempR14,r14
mov TempR15,r15
ret
StoreAll endp
;**********************************************************************************
;******************************** RestoreAll **************************************
;copy values on entry to Registers from TempR*x also R8,9,10,11,12,13,14,15
public RestoreAll
RestoreAll proc
mov rax,Temprax						
mov rbx,Temprbx						
mov rcx,Temprcx						
mov rdx,Temprdx						
mov r8,TempR8						
mov r9,TempR9						
mov r10,TempR10
mov r11,TempR11
mov r12,TempR12					
mov r13,TempR13						
mov r14,TempR14
mov r15,TempR15
ret
RestoreAll endp
;**************************************************************************************

;****************************************************************************************
End
