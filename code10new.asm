extern	scanf
extern	printf

section	.data
	printTRUE	db	'TRUE', 10, 0
	printFALSE	db	'FALSE', 10, 0
	printI	db	'Output: %d', 10, 0
	printR	db	'Output: %f', 10, 0
	printIArr	db	'Output: %d ', 0
	printRArr	db	'Output: %f ', 0
	getI	db	'%d', 0
	getR	db	'%f', 0
	printTRUEArr	db	'TRUE ', 0
	printFALSEArr	db	'FALSE ', 0

	bufferInt	db	0
	outputInt	db	0
	t0	dw	0
	_st0	dq	0
	t1	dw	0
	_st1	dq	0
	t2	dw	0
	_st2	dq	0
	t3	dw	0
	_st3	dq	0
	t4	dw	0
	_st4	dq	0
	t5	dw	0
	_st5	dq	0
	t6	dw	0
	_st6	dq	0
	t7	dw	0
	_st7	dq	0
	t8	dw	0
	_st8	dq	0
	t9	dw	0
	_st9	dq	0
	t10	dw	0
	_st10	dq	0
	t11	dw	0
	_st11	dq	0
	t12	dw	0
	_st12	dq	0
	t13	dw	0
	_st13	dq	0
	t14	dw	0
	_st14	dq	0
	t15	dw	0
	_st15	dq	0
	t16	dw	0
	_st16	dq	0
	t17	dw	0
	_st17	dq	0
	t18	dw	0
	_st18	dq	0
	t19	dw	0
	_st19	dq	0
	t20	dw	0
	_st20	dq	0
	t21	dw	0
	_st21	dq	0
	t22	dw	0
	_st22	dq	0
	t23	dw	0
	_st23	dq	0
	t24	dw	0
	_st24	dq	0
	t25	dw	0
	_st25	dq	0
	t26	dw	0
	_st26	dq	0
	t27	dw	0
	_st27	dq	0
	t28	dw	0
	_st28	dq	0
	t29	dw	0
	_st29	dq	0
	t30	dw	0
	_st30	dq	0
	t31	dw	0
	_st31	dq	0
	t32	dw	0
	_st32	dq	0
	t33	dw	0
	_st33	dq	0
	t34	dw	0
	_st34	dq	0
	t35	dw	0
	_st35	dq	0
	t36	dw	0
	_st36	dq	0
	t37	dw	0
	_st37	dq	0
	t38	dw	0
	_st38	dq	0
	t39	dw	0
	_st39	dq	0

section	.bss

section	.text
bits 64

global	main

main:
	push rbp
	mov rbp, rsp
	 sub rsp, 8

	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 2], bx

	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 4], bx


	mov byte [t0], 0
	mov	r8b, [t0]
	mov byte[rbp - 8], r8b

	mov [_st1], rsp
	mov r8, rbp
	sub r8, 7
	push r8
	mov r8, rbp
	sub r8, 6
	push r8
	sub rsp, 1
	mov r9b, byte[rbp - 8]
	mov byte[rsp], r9b
	push word[rbp - 4]
	push word[rbp - 2]
	call	compute_expr
	mov rsp, [_st1]

	mov rax, 0
	mov ax, word[rbp - 2]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 4]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 6]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov r8b, byte[bufferInt]
	mov byte[rbp - 7], r8b

	mov r8b, byte[rbp - 7]
	sub r8b, 1
	jz _L1
	mov rdi, printFALSE
	jmp _L2
_L1:
	mov rdi, printTRUE
_L2:
	mov rsi, 0
	mov rax, 0
	call printf
	jmp _exit
compute_expr:
	push rbp
	mov rbp, rsp
	sub	rsp, 3
	 sub rsp, 7


	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 9], bx

	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 7], bx

	mov word  r8w, [rbp + 16]
	mov word [t3], r8w
	mov word  r8w, [rbp + 18]
	mov word [t4], r8w
	mov word r8w, [t3]
	mov word r9w, [t4]
	imul r8w, r9w
	mov word [t2], r8w
	mov word  r8w, [rbp - 9]
	mov word [t4], r8w
	mov word[t5], 2
	mov word r8w, [t4]
	mov word r9w, [t5]
	imul r8w, r9w
	mov word [t3], r8w
	mov word r8w, [t2]
	add r8w, [t3]
	mov word [t1], r8w
	mov word[t2], 3
	mov word r8w, [t1]
	sub r8w, [t2]
	mov word [t0], r8w
	mov	bx, [t0]
	mov word[rbp - 5], bx

	mov byte  r8b, [rbp + 20]
	mov byte [t1], r8b
	mov word  r8w, [rbp + 16]
	mov word [t3], r8w
	mov word  r8w, [rbp + 18]
	mov word [t4], r8w
	mov word r8w, [t3]
	cmp r8w, [t4]
	jle true_L3 
	mov byte [t2], 0
	jmp exit_L3
true_L3: mov byte [t2], 1
exit_L3:
	mov byte r8b, [t1]
	and r8b, [t2]
	mov byte [t0], r8b
	mov	r8b, [t0]
	mov byte[rbp - 10], r8b

	mov word  r8w, [rbp - 5]
	mov word [t1], r8w
	mov word  r8w, [rbp - 7]
	mov word [t2], r8w
	mov word r8w, [t1]
	add r8w, [t2]
	mov word [t0], r8w
	mov	bx, [t0]
	mov word[rbp - 2], bx

	mov byte  r8b, [rbp - 10]
	mov byte [t1], r8b
	mov word  r8w, [rbp - 5]
	mov word [t3], r8w
	mov word  r8w, [rbp - 7]
	mov word [t4], r8w
	mov word r8w, [t3]
	cmp r8w, [t4]
	jle true_L4 
	mov byte [t2], 0
	jmp exit_L4
true_L4: mov byte [t2], 1
exit_L4:
	mov byte r8b, [t1]
	or r8b, [t2]
	mov byte [t0], r8b
	mov	r8b, [t0]
	mov byte[rbp - 3], r8b

	mov rax, 0
	mov ax, word[rbp + 16]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp + 18]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov r8b, byte[rbp + 20]
	sub r8b, 1
	jz _L5
	mov rdi, printFALSE
	jmp _L6
_L5:
	mov rdi, printTRUE
_L6:
	mov rsi, 0
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 9]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 5]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 7]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 2]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov r8b, byte[rbp - 3]
	sub r8b, 1
	jz _L7
	mov rdi, printFALSE
	jmp _L8
_L7:
	mov rdi, printTRUE
_L8:
	mov rsi, 0
	mov rax, 0
	call printf
	mov r12, [rbp+21]
	mov r8w, word[rbp-2]
	mov word[r12], r8w
	mov r12, [rbp+29]
	mov r8b, byte[rbp-3]
	mov byte[r12], r8b
	leave
	ret

_exit:
	mov rax, 1
	mov rbx, 0
	int 80h
