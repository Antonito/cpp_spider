#ifndef _WIN32
#include <sys/types.h>
#include <sys/ptrace.h>
#else
#include <windows.h>
#endif

class Debugger
{
  public:
    static bool isBeingAV()
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
#else
        return false;
#endif
    }

    static bool isDebuggerPresent()
    {
#if _MSC_VER && !__INTEL_COMPILER
        bool bExceptionHit = true;
        __try
        {
            __asm {
            PUSH EAX // Save the EAX value to stack
            NOP
            MOV EAX, [FS:0x30] // Get PEB structure address
            NOP
            NOP
            MOV EAX, [EAX+0x02] // Get being debugged byte
            TEST EAX, EAX // Check if being debuged byte is set
            JNE CheckDebugger // If debugger present check again
            POP EAX // Put back the EAX value
            NOP
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            bExceptionHit = false;
        }
        return bExceptionHit;
#else
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
#endif
    }
}