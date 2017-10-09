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

      PacketHeader header;
      union
      {
	PacketEvent ev;
	PacketMov   mov;
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

    private:
      mt::Queue<EventStorage> m_storage;
    };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}