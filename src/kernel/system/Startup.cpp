//
// Kernel System
//
#include "Startup.hpp"
#include "../threading/ProcessManager.hpp"
#include "../diagnostic/KernelLogger.hpp"
#include "../memory/MemoryManager.hpp"
#include "../object/ObjectManager.hpp"
#include <libbsp/bsp.hpp>

using namespace Chino;

void Chino::SystemStartup(const BootParameters& params)
{
	g_Logger->PutString(L"\nChino is successfully loaded ♥\n");
	g_Logger->PutFormat(L"Free memory avaliable: %z bytes\n", g_MemoryMgr->GetFreeBytesRemaining());

	BSPSystemStartup();

	while (1)
		ArchHaltProcessor();
}
