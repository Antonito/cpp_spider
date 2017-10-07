#include "AntiDbg.h"
#include "GenLibrary.h"

#ifndef _WIN32
#include <sys/types.h>
#include <sys/ptrace.h>
#if defined __APPLE__
#include <mach/task.h>
#include <mach/mach_init.h>
#include <stdbool.h>
#endif
#else
#include <windows.h>
#endif

#include <thread>
#include <chrono>
#include <iostream>

namespace spider
{
namespace misc
{
bool Debugger::isBeingAV()
{
    // Check if being analyzed
    bool isBeingAnalyzed = true;

    try
    {
        // Try to load an non-existing library
        // If the loading is successful, it means we are being analyzed
        GenLibrary fakeLib(
#if defined _WIN32
            "./fake.dll"
#elif defined __APPLE__
            "./fake.dylib"
#else
            "./fake.so"
#endif
            );
    }
    catch (...)
    {
        isBeingAnalyzed = false;
    }
    if (isBeingAnalyzed)
    {
        return true;
    }

    // Check if we're in an sandbox
    using namespace std::chrono_literals;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(2s);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() < std::chrono::duration_cast<std::chrono::microseconds>(2s).count())
    {
        return true;
    }
    return false;
}

bool Debugger::isDebuggerPresent()
{
#if _MSC_VER && !__INTEL_COMPILER
    char underDebugger = 0;

    // Check software debugger
#ifndef _WIN64
    __asm {
     xor eax, eax
     mov eax, fs:[0x30]
     mov al, [eax + 0x02]
     nop;
     nop;
     mov underDebugger, al
    }
#else
	underDebugger = IsDebuggerPresent();
#endif
    if (underDebugger)
    {
        return true;
    }

    // Check hardware breakpoints
    CONTEXT ctx;
    std::memset(&ctx, 0, sizeof(CONTEXT));
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    // Get a handle to our thread
    HANDLE hThread = GetCurrentThread();

    // Get the registers
    if (GetThreadContext(hThread, &ctx) == 0)
    {
        return false;
    }

    // Now we can check for hardware breakpoints, its not
    // necessary to check Dr6 and Dr7, however feel free to
    if (ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3)
    {
        return true;
    }
    return false;

#elif __linux__
    int underDebugger = 0;
    static bool isCheckedAlready = false;
    if (!isCheckedAlready)
    {
        if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0)
        {
            underDebugger = 1;
        }
        else
        {
            ptrace(PTRACE_DETACH, 0, 1, 0);
        }
        isCheckedAlready = true;
    }
    return underDebugger == 1;
#else
    mach_msg_type_number_t count = 0;
    exception_mask_t masks[EXC_TYPES_COUNT];
    mach_port_t ports[EXC_TYPES_COUNT];
    exception_behavior_t behaviors[EXC_TYPES_COUNT];
    thread_state_flavor_t flavors[EXC_TYPES_COUNT];
    exception_mask_t mask = EXC_MASK_ALL & ~(EXC_MASK_RESOURCE | EXC_MASK_GUARD);

    kern_return_t result = task_get_exception_ports(mach_task_self(), mask, masks, &count, ports, behaviors, flavors);
    if (result == KERN_SUCCESS)
    {
        for (mach_msg_type_number_t portIndex = 0; portIndex < count; portIndex++)
        {
            if (MACH_PORT_VALID(ports[portIndex]))
            {
                return true;
            }
        }
    }
    return false;
#endif
}
}
}