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
    jmp default_handler