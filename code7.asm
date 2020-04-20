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

	printError	db	'Error- Out of bounds ', 10, 0

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
	 sub rsp, 42



	mov rdi, printINPUTINT
	mov rsi, 0
	xor rax, rax
	call printf
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 16], bx

	mov rdi, printINPUTINT
	mov rsi, 0
	xor rax, rax
	call printf
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word[rbp - 18], bx

	mov r8, rsp
	sub r8, 2
	mov qword[rbp - 26], r8
	mov r8w, word[rbp - 16]
	mov r9, 0
	mov r9w, word[rbp - 18]
	sub r9w, r8w
	imul r9w, 2
	sub rsp, r9
	mov r8, rsp
	sub r8, 2
	mov qword[rbp - 34], r8
	mov r8w, word[rbp - 16]
	mov r9, 0
	mov r9w, word[rbp - 18]
	sub r9w, r8w
	imul r9w, 2
	sub rsp, r9
	mov r8, rsp
	sub r8, 2
	mov qword[rbp - 42], r8
	mov r8w, word[rbp - 16]
	mov r9, 0
	mov r9w, word[rbp - 18]
	sub r9w, r8w
	imul r9w, 2
	sub rsp, r9

	mov r14, 0
	mov r14w, word[rbp - 18]
	sub r14w, word[rbp - 16]
	add r14w, 1
	mov word[t39],r14w
	mov rax, 0
	mov ax, word[t39]
	movsx rax, ax
	mov rdi,printINPUTINTARR
	mov rsi, rax
	mov rax, 0
	call printf
	mov r13, 0
	mov r12, rbp
	mov r15, r13
	imul r15, 2
	mov r12, qword[rbp - 26]
	sub r12, r15
_L1:
	cmp r13w, word[t39]
	jge _L2
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word [r12], bx
	sub r12, 2
	add r13, 1
	jmp _L1
_L2:

	mov r14, 0
	mov r14w, word[rbp - 18]
	sub r14w, word[rbp - 16]
	add r14w, 1
	mov word[t39],r14w
	mov rax, 0
	mov ax, word[t39]
	movsx rax, ax
	mov rdi,printINPUTINTARR
	mov rsi, rax
	mov rax, 0
	call printf
	mov r13, 0
	mov r12, rbp
	mov r15, r13
	imul r15, 2
	mov r12, qword[rbp - 34]
	sub r12, r15
_L3:
	cmp r13w, word[t39]
	jge _L4
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word [r12], bx
	sub r12, 2
	add r13, 1
	jmp _L3
_L4:

	mov word[t0], 7
	mov	bx, [t0]
	mov word[rbp - 6], bx

	mov r8w, 1
	mov word[rbp - 8], r8w
_L5: 
	cmp r8w, 3
	jg _L6

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

	mov word[t0], 2
	mov r12, 0
	mov r13, 0
	mov r13w, word[rbp - 16]
	mov r12w, word[rbp - 2]
	mov r14w, word[rbp - 16]
	cmp r12w, r14w
	jl _L8
	mov r14w, word[rbp - 18]
	cmp r12w, r14w
	jle _L7
_L8: 
	mov rdi, printError
	mov rsi, 0
	mov rax, 0
	call printf
	jmp _exit
_L7: 
	sub r12w, r13w
	mov r10, r12
	imul r10, 2
	mov r11, qword[rbp - 26]
	sub r11, r10
	mov r8w, word[t0]
	mov word[r11], r8w

	mov rax, 0
	mov ax, word[rbp - 10]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 12]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov rax, 0
	mov ax, word[rbp - 14]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf
	mov r8w, word[rbp - 8]
	inc r8w
	mov word[rbp - 8], r8w
	jmp _L5

_L6:
	jmp _exit

_exit:
	mov rax, 1
	mov rbx, 0
	int 80h
