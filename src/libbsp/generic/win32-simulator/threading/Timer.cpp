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

enum
{
	S = sizeof(CONTEXT)
};

static_assert(sizeof(ThreadContext_Arch::context) == sizeof(CONTEXT), "context size error.");

static std::atomic<bool> _switchQueued = false;
static HANDLE _timerThread, _timer, _workerThread, _wfiEvent;

static CONTEXT& GetContext(ThreadContext_Arch* ctx)
{
	return reinterpret_cast<CONTEXT&>(ctx->context);
}

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

static std::atomic<bool> _exitKernel;

static void WaitForExitKernelMode()
{
	_exitKernel.store(true, std::memory_order_release);
	while (1);
}

static std::mutex mutex;

static void ArchSwitchContext(ULONG_PTR)
{
	std::lock_guard<std::mutex> locker(mutex);
	CONTEXT hostCtx = { 0 };
	hostCtx.ContextFlags = CONTEXT_ALL;

	{
		_exitKernel.store(false, std::memory_order_release);
		SuspendThread(_workerThread);
		GetThreadContext(_workerThread, &hostCtx);
		auto newCtx = hostCtx;
		newCtx.Rip = DWORD64(WaitForExitKernelMode);
		SetThreadContext(_workerThread, &newCtx);
		ResumeThread(_workerThread);
		while (!_exitKernel.load(std::memory_order_acquire));
	}

	{
		InterruptService is;
		auto ctx = reinterpret_cast<ThreadContext_Arch*>(g_CurrentThreadContext);

		// Save
		if (ctx)
			GetContext(ctx) = hostCtx;

		// Restore
		Kernel_SwitchThreadContext();
		ctx = reinterpret_cast<ThreadContext_Arch*>(g_CurrentThreadContext);
		auto& newCtx = GetContext(ctx);
		if (newCtx.ContextFlags == 0)
		{
			hostCtx.Rcx = newCtx.Rcx;
			hostCtx.Rip = newCtx.Rip;
			hostCtx.Rsp = newCtx.Rsp;
		}
		else
		{
			hostCtx = GetContext(ctx);
		}

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
		auto& ctx = GetContext(context);
		ctx.Rcx = parameter;
		ctx.Rip = entryPoint;

		--stack;
		*--stack = returnAddress;
		ctx.Rsp = uintptr_t(stack);
	}

	bool ArchValidateThreadContext(ThreadContext_Arch* context, uintptr_t stackTop, uintptr_t stackBottom)
	{
		auto& ctx = GetContext(context);
		return ctx.Rsp <= stackTop && ctx.Rsp >= stackBottom;
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
