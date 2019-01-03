//
// Chino Arch Port
//
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct
	{
		uint8_t context[1232];
	} ThreadContext_Arch;

#define Port_StackWidth 8
#ifdef __cplusplus
}
#endif
