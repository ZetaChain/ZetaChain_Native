; MIT License

; Copyright (c) 2017 ZetaChain_Native

; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:

; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.

; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.


.data
	timeout dq 0

.code 

lockBlockASM proc
	push rax
	push rcx
	push rdx
	mov rax, 0h
	mov rdx, 10h
	not rdx
	mov timeout, rcx
	jmp spin
	spin:
		nop
		inc rax
		cmp rax, qword ptr [timeout]
		jl spin
	pop rdx
	pop rcx
	pop rax
	ret
lockBlockASM endp

lockBlockChainASM proc
	push rax
	push rcx
	push rdx
	mov rax, 0h
	mov rdx, 20h
	not rdx
	mov timeout, rcx
	jmp spin
	spin:
		nop
		inc rax
		cmp rax, qword ptr [timeout]
		jl spin
	pop rdx
	pop rcx
	pop rax
	ret
lockBlockChainASM endp

lockBlockDataASM proc
	push rax
	push rcx
	push rdx
	mov rax, 0h
	mov rdx, 30h
	not rdx
	mov timeout, rcx
	jmp spin
	spin:
		nop
		inc rax
		cmp rax, qword ptr [timeout]
		jl spin
	pop rdx
	pop rcx
	pop rax
	ret
lockBlockDataASM endp

lockTransactionASM proc
	push rax
	push rcx
	push rdx
	mov rax, 0h
	mov rdx, 40h
	not rdx
	mov timeout, rcx
	jmp spin
	spin:
		nop
		inc rax
		cmp rax, qword ptr [timeout]
		jl spin
	pop rdx
	pop rcx
	pop rax
	ret
lockTransactionASM endp

lockTransactionDataASM proc
	push rax
	push rcx
	push rdx
	mov rax, 0h
	mov rdx, 50h
	not rdx
	mov timeout, rcx
	jmp spin
	spin:
		nop
		inc rax
		cmp rax, qword ptr [timeout]
		jl spin
	pop rdx
	pop rcx
	pop rax
	ret
lockTransactionDataASM endp

end