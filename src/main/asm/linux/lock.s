.data
	timeout dq 0
.text 


lockASM:
	push %rax
	push %rcx
	movq $0h, %rax
	movq %rcx, ${timeout}
	jmp spin
	spin:
		nop
		inc %rax
		cmpq %rax, ${timeout}
		jl spin
	pop %rcx
	pop %rax
	ret
