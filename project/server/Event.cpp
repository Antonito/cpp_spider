#include "Event.h"

namespace spider
{
  namespace server
  {
    Event::Event()
        : dest(nullptr), emitter(nullptr), commandName(""), askId(0),
          response()
    {
    }

    Event::Event(Event const &other)
        : dest(other.dest), emitter(other.emitter),
          commandName(other.commandName), askId(other.askId),
          response(other.response)
    {
    }

    Event &Event::operator=(Event const &other)
    {
      if (this != &other)
	{
	  dest = other.dest;
	  emitter = other.emitter;
	  commandName = other.commandName;
	  askId = other.askId;
	  response = other.response;
	}
      return (*this);
    }
  }
}

