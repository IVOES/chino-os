#include <kernel/kernel_iface.h>
#include <uarths.h>
#include <string.h>

static void init_bss(void)
{
    extern unsigned char __bss_start[];
    extern unsigned char __bss_end[];

    memset(__bss_start, 0, __bss_end - __bss_start);
}

void BSPEntry()
{
    init_bss();
	Kernel_Main(NULL);
}
