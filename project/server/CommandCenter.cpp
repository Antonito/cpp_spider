#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include "CommandCenter.h"
#include "IPlugin.h"
#include "Logger.hpp"

namespace spider
{
namespace server
{
CommandCenter::CommandCenter(std::string const &pluginDirectory) : m_infos(), m_plugins(), m_action{}
{
    // For each file in directory
    boost::filesystem::path targetDir(pluginDirectory);
    boost::filesystem::directory_iterator it(targetDir), eod;

    BOOST_FOREACH (boost::filesystem::path const &p, std::make_pair(it, eod))
    {
        if (boost::filesystem::is_regular_file(p))
        {
            nope::log::Log(Info) << "Loading " << p;
            m_plugins.push_back(GenLibrary());

            GenLibrary &currentLib = m_plugins.back();
            currentLib.load(p.string());

            auto getPlugin = currentLib.getFunction<IPlugin *()>("getPlugin");
            IPlugin *plugin = getPlugin();

            m_infos.push_back(CommandInfo(plugin->getName(), plugin->getDescription()));
            nope::log::Log(Info) << "Plugin: " << plugin->getName() << " - " << plugin->getDescription();
            m_action[plugin->getName()] = [plugin](IClient const *client, void const *ctrl) { plugin->command(client, ctrl); };
        }
    }
    nope::log::Log(Info) << "Command Center initialized. Loadded " << m_infos.size() << " plugins";
}

std::vector<CommandInfo> const &CommandCenter::getCommand() const
{
    return std::vector<CommandInfo>();
}

void CommandCenter::execCommand(Client const &client, Event const &ev) const
{
    m_action.at(ev.commandName)(&client, ev.emitter);
}
}
}