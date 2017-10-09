#pragma once

#include <string>

#if defined(__linux__) || (__APPLE__)
#define SPIDER_API
#else
#define SPIDER_API __declspec(dllexport)
#endif

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
      // return the JSON for webPanel
      virtual std::string const getJSON(std::string const &res) const = 0;
      // Execute the plugin action
      // TODO: IControl * not void *
      virtual void command(IClient *, void const *) = 0;
    };
  }
}
