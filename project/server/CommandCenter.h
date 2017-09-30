#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include "Client.h"
#include "CommandInfo.h"
#include "Client.h"
#include "Event.h"
#include "GenLibrary.h"
#include "IControl.h"

namespace spider
{
namespace server
{
class CommandCenter final
{
public:
  explicit CommandCenter(std::string const &pluginDirectory);
  ~CommandCenter() = default;

  CommandCenter(CommandCenter const &) = delete;
  CommandCenter(CommandCenter &&) = delete;
  CommandCenter &operator=(CommandCenter const &) = delete;
  CommandCenter &operator=(CommandCenter &&) = delete;

  std::vector<CommandInfo> const &getCommand() const;
  void execCommand(Client const &client, Event const &ev) const;

private:
  std::vector<CommandInfo> m_infos;
  std::vector<GenLibrary> m_plugins;

  // TODO: IControl * not void *
  std::unordered_map<std::string, std::function<void(IClient const *, void const *)>> m_action;
};
}
}
