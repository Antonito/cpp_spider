#pragma once

#if defined __clang__
#pragma clang diagnostic push
#pragma clang system_header
#elif defined __GNUC__
#pragma gcc system_header
#endif
#if defined MONGOCXX
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <vector>
#endif
#if defined __clang__
#pragma clang diagnostic pop
#endif
#include <fstream>
#include "Queue.h"
#include "Packet.h"

namespace spider
{
  namespace server
  {

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

    struct EventStorage
    {
      EventStorage();
      ~EventStorage();

      EventStorage(EventStorage const &) noexcept;
      EventStorage &operator=(EventStorage const &) noexcept;

      network::tcp::PacketHeader header;
      union
      {
	network::tcp::PacketEvent ev;
	network::tcp::PacketMov   mov;
	network::tcp::PacketInfos infos;
      };
    };

    class Storage
    {
    public:
      Storage();
      ~Storage();

      Storage(Storage const &) = delete;
      Storage(Storage &&) = delete;

      Storage &operator=(Storage const &) = delete;
      Storage &operator=(Storage &&) = delete;

      void push(EventStorage const &ev);
      void write();

    private:
      mt::Queue<EventStorage> m_storage;
      std::ofstream           m_logFile;
#if defined MONGOCXX
      mongocxx::instance      m_mongoInstance;
      mongocxx::client        m_mongoClient;
      mongocxx::database      m_mongoDb;
#endif

      std::string timeToString(std::time_t const rawtime) const;
    };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
