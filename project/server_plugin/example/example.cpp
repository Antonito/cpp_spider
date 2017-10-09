#if defined _WIN32
#include <windows.h>
#endif

#include "IPlugin.h"

class ExamplePlugin : public spider::server::IPlugin
{
public:
  ExamplePlugin()
      : m_name("PluginExample"), m_description("An example of plugin")
  {
  }

  virtual ~ExamplePlugin() = default;

  virtual std::string const &getName() const
  {
    return m_name;
  }

  virtual std::string const &getDescription() const
  {
    return m_description;
  }

  virtual void command(spider::server::IClient *, void const *)
  {
  }

private:
  std::string const m_name;
  std::string const m_description;
};

static ExamplePlugin plugin;

extern "C" {

SPIDER_API spider::server::IPlugin *getPlugin()
{
  return static_cast<spider::server::IPlugin *>(&plugin);
}

#if defined _WIN32
BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
  return (true);
}
#endif
}