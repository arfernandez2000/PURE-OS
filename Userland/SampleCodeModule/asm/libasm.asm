GLOBAL sys_read, sys_write, sys_time, sys_malloc, sys_free, sys_ps,sys_kill_process, sys_block_process, sys_unblock_process
GLOBAL _getMem, sys_loadProcess
GLOBAL raiseOpcodeExc
GLOBAL _getRegs, sys_switchContext
GLOBAL _rbp, _rsp
GLOBAL syscall

section .text

sys_write:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 0
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

sys_read:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 1
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

_getMem:
	push rbp
	mov rbp, rsp

	mov eax, dword [rdi]

	mov rsp, rbp
	pop rbp
	ret

_getRegs:
	mov [regs], rax
	mov [regs + 8], rbx
	mov [regs + 16], rcx
	mov [regs + 24], rdx
	mov [regs + 32], rbp
	mov [regs + 40], rdi
	mov [regs + 48], rsi
	mov [regs + 56], r8
	mov [regs + 64], r9
	mov [regs + 72], r10
	mov [regs + 80], r11
	mov [regs + 88], r12
	mov [regs + 96], r13
	mov [regs + 104], r14
	mov [regs + 112], r15

	lea rax, [rsp + 8]
	mov [regs + 120], rax

	mov rax, [rsp]
	mov [regs + 128], rax

	mov rax, regs
	ret

raiseOpcodeExc:
	ud2

sys_loadProcess:
	push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rcx
	push rdx
	push r10
	push r8
	push r9


	mov r9, r8
	mov r8, r10
	mov r10, rdx
	mov rdx, rcx
	mov rcx, rsi
	mov rsi, rdi
	mov rdi, 3

    int 80h

	pop r9
	pop r8
	pop r10
	pop rdx
	pop rcx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

; sys_switchContext:
; 	push rbp
;     mov rbp, rsp

;     int 81h

;     mov rsp, rbp
;     pop rbp
;     ret


syscall:
    push rbp
    mov rbp, rsp
	pushStateNoRax

    int 80h

	popStateNoRax
	mov rsp, rbp
    pop rbp

    ret
	
sys_time:
	push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rsi, rdi ; option
    mov rdi, 2
    int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

sys_malloc:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 4
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

sys_free:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 5
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

sys_ps:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 6
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret


_rsp:
	mov rax,rsp
	ret
_rbp:
	mov rax, rbp
	ret
sys_kill_process:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 7
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

sys_block_process:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 8
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

sys_unblock_process:
    push rbp
    mov rbp, rsp

	push rdi
	push rsi
	push rdx
	push rcx

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 9
	int 80h

	pop rcx
	pop rdx
	pop rsi
	pop rdi

    mov rsp, rbp
    pop rbp
    ret

section .bss
	regs resb 120