extern	scanf
extern	printf

SECTION	.data
	printI	db	'%d', 10, 0
	printR	db	'%f', 10, 0
	getI	db	'%d', 10, 0
	getR	db	'%f', 10, 0
	printTRUE	db	'TRUE', 10, 0
	printFALSE	db	'FALSE', 10, 0
SECTION	.text
	bits 32
	global	main

main:
	sub	esp, 6
	sub	esp, 6
	mov	r8w, [t0]
	mov	[_driverFunctionNode_local - 6], r8w
	mov	r8w, [t0]
	mov	[_driverFunctionNode_local - 8], r8w
	mov r8w,	_driverFunctionNode_local - 0
	push r8w
	push getI
	call _scanf
	mov r8w,	_driverFunctionNode_local - 2
	push r8w
	push getI
	call _scanf
	mov r8w, _driverFunctionNode_local - 4
	push r8w
	push printI
	call _printf

SECTION	.bss
	_driverFunctionNode_ret	resw	1
	_driverFunctionNode_local	resw	1
