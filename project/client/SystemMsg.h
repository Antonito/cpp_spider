#pragma once

#include "Packet.h"

namespace spider
{
namespace client
{

enum class SystemMsgType : std::uint32_t
{
    EventKeyboard,
    EventMouse,
    Infos
};

enum class SystemMsgEventState : std::uint8_t
{
    Down,
    Up
};

struct SystemMsg
{
    SystemMsgType type;
    network::udp::PathArray currentWindow;
    union {
        struct
        {
            std::uint32_t key;
            std::uint32_t posX;
            std::uint32_t posY;
            SystemMsgEventState state;
        } event;
        struct
        {
            std::uint32_t size;
            std::uint8_t *raw;
        } data;
    };
};
}
}