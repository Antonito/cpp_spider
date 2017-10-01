#include "AntiDbg.h"

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

#include <iostream>

namespace spider
{
namespace misc
{
bool Debugger::isBeingAV()
{
#if _WIN32
    // Check if syscalls are being intercepted
    HINSTANCE dll = LoadLibrary(TEXT("fake.dll"));
    if (dll)
    {
        return true;
    }
    int Tick = GetTickCount();
    Sleep(1000);
    int Tac = GetTickCount();
    if ((Tac - Tick) < 1000)
    {
        return true;
    }
#endif
    return false;
}

bool Debugger::isDebuggerPresent()
{
#if _MSC_VER && !__INTEL_COMPILER
    char underDebugger = 0;

    // Check software debugger
    __asm {
     mov eax, fs:[30h]
     mov al, [eax + 2h]
     mov underDebugger, al
    }
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