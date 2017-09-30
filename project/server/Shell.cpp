#include <algorithm>
#include "Shell.h"
#include "Logger.hpp"

namespace spider
{
namespace shell
{
Shell::Shell(server::CommandCenter const &cmdCenter, volatile bool const &running) : AControl(cmdCenter, running)
{
    nope::log::Log(Info) << "Creating Shell control";
}

bool Shell::pollEvent(server::Event &ev)
{
    if (!m_commandQueue.empty()) {
        ev = m_commandQueue.front();
        m_commandQueue.pop();
        return true;
    }
    return false;
}

void Shell::sendResponse(server::Event const &ev)
{
    m_responseQueue.push(ev);
}

void Shell::sendEvent(server::Event &ev)
{
    m_commandQueue.push(ev);
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
        else if (std::find_if(m_commands.begin(), m_commands.end(), [&](server::CommandInfo const & m){ return m.name == line; }) != m_commands.end()) {
            // TODO: Allow to select client
            server::Event ev;

            ev.destId = 0;
            ev.emitter = this;
            ev.commandName = line;

            sendEvent(ev);
        }

    } while (m_running && std::cin);

    nope::log::Log(Info) << "Stopping Shell control";
}

void Shell::displayHelpMessage() const
{
    std::cout << "help - Display this message";
    for (auto const &cur : m_commands)
    {
        std::cout << '\n'
                  << cur.name << " - " << cur.description;
    }
    std::cout << std::endl;
}
}
}