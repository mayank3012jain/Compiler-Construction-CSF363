extern	scanf
extern	printf

section	.data
	printTRUE	db	'TRUE', 10, 0
	printFALSE	db	'FALSE', 10, 0
	printI	db	'%d', 10, 0
	printR	db	'%f', 10, 0
	printIArr	db	'%d ', 0
	printRArr	db	'%f ', 0
	getI	db	'%d', 0
	getR	db	'%f', 0
	printTRUEArr	db	'TRUE ', 0
	printFALSEArr	db	'FALSE ', 0

	bufferInt	db	0
	outputInt	db	0
	t0	dw	0
	t1	dw	0
	t2	dw	0
	t3	dw	0
	t4	dw	0
	t5	dw	0
	t6	dw	0
	t7	dw	0
	t8	dw	0
	t9	dw	0
	t10	dw	0
	t11	dw	0
	t12	dw	0
	t13	dw	0
	t14	dw	0
	t15	dw	0
	t16	dw	0
	t17	dw	0
	t18	dw	0
	t19	dw	0
	t20	dw	0
	t21	dw	0
	t22	dw	0
	t23	dw	0
	t24	dw	0
	t25	dw	0
	t26	dw	0
	t27	dw	0
	t28	dw	0
	t29	dw	0
	t30	dw	0
	t31	dw	0
	t32	dw	0
	t33	dw	0
	t34	dw	0
	t35	dw	0
	t36	dw	0
	t37	dw	0
	t38	dw	0
	t39	dw	0

section	.bss

section	.text
bits 64

global	main

main:
	push rbp
	mov rbp, rsp

	sub	rsp, 6

	sub	rsp, 6

	sub	rsp, 2

	sub	rsp, 13

	 mov byte[t0], 1
	mov	r8b, [t0]
	mov	byte[rbp - 12], r8b

	 mov word[t0], 5
	mov	r8w, [t0]
	mov	word[rbp - 6], r8w

	 mov word[t0], 9
	mov	r8w, [t0]
	mov	word[rbp - 8], r8w

	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word [rbp - 0], bx

	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word [rbp - 2], bx

	 mov word  r8w, [rbp - 0]
	 mov word [t1], r8w
	 mov word  r8w, [rbp - 2]
	 mov word [t4], r8w
	 mov word  r8w, [rbp - 8]
	 mov word [t5], r8w
	 mov word r8w, [t4]
	 mov word r9w, [t5]
	 imul r8w, r9w
	 mov word [t3], r8w
	 mov word  r8w, [rbp - 6]
	 mov word [t7], r8w
	 mov word  r8w, [rbp - 8]
	 mov word [t8], r8w
	 mov word r8w, [t7]
	 sub r8w, [t8]
	 mov word [t6], r8w
	 mov word  r8w, [rbp - 2]
	 mov word [t7], r8w
	 mov word r8w, [t6]
	 mov word r9w, [t7]
	 imul r8w, r9w
	 mov word [t5], r8w
	 mov word  r8w, [rbp - 6]
	 mov word [t8], r8w
	 mov word[t9], 2
	 mov word r8w, [t8]
	 mov word r9w, [t9]
	 imul r8w, r9w
	 mov word [t7], r8w
	 mov word  r8w, [rbp - 8]
	 mov word [t9], r8w
	 mov word  r8w, [rbp - 0]
	 mov word [t10], r8w
	 mov word r8w, [t9]
	 mov word r9w, [t10]
	 imul r8w, r9w
	 mov word [t8], r8w
	 mov word r8w, [t7]
	 sub r8w, [t8]
	 mov word [t6], r8w
	 mov word r8w, [t5]
	 add r8w, [t6]
	 mov word [t4], r8w
	 mov word r8w, [t3]
	 add r8w, [t4]
	 mov word [t2], r8w
	 mov word r8w, [t1]
	 add r8w, [t2]
	 mov word [t0], r8w
	mov	r8w, [t0]
	mov	word[rbp - 4], r8w

	 mov word  r8w, [rbp - 4]
	 mov word [t2], r8w
	 mov word[t3], 10
	 mov word r8w, [t2]
	 cmp r8w, [t3]
	 jg true_L1 
	 mov byte [t1], 0
	 jmp exit_L1
true_L1: mov byte [t1], 1
exit_L1:
	 mov word  r8w, [rbp - 6]
	 mov word [t4], r8w
	 mov word  r8w, [rbp - 8]
	 mov word [t5], r8w
	 mov word r8w, [t4]
	 cmp r8w, [t5]
	 jle true_L2 
	 mov byte [t3], 0
	 jmp exit_L2
true_L2: mov byte [t3], 1
exit_L2:
	 mov word  r8w, [rbp - 0]
	 mov word [t6], r8w
	 mov word  r8w, [rbp - 2]
	 mov word [t7], r8w
	 mov word r8w, [t6]
	 cmp r8w, [t7]
	 jl true_L3 
	 mov byte [t5], 0
	 jmp exit_L3
true_L3: mov byte [t5], 1
exit_L3:
	 mov byte  r8b, [rbp - 12]
	 mov byte [t6], r8b
	 mov byte r8b, [t5]
	 and r8b, [t6]
	 mov byte [t4], r8b
	 mov byte r8b, [t3]
	 and r8b, [t4]
	 mov byte [t2], r8b
	 mov byte r8b, [t1]
	 or r8b, [t2]
	 mov byte [t0], r8b
	mov	r8b, [t0]
	mov	byte[rbp - 13], r8b

	mov rax, 0
	mov ax, word[rbp - 4]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov r8b, byte[rbp - 12]
	sub r8b, 1
	jz _L4
	mov rdi, printFALSE
	jmp _L5
_L4:
	mov rdi, printTRUE
_L5:
	mov rsi, 0
	mov rax, 0
	call printf

	mov r13, 0
	mov r12, rbp
	sub r12, 14
_L6:
	cmp r13, 6
	jge _L7
	mov rsi,	bufferInt
	mov rdi, getI
	mov al, 0
	call scanf
	mov bx, word[bufferInt]
	mov word [r12], bx
	sub r12, 2
	add r13, 1
	jmp _L6
_L7:

	 mov word  r8w, [rbp - 6]
	 mov word [t1], r8w
	 mov word  r8w, [rbp - 0]
	 mov word [t3], r8w
	 mov word[t7], 2
	 mov word[t9], 3
	 mov word r8w, [t7]
	 add r8w, [t8]
	 mov word [t6], r8w
	 mov word r8w, [t5]
	 add r8w, [t6]
	 mov word [t4], r8w
	 mov word r8w, [t3]
	 add r8w, [t4]
	 mov word [t2], r8w
	 mov word r8w, [t1]
	 add r8w, [t2]
	 mov word [t0], r8w
	mov	r8w, [t0]
	mov	word[rbp - 2], r8w

	mov rax, 0
	mov ax, word[rbp - 2]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf

	mov r13, 0
	mov r12, rbp
	sub r12, 14
_L8:
	cmp r13, 6
	jge _L9
	mov rax, 0
	mov ax, word[r12]
	movsx rax, ax
	mov rdi,printI
	mov rsi, rax
	mov rax, 0
	call printf
	sub r12, 2
	add r13, 1
	jmp _L8
_L9:


; Exit
	mov rax, 1
	mov rbx, 0
	int 80h
