#include "CommandInfo.h"

namespace spider
{
  namespace server
  {
    CommandInfo::CommandInfo(std::string const &_name,
                             std::string const &_description)
        : name(_name), description(_description)
    {
    }
  }
}
