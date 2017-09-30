#pragma once

#include <string>

namespace spider
{
namespace server
{
struct CommandInfo final
{
    CommandInfo(std::string const &_name, std::string const &_description);
    std::string name;
    std::string description;
};
}
}