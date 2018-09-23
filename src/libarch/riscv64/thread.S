#define REGBYTES 8

.global ArchHaltProcessor
.global ArchEnableInterrupt
.global ArchDisableInterrupt
.global ArchRestoreThreadContextArch
.global PendSV_Handler

.section .text

.align 4
ArchHaltProcessor:
    wfi
	ret
	
.align 4
ArchEnableInterrupt:
    csrs mstatus, 8
	ret
	
.align 4
ArchDisableInterrupt:
    csrc mstatus, 8
	ret
	
  .type  PendSV_Handler, %function
PendSV_Handler:
	ret
restore:
	ret
