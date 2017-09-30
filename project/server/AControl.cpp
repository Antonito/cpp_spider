/**
 * Project Untitled
 */

#include "AControl.h"

/**
 * AControl implementation
 */

namespace spider
{
namespace server
{

AControl::AControl(CommandCenter const &cmdCenter) : m_cmdCenter(cmdCenter), m_responseQueue(), m_commandQueue()
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
