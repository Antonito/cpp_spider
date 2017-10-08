#pragma once

#include "Packet.h"

namespace spider
{
namespace client
{

enum class SystemMsgType : std::uint32_t
{
    EventKeyboard = 1,
    EventMouse,
    Data,
    Infos
};

enum class SystemMsgEventState : std::uint8_t
{
    Down,
    Up
};

struct SystemMsg
{
    union {
        std::uint32_t valid; // is 0 if non valid
        struct
        {
            SystemMsgType type;
            network::tcp::PathArray currentWindow;
            network::tcp::MacAddrArray mac;
            std::uint64_t time;
            union {
                struct
                {
                    std::uint32_t key;
                    std::uint32_t posX;
                    std::uint32_t posY;
                    SystemMsgEventState state;
                    std::uint8_t upper;
                } event;
                struct
                {
                    std::uint32_t size;
                    std::uint8_t const *raw;
                } data;
            };
        };
    };
};
}
}