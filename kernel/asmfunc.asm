;
; Nova27's Operating System
;
; Copyright (c) 2020 nova27
;
; This software is released under the MIT License.
; http://opensource.org/licenses/mit-license.php

extern TimerInterrupt

global io_out32
io_out32: ; void io_out32(int port, int data);
    mov     edx, edi   ; port
    mov     eax, esi   ; data
    out     dx, eax
    ret

global io_in32
io_in32: ; int io_in32 (int port);
    mov     edx, edi   ; port
    in      eax, dx
    ret

global default_handler
default_handler:
	mov eax, 0x100
    hlt
    jmp default_handler

global timer_handler
timer_handler:
	push rax
	push rcx
	push rdx
	push rbx
	push rbp
	push rsi
	push rdi
	call TimerInterrupt
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	pop rdx
	pop rcx
	pop rax
	iretq