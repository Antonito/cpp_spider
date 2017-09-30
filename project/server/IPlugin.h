#pragma once

#include <string>

namespace spider
{
namespace server
{

class IClient;
class IControl;

class IPlugin
{
public:
  virtual ~IPlugin() = default;

  // Return the name of the plugin
  virtual std::string const &getName() const = 0;
  // Return the description of the plugin
  virtual std::string const &getDescription() const = 0;
  // Execute the plugin action
  // TODO: IControl * not void *
  virtual void command(IClient const *, void const *) = 0;
};
}
}