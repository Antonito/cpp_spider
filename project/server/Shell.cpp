#include <algorithm>
#include "Shell.h"
#include "Logger.hpp"

namespace spider
{
  namespace shell
  {
    Shell::Shell(
        server::CommandCenter const &cmdCenter, volatile bool const &running,
        std::vector<std::unique_ptr<::spider::server::Client>> const &clients)
        : AControl(cmdCenter, running, clients)
    {
      nope::log::Log(Info) << "Creating Shell control";
    }

    bool Shell::pollEvent(server::Event &ev)
    {
      return (AControl::pollEvent(ev));
    }

    void Shell::sendEvent(server::Event &ev)
    {
      AControl::sendEvent(ev);
    }

    void Shell::sendToSpider(server::Event &ev)
    {
      AControl::sendToSpider(ev);
    }

    void Shell::run()
    {
      nope::log::Log(Info) << "Starting Shell control";
      bool stdinOpen = true;
      do
	{
	  std::cout << "> ";
	  std::string line;
	  std::getline(std::cin, line);

	  if (line == "help")
	    {
	      displayHelpMessage();
	    }
	  else if (std::find_if(m_commands.begin(), m_commands.end(),
	                        [&](server::CommandInfo const &m) {
	                          return m.name == line;
	                        }) != m_commands.end())
	    {
	      // TODO: Allow to select client
	      server::Event ev;

	      ev.destId = 0;
	      ev.emitter = this;
	      ev.commandName = line;

	      sendToSpider(ev);
	    }
	}
      while (m_running && std::cin);

      nope::log::Log(Info) << "Stopping Shell control";
    }

    void Shell::displayHelpMessage() const
    {
      std::cout << "help - Display this message";
      for (auto const &cur : m_commands)
	{
	  std::cout << '\n' << cur.name << " - " << cur.description;
	}
      std::cout << std::endl;
    }
  }
}
