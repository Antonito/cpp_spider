#pragma once

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

      std::string timeToString(std::time_t const rawtime) const;
    };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
