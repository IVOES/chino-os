//
// Chino Thread
//
#include <libbsp/bsp.hpp>
#include <libarch/arch.h>
#include <clint.h>

using namespace Chino::Threading;

#define configCPU_CLOCK_HZ 72000000
#define configTICK_CLOCK_HZ (configCPU_CLOCK_HZ / 50)
#define configTICK_RATE_HZ 10

void Chino::Threading::BSPSetupSchedulerTimer()
{
    clint_ipi_enable();
    clint->mtimecmp[0] = clint->mtime + (configTICK_CLOCK_HZ / configTICK_RATE_HZ);
    /* Enable timer interupt */
    __asm volatile("csrs mie,%0" ::"r"(0x80));
}

void Chino::Threading::BSPSleepMs(uint32_t ms)
{
	auto count = configCPU_CLOCK_HZ / 1000 * ms / 3;
	for (size_t i = 0; i < count; i++)
		__asm volatile ("nop");
}

void Chino::Threading::BSPYield()
{
    clint_ipi_send(0);
}

size_t Chino::Threading::BSPMsToTicks(size_t ms)
{
    return configTICK_RATE_HZ * ms / 1000;
}

static void SetNextTimerInterrupt(void)
{
    clint->mtimecmp[0] += (configTICK_CLOCK_HZ / configTICK_RATE_HZ);
}

extern "C" void SysTick_Handler()
{
	ArchDisableInterrupt();

	if (Kernel_IncrementTick())
		BSPYield();

    SetNextTimerInterrupt();
	ArchEnableInterrupt();
}
