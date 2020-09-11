;
; Nova27's Operating System
;
; Copyright (c) 2020 nova27
;
; This software is released under the MIT License.
; http://opensource.org/licenses/mit-license.php

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