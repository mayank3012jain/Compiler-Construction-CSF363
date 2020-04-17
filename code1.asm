extern	scanf
extern	printf

section	.data
	printI	db	'%d', 10, 0
	printR	db	'%f', 10, 0
	getI	db	'%d', 10, 0
	getR	db	'%f', 10, 0
	printTRUE	db	'TRUE', 10, 0
	printFALSE	db	'FALSE', 10, 0

	t0	db	0
	t1	db	0
	t2	db	0
	t3	db	0
	t4	db	0
	t5	db	0
	t6	db	0
	t7	db	0
	t8	db	0
	t9	db	0
	t10	db	0
	t11	db	0
	t12	db	0
	t13	db	0
	t14	db	0
	t15	db	0
	t16	db	0
	t17	db	0
	t18	db	0
	t19	db	0
	t20	db	0
	t21	db	0
	t22	db	0
	t23	db	0
	t24	db	0
	t25	db	0
	t26	db	0
	t27	db	0
	t28	db	0
	t29	db	0
	t30	db	0
	t31	db	0
	t32	db	0
	t33	db	0
	t34	db	0
	t35	db	0
	t36	db	0
	t37	db	0
	t38	db	0
	t39	db	0

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
	mov	r8w, [t0]
	mov	[rbp - 6], r8w
	 mov word[t0], 9
	mov	r8w, [t0]
	mov	[rbp - 8], r8w
	mov rax,rbp
	sub rax, 0
	mov r8, rax
	mov rsi,	r8
	mov rdi, getI
	mov al, 0
	call scanf
	mov rax,rbp
	sub rax, 2
	mov r8, rax
	mov rsi,	r8
	mov rdi, getI
	mov al, 0
	call scanf
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
	mov	[rbp - 4], r8w
	mov rax, rbp
	sub rax, 4
	mov r8, rax
	mov rsi, r8
	mov rdi,printI
	xor rax, rax
	call printf


; Exit
	mov rax, 1
	mov rbx, 0
	int 80h
