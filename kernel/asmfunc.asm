;
; Nova27's Operating System
;
; Copyright (c) 2020 nova27
;
; This software is released under the MIT License.
; http://opensource.org/licenses/mit-license.php

extern kbcInterrupt
extern timerInterrupt

global ioOut8
ioOut8: ; void ioOut8(int port, char data);
    mov     edx, edi   ; port
    mov     eax, esi   ; data
    out     dx, al
    ret

global ioIn8
ioIn8: ; int ioIn32 (int port);
    mov     edx, edi   ; port
    in      al, dx
    ret

global ioOut32
ioOut32: ; void ioOut32(int port, int data);
    mov     edx, edi   ; port
    mov     eax, esi   ; data
    out     dx, eax
    ret

global ioIn32
ioIn32: ; int ioIn32 (int port);
    mov     edx, edi   ; port
    in      eax, dx
    ret

global defaultHandler
defaultHandler:
    hlt
    jmp defaultHandler

global kbcHandler
kbcHandler:
	push rax
	push rcx
	push rdx
	push rbx
	push rbp
	push rsi
	push rdi
	call kbcInterrupt
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	pop rdx
	pop rcx
	pop rax
	iretq

global timerHandler
timerHandler:
	push rax
	push rcx
	push rdx
	push rbx
	push rbp
	push rsi
	push rdi
	call timerInterrupt
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	pop rdx
	pop rcx
	pop rax
	iretq