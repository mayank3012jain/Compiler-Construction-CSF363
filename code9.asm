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
	 sub rsp, 9



	mov word[t0], 19
	mov	bx, [t0]
	mov word[rbp - 4], bx

	mov word[t0], 56
	mov	bx, [t0]
	mov word[rbp - 6], bx

	mov r8w, 3
	mov word[rbp - 9], r8w
_L1: 
	cmp r8w, 6
	jg _L2

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

	mov r8w, word[rbp - 2]
	cmp	r8w,	1
	jne	_L4

	mov word  r8w, [rbp - 2]
	mov word [t3], r8w
	mov word[t4], 2
	mov word r8w, [t3]
	mov word r9w, [t4]
	imul r8w, r9w
	mov word [t2], r8w
	mov word  r8w, [rbp - 4]
	mov word [t3], r8w
	mov word r8w, [t2]
	sub r8w, [t3]
	mov word [t1], r8w
	mov word  r8w, [rbp - 9]
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
	jne	_L3
_L4:
	mov r8w, word[rbp - 2]
	cmp	r8w,	2
	jne	_L5

	mov word  r8w, [rbp - 2]
	mov word [t3], r8w
	mov word[t4], 3
	mov word r8w, [t3]
	mov word r9w, [t4]
	imul r8w, r9w
	mov word [t2], r8w
	mov word  r8w, [rbp - 6]
	mov word [t3], r8w
	mov word r8w, [t2]
	sub r8w, [t3]
	mov word [t1], r8w
	mov word  r8w, [rbp - 9]
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
	jne	_L3
_L5:

	mov rax, 0
	mov ax, word[rbp - 2]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

_L3:

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
	mov r8w, word[rbp - 9]
	inc r8w
	mov word[rbp - 9], r8w
	jmp _L1

_L2:
	jmp _exit

_exit:
	mov rax, 1
	mov rbx, 0
	int 80h
