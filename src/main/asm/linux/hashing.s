/*
MIT License

Copyright (c) 2017 ZetaChain_Native

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

.data
	message dq 0
	dataSize dq 0
	hash dq 5381

.code

HashASM:
	mov %rcx, $(message)
	sub $1, %rdx
	mov %rdx, $(dataSize)
	mov $0, %rbx
	jmp hashLoop
	hashLoop:
		cmp %rbx $(dataSize),
		je done
		call HashMessageASM
		inc %rbx
		jmp hashLoop
	done:
		ret

HashMessageASM:
	mov $5381, hash
	mov $(message), %rax
	add %rbx, %rax
	mov $(hash), %rcx
	shl $5, %rcx
	add $(hash), %rcx
	add %rax, %rcx
	add %rbx, %rcx
	mov %rcx, hash
	mov %rcx, %rax
	ret
