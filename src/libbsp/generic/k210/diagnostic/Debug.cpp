//
// Kernel Diagnostic
//
#include <kernel/utils.hpp>
#include <libbsp/bsp.hpp>
#include <uarths.h>

using namespace Chino;

void Chino::Diagnostic::BSPInitializeDebug(const BootParameters& bootParams)
{
    uart_init();
}

void Chino::Diagnostic::BSPDebugPutChar(wchar_t chr)
{
    uart_putchar((char)chr);
}

void Chino::Diagnostic::BSPDebugBlueScreen()
{
}

void Chino::Diagnostic::BSPDebugClearScreen()
{
}
