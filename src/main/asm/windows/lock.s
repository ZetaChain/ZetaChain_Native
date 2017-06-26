.data
	timeout dq 0

.code 

_DllMainCRTStartup proc
	nop
	ret
_DllMainCRTStartup endp

lockASM proc
	push rax
	push rcx
	mov rax, 0h
	mov timeout, rcx
	jmp spin
	spin:
		nop
		inc rax
		cmp rax, qword ptr [timeout]
		jl spin
	pop rcx
	pop rax
	ret
lockASM endp

end