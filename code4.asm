extern	scanf
extern	printf

section	.data
	printTRUE	db	'Output: TRUE', 10, 0
	printFALSE	db	'Output: FALSE', 10, 0
	printI	db	'Output: %d', 10, 0
	printR	db	'Output: %f', 10, 0
	printINPUTINT	db	'Input: Enter an INTEGER value:', 10, 0
	printINPUTBOOL	db	'Input: Enter a BOOLEAN (0/1) Value:', 10, 0
	printINPUTINTARR	db	'Input: Enter %d INTEGER values:', 10, 0
	printINPUTBOOLARR	db	'Input: Enter %d BOOLEAN (0/1) values:', 10, 0
	printArrUtil1	db	'Output: ', 0
	printIArr	db	'%d ', 0
	printRArr	db	'%f ', 0
	printArrUtil2	db	' ', 10, 0
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
	 sub rsp, 33




	mov byte[t0], 1
	mov	r8b, [t0]
	mov byte[rbp - 15], r8b

	mov word[t0], 5
	mov	bx, [t0]
	mov word[rbp - 10], bx

	mov word[t0], 9
	mov	bx, [t0]
	mov word[rbp - 12], bx

	mov rdi, printINPUTINT
	mov rsi, 0
	xor rax, rax
	call printf
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 2], bx

	mov rdi, printINPUTINT
	mov rsi, 0
	xor rax, rax
	call printf
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 4], bx

	mov word  r8w, [rbp - 2]
	mov word [t4], r8w
	mov word  r8w, [rbp - 4]
	mov word [t6], r8w
	mov word  r8w, [rbp - 12]
	mov word [t7], r8w
	mov word r8w, [t6]
	mov word r9w, [t7]
	imul r8w, r9w
	mov word [t5], r8w
	mov word r8w, [t4]
	add r8w, [t5]
	mov word [t3], r8w
	mov word  r8w, [rbp - 10]
	mov word [t6], r8w
	mov word  r8w, [rbp - 12]
	mov word [t7], r8w
	mov word r8w, [t6]
	sub r8w, [t7]
	mov word [t5], r8w
	mov word  r8w, [rbp - 4]
	mov word [t6], r8w
	mov word r8w, [t5]
	mov word r9w, [t6]
	imul r8w, r9w
	mov word [t4], r8w
	mov word r8w, [t3]
	add r8w, [t4]
	mov word [t2], r8w
	mov word  r8w, [rbp - 10]
	mov word [t4], r8w
	mov word[t5], 2
	mov word r8w, [t4]
	mov word r9w, [t5]
	imul r8w, r9w
	mov word [t3], r8w
	mov word r8w, [t2]
	add r8w, [t3]
	mov word [t1], r8w
	mov word  r8w, [rbp - 12]
	mov word [t3], r8w
	mov word  r8w, [rbp - 2]
	mov word [t4], r8w
	mov word r8w, [t3]
	mov word r9w, [t4]
	imul r8w, r9w
	mov word [t2], r8w
	mov word r8w, [t1]
	sub r8w, [t2]
	mov word [t0], r8w
	mov	bx, [t0]
	mov word[rbp - 6], bx

	mov rax, 0
	mov ax, word[rbp - 6]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov r8w, 10
	mov word[rbp - 8], r8w
_L1: 
	cmp r8w, 15
	jg _L2


	mov word[t0], 7
	mov	bx, [t0]
	mov word[rbp - 31], bx

	mov rdi, printINPUTINT
	mov rsi, 0
	xor rax, rax
	call printf
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 33], bx

	mov word  r8w, [rbp - 31]
	mov word [t1], r8w
	mov word  r8w, [rbp - 33]
	mov word [t2], r8w
	mov word r8w, [t1]
	add r8w, [t2]
	mov word [t0], r8w
	mov	bx, [t0]
	mov word[rbp - 31], bx

	mov rax, 0
	mov ax, word[rbp - 31]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov word  r8w, [rbp - 6]
	mov word [t2], r8w
	mov word  r8w, [rbp - 8]
	mov word [t3], r8w
	mov word r8w, [t2]
	add r8w, [t3]
	mov word [t1], r8w
	mov word  r8w, [rbp - 31]
	mov word [t3], r8w
	mov word[t4], 2
	mov word r8w, [t3]
	mov word r9w, [t4]
	imul r8w, r9w
	mov word [t2], r8w
	mov word r8w, [t1]
	add r8w, [t2]
	mov word [t0], r8w
	mov	bx, [t0]
	mov word[rbp - 6], bx

	mov rax, 0
	mov ax, word[rbp - 6]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf
	mov r8w, word[rbp - 8]
	inc r8w
	mov word[rbp - 8], r8w
	jmp _L1

_L2:

	mov word  r8w, [rbp - 2]
	mov word [t4], r8w
	mov word  r8w, [rbp - 4]
	mov word [t6], r8w
	mov word  r8w, [rbp - 12]
	mov word [t7], r8w
	mov word r8w, [t6]
	mov word r9w, [t7]
	imul r8w, r9w
	mov word [t5], r8w
	mov word r8w, [t4]
	add r8w, [t5]
	mov word [t3], r8w
	mov word  r8w, [rbp - 10]
	mov word [t6], r8w
	mov word  r8w, [rbp - 12]
	mov word [t7], r8w
	mov word r8w, [t6]
	sub r8w, [t7]
	mov word [t5], r8w
	mov word  r8w, [rbp - 4]
	mov word [t6], r8w
	mov word r8w, [t5]
	mov word r9w, [t6]
	imul r8w, r9w
	mov word [t4], r8w
	mov word r8w, [t3]
	add r8w, [t4]
	mov word [t2], r8w
	mov word  r8w, [rbp - 10]
	mov word [t4], r8w
	mov word[t5], 2
	mov word r8w, [t4]
	mov word r9w, [t5]
	imul r8w, r9w
	mov word [t3], r8w
	mov word r8w, [t2]
	add r8w, [t3]
	mov word [t1], r8w
	mov word  r8w, [rbp - 12]
	mov word [t3], r8w
	mov word  r8w, [rbp - 2]
	mov word [t4], r8w
	mov word r8w, [t3]
	mov word r9w, [t4]
	imul r8w, r9w
	mov word [t2], r8w
	mov word r8w, [t1]
	sub r8w, [t2]
	mov word [t0], r8w
	mov	bx, [t0]
	mov word[rbp - 6], bx

	mov word  r8w, [rbp - 6]
	mov word [t4], r8w
	mov word[t5], 10
	mov word r8w, [t4]
	cmp r8w, [t5]
	jg true_L3 
	mov byte [t3], 0
	jmp exit_L3
true_L3: mov byte [t3], 1
exit_L3:
	mov word  r8w, [rbp - 10]
	mov word [t5], r8w
	mov word  r8w, [rbp - 12]
	mov word [t6], r8w
	mov word r8w, [t5]
	cmp r8w, [t6]
	jle true_L4 
	mov byte [t4], 0
	jmp exit_L4
true_L4: mov byte [t4], 1
exit_L4:
	mov byte r8b, [t3]
	or r8b, [t4]
	mov byte [t2], r8b
	mov word  r8w, [rbp - 2]
	mov word [t4], r8w
	mov word  r8w, [rbp - 4]
	mov word [t5], r8w
	mov word r8w, [t4]
	cmp r8w, [t5]
	jl true_L5 
	mov byte [t3], 0
	jmp exit_L5
true_L5: mov byte [t3], 1
exit_L5:
	mov byte r8b, [t2]
	and r8b, [t3]
	mov byte [t1], r8b
	mov byte  r8b, [rbp - 15]
	mov byte [t2], r8b
	mov byte r8b, [t1]
	and r8b, [t2]
	mov byte [t0], r8b
	mov	r8b, [t0]
	mov byte[rbp - 16], r8b

	mov rax, 0
	mov ax, word[rbp - 6]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov r8b, byte[rbp - 15]
	sub r8b, 1
	jz _L6
	mov rdi, printFALSE
	jmp _L7
_L6:
	mov rdi, printTRUE
_L7:
	mov rsi, 0
	mov rax, 0
	call printf

	mov word[t39], 6
	mov rax, 0
	mov ax, word[t39]
	movsx rax, ax
	mov rdi,printINPUTINTARR
	mov rsi, rax
	mov rax, 0
	call printf
	mov r13, 0
	mov r12, rbp
	mov r12, rbp
	sub r12, 18
_L8:
	cmp r13w, word[t39]
	jge _L9
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word [r12], bx
	sub r12, 2
	add r13, 1
	jmp _L8
_L9:

	mov word  r8w, [rbp - 10]
	mov word [t4], r8w
	mov word  r8w, [rbp - 2]
	mov word [t5], r8w
	mov word r8w, [t4]
	add r8w, [t5]
	mov word [t3], r8w
	xor rsi, rsi
	mov si, 13
	sub rsi, 10
	imul rsi, 2
	mov r12, rbp
	sub r12, 18
	sub r12, rsi
	mov r8w, word[r12]
	mov word [t5], r8w
	mov word[t6], 2
	mov word r8w, [t5]
	mov word r9w, [t6]
	imul r8w, r9w
	mov word [t4], r8w
	mov word r8w, [t3]
	add r8w, [t4]
	mov word [t2], r8w
	xor rsi, rsi
	mov si, 14
	sub rsi, 10
	imul rsi, 2
	mov r12, rbp
	sub r12, 18
	sub r12, rsi
	mov r8w, word[r12]
	mov word [t4], r8w
	mov word[t5], 3
	mov word r8w, [t4]
	mov word r9w, [t5]
	imul r8w, r9w
	mov word [t3], r8w
	mov word r8w, [t2]
	add r8w, [t3]
	mov word [t1], r8w
	xor rsi, rsi
	mov si, 15
	sub rsi, 10
	imul rsi, 2
	mov r12, rbp
	sub r12, 18
	sub r12, rsi
	mov r8w, word[r12]
	mov word [t2], r8w
	mov word r8w, [t1]
	add r8w, [t2]
	mov word [t0], r8w
	mov	bx, [t0]
	mov word[rbp - 4], bx

	mov rax, 0
	mov ax, word[rbp - 4]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rdi, printArrUtil1
	mov rsi, 0
	mov rax, 0
	call printf
	mov r13, 0
	mov r12, rbp
	sub r12, 18
_L10:
	cmp r13, 6
	jge _L11
	mov rax, 0
	mov ax, word[r12]
	movsx rax, ax
	mov rdi,printIArr
	mov rsi, rax
	mov rax, 0
	call printf
	sub r12, 2
	add r13, 1
	jmp _L10
_L11:
	mov rdi, printArrUtil2
	mov rsi, 0
	mov rax, 0
	call printf
	jmp _exit

_exit:
	mov rax, 1
	mov rbx, 0
	int 80h
