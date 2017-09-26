
; MIT License

; Copyright (c) 2017 ZetaChain_Native

; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.


.data
	message dq 0
	dataSize dq 0
	hash dq 5381

.code

HashASM proc
	mov message, rcx
	sub rdx, 1
	mov dataSize, rdx
	mov rbx, 0
	jmp hashLoop
	hashLoop:
		cmp rbx, dataSize
		je done
		call HashMessageASM
		inc rbx
		jmp hashLoop
	done:
		ret
HashASM endp

HashMessageASM proc
	mov hash, 5381
	mov rax, qword ptr [message]
	add rax, rbx
	mov rcx, hash
	shl rcx, 5
	add rcx, hash
	add rcx, rax
	add rcx, rbx
	mov hash, rcx
	mov rax, rcx
	ret
HashMessageASM endp

end
