//
// Chino Thread
//
#include <libbsp/bsp.hpp>
#include <libarch/arch.h>
#include <Windows.h>
#include <process.h>
#include <mutex>

using namespace Chino::Threading;

#define portINITIAL_RFLAGS	0x206u

#define configTICK_RATE_HZ 10

static std::atomic<bool> _switchQueued = false;
static HANDLE _timerThread, _timer, _workerThread, _wfiEvent;

static void ArchQueueContextSwitch();

static void TimerThreadMain(void*)
{
	_timer = CreateWaitableTimer(nullptr, FALSE, nullptr);
	ArchEnableInterrupt();
	while (true)
		SleepEx(INFINITE, TRUE);
}

void Chino::Threading::BSPSetupSchedulerTimer()
{
	_wfiEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &_workerThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
	_timerThread = (HANDLE)_beginthread(TimerThreadMain, 1024, nullptr);

	while (1);
}

void Chino::Threading::BSPSleepMs(uint32_t ms)
{
	Sleep(ms);
}

void Chino::Threading::BSPYield()
{
	ArchQueueContextSwitch();
}

size_t Chino::Threading::BSPMsToTicks(size_t ms)
{
	return configTICK_RATE_HZ * ms / 1000;
}

struct InterruptService
{
	InterruptService()
	{
		SuspendThread(_workerThread);
		SetEvent(_wfiEvent);
	}

	~InterruptService()
	{
		ResumeThread(_workerThread);
	}
};

extern "C" void ArchSwitchContextCore();

static std::mutex mutex;

static void ArchSwitchContext(ULONG_PTR)
{
	std::lock_guard<std::mutex> locker(mutex);

	{
		InterruptService is;
		CONTEXT hostCtx = { 0 };
		hostCtx.ContextFlags = CONTEXT_CONTROL;
		GetThreadContext(_workerThread, &hostCtx);

		auto stack = reinterpret_cast<uintptr_t*>(hostCtx.Rsp -= sizeof(uintptr_t));
		stack[0] = uintptr_t(hostCtx.Rip);
		hostCtx.Rip = uintptr_t(ArchSwitchContextCore);
		SetThreadContext(_workerThread, &hostCtx);
		_switchQueued.store(false, std::memory_order_release);
	}
}

static void ArchQueueContextSwitch()
{
	bool exp = false;
	if (_switchQueued.compare_exchange_strong(exp, true, std::memory_order_acq_rel))
		QueueUserAPC(ArchSwitchContext, _timerThread, 0);
}

static void SchedulerTimerCallback(LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	InterruptService is;
	if (Kernel_IncrementTick())
		ArchQueueContextSwitch();
}

extern "C"
{
	void ArchInitializeThreadContextArch(ThreadContext_Arch* context, uintptr_t stackPointer, uintptr_t entryPoint, uintptr_t returnAddress, uintptr_t parameter)
	{
		auto stack = reinterpret_cast<uint64_t*>(stackPointer);
		context->rcx = parameter;
		context->rip = entryPoint;
		context->rflags = 0;

		--stack;
		*--stack = returnAddress;
		context->rsp = uintptr_t(stack);
	}

	bool ArchValidateThreadContext(ThreadContext_Arch* context, uintptr_t stackTop, uintptr_t stackBottom)
	{
		return context->rsp <= stackTop && context->rsp >= stackBottom;
	}

	void ArchDisableInterrupt()
	{
		CancelWaitableTimer(_timer);
	}

	void ArchEnableInterrupt()
	{
		LARGE_INTEGER dueTime;
		dueTime.QuadPart = 0;
		SetWaitableTimer(_timer, &dueTime, 1000 / configTICK_RATE_HZ, SchedulerTimerCallback, nullptr, FALSE);
	}

	void ArchHaltProcessor()
	{
		while (WaitForSingleObject(_wfiEvent, 10) != WAIT_OBJECT_0)
			Sleep(0);
	}
}
