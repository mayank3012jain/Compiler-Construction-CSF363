extern	scanf
extern	printf

section	.data
	printI	db	'%d', 10, 0
	printR	db	'%f', 10, 0
	getI	db	'%d', 0
	getR	db	'%f', 0
	printTRUE	db	'TRUE', 10, 0
	printFALSE	db	'FALSE', 10, 0
	bufferInt	db	0
	ouputInt	db	0

	t0	dw	0
	t1	dw	0
	t2	dw	0
	t3	dw	0
	t4	dw	0
	t5	dw	0
	t_6	dw	0
	t_7	dw	0
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
	 mov word[t0], 5
	mov	ax, word[t0]
	mov	word[rbp - 6], ax
	 mov word[t0], 9
	mov	ax, word[t0]
	mov	word[rbp - 8], ax
	;
	mov rsi,bufferInt
	mov rdi, getI
	mov al, 0
	call scanf

	mov bx, [bufferInt]
	mov word[rbp-0], bx

	mov rsi,bufferInt
	mov rdi, getI
	mov al, 0
	call scanf

	mov bx, [bufferInt]
	mov word[rbp-2], bx

	 mov   r8w, word[rbp - 0]
	 mov  word[t1], r8w
	 mov   r8w, word[rbp - 2]
	 mov  word[t4], r8w
	 mov   r8w, word[rbp - 8]
	 mov  word[t5], r8w
	 mov  r8w, word[t4]
	 mov  r9w, word[t5]
	 imul r8w, r9w
	 mov  word[t3], r8w
	 mov   r8w, word[rbp - 6]
	 mov  word[t_7], r8w
	 mov   r8w, word[rbp - 8]
	 mov  word[t8], r8w
	 mov  r8w, word[t_7]
	 sub r8w, word[t8]
	 mov  word[t_6], r8w
	 mov   r8w, word[rbp - 2]
	 mov  word[t_7], r8w
	 mov  r8w, word[t_6]
	 mov  r9w, word[t_7]
	 imul r8w, r9w
	 mov  word[t5], r8w
	 mov   r8w, word[rbp - 6]
	 mov  word[t8], r8w
	 mov word[t9], 2
	 mov  r8w, word[t8]
	 mov  r9w, word[t9]
	 imul r8w, r9w
	 mov  word[t_7], r8w
	 mov   r8w, word[rbp - 8]
	 mov  word[t9], r8w
	 mov   r8w, word[rbp - 0]
	 mov  word[t10], r8w
	 mov  r8w, word[t9]
	 mov  r9w, word[t10]
	 imul r8w, r9w
	 mov  word[t8], r8w
	 mov  r8w, word[t_7]
	 sub r8w, word[t8]
	 mov  word[t_6], r8w
	 mov  r8w, word[t5]
	 add r8w, word[t_6]
	 mov  word[t4], r8w
	 mov  r8w, word[t3]
	 add r8w, word[t4]
	 mov  word[t2], r8w
	 mov  r8w, word[t1]
	 add r8w, word[t2]
	 mov  word[t0], r8w
	mov	r8w, word[t0]
	mov	word[rbp - 4], r8w

	mov rax, 0
	mov ax,  word[rbp - 4]
	movsx rax, ax
	mov rdi, printI
	mov rsi, rax
	mov rax, 0
	call printf

	; mov rax, rbp
	; sub rax, 4
	; mov r8, rax
	; mov rsi, r8
	; mov rdi,printI
	; xor rax, rax
	; call printf


; Exit
	mov rax, 1
	mov rbx, 0
	int 80h
