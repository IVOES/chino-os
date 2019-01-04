//
// Chino Arch Port
//
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
#pragma pack(push, 1)
	typedef struct alignas(16)
	{
		uint64_t low;
		uint64_t hight;
	} ArchUint128;

	typedef struct
	{
		uint64_t rax;
		uint64_t rbx;
		uint64_t rcx;
		uint64_t rdx;
		uint64_t rbp;
		uint64_t rdi;
		uint64_t rsi;

		uint64_t r8;
		uint64_t r9;
		uint64_t r10;
		uint64_t r11;
		uint64_t r12;
		uint64_t r13;
		uint64_t r14;
		uint64_t r15;

		uint64_t rip;

		ArchUint128 xmm0;
		ArchUint128 xmm1;
		ArchUint128 xmm2;
		ArchUint128 xmm3;
		ArchUint128 xmm4;
		ArchUint128 xmm5;
		ArchUint128 xmm6;
		ArchUint128 xmm7;
		ArchUint128 xmm8;
		ArchUint128 xmm9;
		ArchUint128 xmm10;
		ArchUint128 xmm11;
		ArchUint128 xmm12;
		ArchUint128 xmm13;
		ArchUint128 xmm14;
		ArchUint128 xmm15;

		uint64_t rsp;
		uint64_t rflags;
		uint16_t fs;
		uint16_t gs;
	} ThreadContext_Arch;
#pragma pack(pop)

#define Port_StackWidth 8
#ifdef __cplusplus
}
#endif
