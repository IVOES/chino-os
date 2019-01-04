EXTERN g_CurrentThreadContext : QWORD
EXTERN Kernel_SwitchThreadContext : PROC

PUBLIC ArchSwitchContextCore

.code
ArchSwitchContextCore PROC

    pushfq
    push rax
    mov rax, [g_CurrentThreadContext]
    test rax, rax
    jz _restore
_save:
    mov [rax + 8 * 1], rbx
    mov [rax + 8 * 2], rcx
    mov [rax + 8 * 3], rdx
    mov [rax + 8 * 4], rbp
    mov [rax + 8 * 5], rdi
    mov [rax + 8 * 6], rsi
    mov [rax + 8 * 7], r8
    mov [rax + 8 * 8], r9
    mov [rax + 8 * 9], r10
    mov [rax + 8 * 10], r11
    mov [rax + 8 * 11], r12
    mov [rax + 8 * 12], r13
    mov [rax + 8 * 13], r14
    mov [rax + 8 * 14], r15

	pop rbx ; rax
	mov [rax], rbx

	pop rbx ; rflags
	mov [rax + 8 * 17], rbx

	pop rbx ; rip
	mov [rax + 8 * 15], rbx

	push fs
	pop bx
	mov [rax + 8 * 18], bx

	push gs
	pop bx
	mov [rax + 8 * 18 + 2], bx

	mov [rax + 8 * 16], rsp
	jmp _restore_2
_restore:
    pop rax
    pop rax
    pop rax
_restore_2:
    call Kernel_SwitchThreadContext
    mov rax, [g_CurrentThreadContext]
	mov rcx, [rax + 8 * 2]
	mov rdx, [rax + 8 * 3]
	mov rbp, [rax + 8 * 4]
	mov rdi, [rax + 8 * 5]
	mov rsi, [rax + 8 * 6]
	mov r8,  [rax + 8 * 7]
	mov r9,  [rax + 8 * 8]
	mov r10, [rax + 8 * 9]
	mov r11, [rax + 8 * 10]
	mov r12, [rax + 8 * 11]
	mov r13, [rax + 8 * 12]
	mov r14, [rax + 8 * 13]
	mov r15, [rax + 8 * 14]

	mov rsp, [rax + 8 * 16]
	mov rbx, [rax + 8 * 15] ; rip
	push rbx

	mov rbx, [rax + 8 * 17] ; rflags
	test rbx, rbx
	jz _restore_end
	push rbx
	popfq

	mov bx, [rax + 8 * 18] ; fs
	push bx
	pop fs

	mov bx, [rax + 8 * 18 + 2] ; gs
	push bx
	pop gs
_restore_end:
    mov rbx, [rax + 8 * 1]
	mov rax, [rax]
	ret

ArchSwitchContextCore ENDP
END