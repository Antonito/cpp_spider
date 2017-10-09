#pragma once

#include "GenLibrary.h"
#include "Queue.h"
#include "SystemMsg.h"
#include "IPayload.h"

namespace spider
{
  namespace client
  {
    namespace core
    {
      class Core final
      {
      public:
	// The folder where the foo dynamic libraries are located
	explicit Core(std::string const &path);
	~Core();

	void init();

	Core(Core const &) = delete;
	Core(Core &&) = delete;
	Core &operator=(Core const &) = delete;
	Core &operator=(Core &&) = delete;

	int run();

	mt::Queue<SystemMsg> &               getSendToNetwork();
	mt::Queue<library::IPayload::Order> &getReceivedFromNetwork();
	mt::Queue<std::string> &             getResponseQueue() const;

      private:
	GenLibrary                           m_lib;
	library::IPayload *                  m_payload;
	mt::Queue<SystemMsg>                 m_sendToNetwork;
	mt::Queue<library::IPayload::Order> *m_receivedFromNetwork;
      };
    }
  }
}
