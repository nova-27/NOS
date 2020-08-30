extern kernel_stack
extern KernelMain

;ブートローダーから呼ばれる
global KernelEntry
KernelEntry:
    mov rsp, kernel_stack + 1024 * 1024  ;スタックポインタを変更しておく
    call KernelMain  ;C言語の関数を呼び出す
;カーネルから帰ってきても何もすることがないので、haltのループに入る
.fin:
    hlt
    jmp .fin