#include "AControl.h"

namespace spider
{
namespace server
{

AControl::AControl(CommandCenter const &cmdCenter, volatile bool const &running) : m_cmdCenter(cmdCenter), m_responseQueue(), m_commandQueue(), m_commands(m_cmdCenter.getCommand()), m_running(running)
{
}

bool AControl::pollEvent(Event &ev)
{
}

void AControl::sendResponse(Event const &ev)
{
}
}
}
