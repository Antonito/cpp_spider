#pragma once

#include <cstdint>

namespace spider
{
  namespace client
  {
    namespace library
    {
      enum class MouseButton : std::uint32_t
      {
	None,
	ButtonLeft,
	ButtonRight
      };

      enum class KeyboardKey : std::uint32_t
      {
	KB_A = 0,            // A key
	KB_B,                // B key
	KB_C,                // C key
	KB_D,                // D key
	KB_E,                // E key
	KB_F,                // F key
	KB_G,                // G key
	KB_H,                // H key
	KB_I,                // I key
	KB_J,                // J key
	KB_K,                // K key
	KB_L,                // L key
	KB_M,                // M key
	KB_N,                // N key
	KB_O,                // O key
	KB_P,                // P key
	KB_Q,                // Q key
	KB_R,                // R key
	KB_S,                // S key
	KB_T,                // T key
	KB_U,                // U key
	KB_V,                // V key
	KB_W,                // W key
	KB_X,                // X key
	KB_Y,                // Y key
	KB_Z,                // Z key
	KB_0,                // 0 key
	KB_1,                // 1 key
	KB_2,                // 2 key
	KB_3,                // 3 key
	KB_4,                // 4 key
	KB_5,                // 5 key
	KB_6,                // 6 key
	KB_7,                // 7 key
	KB_8,                // 8 key
	KB_9,                // 9 key
	KB_ARROW_LEFT,       // Left arrow key
	KB_ARROW_RIGHT,      // Right arrow key
	KB_ARROW_UP,         // Up arrow key
	KB_ARROW_DOWN,       // Down arrow key
	KB_SPACE,            // Space key
	KB_ENTER,            // Enter/Carriage return key
	KB_BACKSPACE,        // Backspace key
	KB_LCTRL,            // Left Control key
	KB_RCTRL,            // Right Control key
	KB_LALT,             // Left Alt key
	KB_RALT,             // Right Alt key
	KB_LSHIFT,           // Left Shift key
	KB_RSHIFT,           // Right Shift key
	KB_CAPSLOCK,         // CapsLock key
	KB_TAB,              // Tabulation key
	KB_ESCAPE,           // Escape key
	KB_PAGEUP,           // Page up key
	KB_PAGEDOWN,         // Page down
	KB_HOME,             // Home key
	KB_END,              // End key
	KB_FN1,              // Function key 1 (F1)
	KB_FN2,              // Function key 2 (F2)
	KB_FN3,              // Function key 3 (F3)
	KB_FN4,              // Function key 4 (F4)
	KB_FN5,              // Function key 5 (F5)
	KB_FN6,              // Function key 6 (F6)
	KB_FN7,              // Function key 7 (F7)
	KB_FN8,              // Function key 8 (F8)
	KB_FN9,              // Function key 9 (F9)
	KB_FN10,             // Function key 10 (F10)
	KB_FN11,             // Function key 11 (F11)
	KB_FN12,             // Function key 12 (F12)
	KB_FN13,             // Function key 13 (F13)
	KB_FN14,             // Function key 14 (F14)
	KB_FN15,             // Function key 15 (F15)
	KB_FN16,             // Function key 16 (F16)
	KB_FN17,             // Function key 17 (F17)
	KB_FN18,             // Function key 18 (F18)
	KB_FN19,             // Function key 19 (F19)
	KB_FN20,             // Function key 20 (F20)
	KB_FN21,             // Function key 21 (F21)
	KB_FN22,             // Function key 22 (F22)
	KB_FN23,             // Function key 23 (F23)
	KB_FN24,             // Function key 24 (F24)
	KB_COMMA,            // Comma key (,)
	KB_DOT,              // Dot (period) key (.)
	KB_SLASH,            // Slash key (/)
	KB_INFERIOR,         // Inferior symbol key (<)
	KB_SUPERIOR,         // Superior symbol key (>)
	KB_QUESTION,         // Question mark key (?)
	KB_SEMICOLON,        // Semicolon key (;)
	KB_COLON,            // Colon key (:)
	KB_BACKQUOTE,        // Back quote key (`)
	KB_SIMPLEQUOTE,      // Simple quote key (')
	KB_DOUBLEQUOTE,      // Double quote key (")
	KB_LEFTBRACE,        // Left brace key ({)
	KB_RIGHTBRACE,       // Right brace key (})
	KB_LEFTBRACKET,      // Left bracket key ([)
	KB_RIGHTBRACKET,     // Right bracker key (])
	KB_LEFTPAREN,        // Left parenthesis key (()
	KB_RIGHTPAREN,       // Right parenthesis key ())
	KB_BACKSLASH,        // Backslash key (\)
	KB_VERTICALBAR,      // Vertical bar key (|)
	KB_EXCLAMATION,      // Exclamation mark key (!)
	KB_ATSYMBOL,         // At symbol key (@)
	KB_HASHTAG,          // Hash key (#)
	KB_DOLLAR,           // Dollar key ($)
	KB_PERCENT,          // Percent symbol key (%)
	KB_CIRCUMFLEX,       // Circumflex symbol key (^)
	KB_AMPERSAND,        // Ampersand key (&)
	KB_ASTERISK,         // Asterisk key (*)
	KB_TILDE,            // Tilde key (~)
	KB_UNDERSCORE,       // Underscore key (_)
	KB_MINUS,            // Minus symbol key (-)
	KB_PLUS,             // Plus symbol key (+)
	KB_EQUALS,           // Equals symbol key (=)
	KB_DELETE,           // Delete key (del)
	KB_NUMLOCK,          // Numlock key
	KB_SCROLL,           // Scroll lock key
	KB_LCMD,             // Left command key (Windows on most keyboard)
	KB_RCMD,             // Right command key (Windows on most keyboard)
	NB_KEYBOARD_KEY,     // Number of keyboard keys
	KB_NONE = 0xFFFFFFFF // Unkwown key
      };
    }
  }
}
