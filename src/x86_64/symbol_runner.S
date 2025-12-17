// MIT License
//
// Copyright (c) 2025 moi_c_sam, ArthurPV
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <coulang/data_type.h>

.intel_syntax noprefix

.section .bss

.lcomm i_param_0, 8
.lcomm i_param_1, 8
.lcomm i_param_2, 8
.lcomm i_param_3, 8
.lcomm i_param_4, 8
.lcomm i_param_5, 8

.lcomm f_param_0, 8
.lcomm f_param_1, 8
.lcomm f_param_2, 8
.lcomm f_param_3, 8
.lcomm f_param_4, 8
.lcomm f_param_5, 8
.lcomm f_param_6, 8
.lcomm f_param_7, 8

.section .text

.global run__SymbolRunner
.extern is_integer__CoulangValue
.extern get_integer_as_c_value__CoulangValue
.extern get_float_as_c_value__CoulangValue
.extern init_int__CoulangValue
.extern init_float__CoulangValue
.extern init_ptr__CoulangValue
.extern abort

run__SymbolRunner:
	push rbp
	mov rbp, rsp	

	// QWORD %0: addr
	// QWORD %1: params
	// QWORD %2: params_len
	// DWORD %3: data_type
	sub rsp, 64
	mov [rbp - 32], rdi // store %0
	mov [rbp - 24], rsi // store %1
	mov [rbp - 16], rdx // store %2
	mov [rbp - 8], r10d // store %3
	mov QWORD PTR [rbp - 40], 0 // store counter
	mov QWORD PTR [rbp - 48], 0 // store integer param counter
	mov QWORD PTR [rbp - 56], 0 // store float param counter

	mov QWORD PTR [rip + i_param_0], 0
	mov QWORD PTR [rip + i_param_1], 0
	mov QWORD PTR [rip + i_param_2], 0
	mov QWORD PTR [rip + i_param_3], 0
	mov QWORD PTR [rip + i_param_4], 0
	mov QWORD PTR [rip + i_param_5], 0

	mov QWORD PTR [rip + f_param_0], 0
	mov QWORD PTR [rip + f_param_1], 0
	mov QWORD PTR [rip + f_param_2], 0
	mov QWORD PTR [rip + f_param_3], 0
	mov QWORD PTR [rip + f_param_4], 0
	mov QWORD PTR [rip + f_param_5], 0
	mov QWORD PTR [rip + f_param_6], 0
	mov QWORD PTR [rip + f_param_7], 0

.loop_cond_params:
	mov rdi, [rbp - 16]
	cmp QWORD PTR [rbp - 40], rdi
	jl .loop_params
	jmp .call_addr

.loop_params:
	mov rdi, [rbp - 24]
	add rdi, [rbp - 40]
	call is_integer__CoulangValue

	test rax, rax
	jz .loop_i_params
	jmp .loop_f_params

.loop_i_params:
	mov rdi, [rbp - 24]
	add rdi, [rbp - 40]
	call get_integer_as_c_value__CoulangValue
	mov rsi, rax
	mov rdi, [rbp - 48]
	cmp rdi, 0
	je .i_param_0
	cmp rdi, 1
	je .i_param_1
	cmp rdi, 2
	je .i_param_2
	cmp rdi, 3
	je .i_param_3
	cmp rdi, 4
	je .i_param_4
	cmp rdi, 5
	je .i_param_5
	jmp .other_i_param

.i_param_0:
	mov QWORD PTR [rip + i_param_0], rsi
	jmp .end_i_param

.i_param_1:
	mov QWORD PTR [rip + i_param_1], rsi
	jmp .end_i_param

.i_param_2:
	mov QWORD PTR [rip + i_param_2], rsi
	jmp .end_i_param

.i_param_3:
	mov QWORD PTR [rip + i_param_3], rsi
	jmp .end_i_param

.i_param_4:
	mov QWORD PTR [rip + i_param_4], rsi
	jmp .end_i_param

.i_param_5:
	mov QWORD PTR [rip + i_param_5], rsi
	jmp .end_i_param

.other_i_param:
	push rsi
	jmp .end_i_param

.end_i_param:
	inc QWORD PTR [rbp - 48]
	jmp .end_loop_params

.loop_f_params:
	mov rdi, [rbp - 24]
	mov rdi, [rbp - 40]
	call get_float_as_c_value__CoulangValue
	mov rdi, [rbp - 56]
	cmp rdi, 0
	je .f_param_0
	cmp rdi, 1
	je .f_param_1
	cmp rdi, 2
	je .f_param_2
	cmp rdi, 3
	je .f_param_3
	cmp rdi, 4
	je .f_param_4
	cmp rdi, 5
	je .f_param_5
	cmp rdi, 6
	je .f_param_6
	cmp rdi, 7
	je .f_param_7
	jmp .other_f_param

.f_param_0:
	movsd QWORD PTR [rip + f_param_0], xmm0
	jmp .end_f_param

.f_param_1:
	movsd QWORD PTR [rip + f_param_1], xmm0
	jmp .end_f_param

.f_param_2:
	movsd QWORD PTR [rip + f_param_2], xmm0
	jmp .end_f_param

.f_param_3:
	movsd QWORD PTR [rip + f_param_3], xmm0
	jmp .end_f_param

.f_param_4:
	movsd QWORD PTR [rip + f_param_4], xmm0
	jmp .end_f_param

.f_param_5:
	movsd QWORD PTR [rip + f_param_5], xmm0
	jmp .end_f_param

.f_param_6:
	movsd QWORD PTR [rip + f_param_6], xmm0
	jmp .end_f_param

.f_param_7:
	movsd QWORD PTR [rip + f_param_7], xmm0
	jmp .end_f_param

.other_f_param:
	movq rax, xmm0
	push rax
	jmp .end_f_param

.end_f_param:
	inc QWORD PTR [rbp - 56]
	jmp .end_loop_params

.end_loop_params:
	inc QWORD PTR [rbp - 40]
	jmp .loop_cond_params

.call_addr:
	mov rdi, [rip + i_param_0]
	mov rsi, [rip + i_param_1]
	mov rdx, [rip + i_param_2]
	mov r10, [rip + i_param_3]
	mov r8, [rip + i_param_4]
	mov r9, [rip + i_param_5]

	movsd xmm0, [rip + f_param_0]
	movsd xmm1, [rip + f_param_1]
	movsd xmm2, [rip + f_param_2]
	movsd xmm3, [rip + f_param_3]
	movsd xmm4, [rip + f_param_4]
	movsd xmm5, [rip + f_param_5]
	movsd xmm6, [rip + f_param_6]
	movsd xmm7, [rip + f_param_7]

	call [rbp - 16]

	cmp QWORD PTR [rbp - 8], COULANG_DATA_TYPE_INT
	je .ret_int
	cmp QWORD PTR [rbp - 8], COULANG_DATA_TYPE_FLOAT
	je .ret_float
	cmp QWORD PTR [rbp - 8], COULANG_DATA_TYPE_PTR
	je .ret_ptr
	jmp .ret_error

.ret_int:
	mov rdi, rax
	call init_int__CoulangValue
	jmp .exit

.ret_float:
	mov rdi, rax
	call init_float__CoulangValue
	jmp .exit

.ret_ptr:
	mov rdi, rax
	call init_ptr__CoulangValue
	jmp .exit

.ret_error:
	call abort

.exit:
	mov rsp, rbp
	pop rbp
	ret
