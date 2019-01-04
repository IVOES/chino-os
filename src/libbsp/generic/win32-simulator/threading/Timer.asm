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

	movaps [rax + 8 * 16], xmm0
	movaps [rax + 8 * 18], xmm1
	movaps [rax + 8 * 20], xmm2
	movaps [rax + 8 * 22], xmm3
	movaps [rax + 8 * 24], xmm4
	movaps [rax + 8 * 26], xmm5
	movaps [rax + 8 * 28], xmm6
	movaps [rax + 8 * 30], xmm7
	movaps [rax + 8 * 32], xmm8
	movaps [rax + 8 * 34], xmm9
	movaps [rax + 8 * 36], xmm10
	movaps [rax + 8 * 38], xmm11
	movaps [rax + 8 * 40], xmm12
	movaps [rax + 8 * 42], xmm13
	movaps [rax + 8 * 44], xmm14
	movaps [rax + 8 * 46], xmm15

	pop rbx ; rax
	mov [rax], rbx

	pop rbx ; rflags
	mov [rax + 8 * 49], rbx

	pop rbx ; rip
	mov [rax + 8 * 15], rbx

	push fs
	pop bx
	mov [rax + 8 * 50], bx

	push gs
	pop bx
	mov [rax + 8 * 50 + 2], bx

	mov [rax + 8 * 48], rsp
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
	
	movaps xmm0,  [rax + 8 * 16] 
	movaps xmm1,  [rax + 8 * 18] 
	movaps xmm2,  [rax + 8 * 20] 
	movaps xmm3,  [rax + 8 * 22] 
	movaps xmm4,  [rax + 8 * 24] 
	movaps xmm5,  [rax + 8 * 26] 
	movaps xmm6,  [rax + 8 * 28] 
	movaps xmm7,  [rax + 8 * 30] 
	movaps xmm8,  [rax + 8 * 32] 
	movaps xmm9,  [rax + 8 * 34] 
	movaps xmm10, [rax + 8 * 36] 
	movaps xmm11, [rax + 8 * 38] 
	movaps xmm12, [rax + 8 * 40] 
	movaps xmm13, [rax + 8 * 42] 
	movaps xmm14, [rax + 8 * 44] 
	movaps xmm15, [rax + 8 * 46] 

	mov rsp, [rax + 8 * 48]
	mov rbx, [rax + 8 * 15] ; rip
	push rbx

	mov rbx, [rax + 8 * 49] ; rflags
	test rbx, rbx
	jz _restore_end
	push rbx
	popfq

	mov bx, [rax + 8 * 50] ; fs
	push bx
	pop fs

	mov bx, [rax + 8 * 50 + 2] ; gs
	push bx
	pop gs
_restore_end:
    mov rbx, [rax + 8 * 1]
	mov rax, [rax]
	ret

ArchSwitchContextCore ENDP
END