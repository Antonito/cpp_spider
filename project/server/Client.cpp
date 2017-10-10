#include "Client.h"

namespace spider
{
  namespace server
  {

    constexpr std::size_t Client::maxLength;

    Client::Client(sock_t const sock, CommandCenter const &cmdCenter,
                   std::size_t const ndx)
        : m_os(""), m_ip(""), m_geo(""), m_pcName(""), m_commandQueue(),
          m_responseQueue(), m_cmdCenter(cmdCenter), m_socket(sock),
          m_socketData(-1), m_id(static_cast<std::uint16_t>(ndx)),
          m_canWrite(false), m_outputQueue(), m_receiveBuffer(), m_macAddr{}
    {
    }

    void Client::setDataSocket(sock_t const sock)
    {
      m_socketData = sock;
    }

    bool Client::canWrite() const
    {
      return m_canWrite;
    }

    void Client::toggleWrite()
    {
      m_canWrite = !m_canWrite;
    }

    bool Client::operator==(network::tcp::MacAddrArray const &arr) const
    {
      return arr == m_macAddr;
    }

    ::network::IClient::ClientAction Client::treatIncomingData()
    {
      auto ret = ::network::IClient::ClientAction::FAILURE;

      // TODO: treat values
      std::queue<std::string> resp;
      ret = readFromNetwork(resp);
      while (!resp.empty())
	{
	  // TODO: this is is for the Data socket
	  if (!m_responseQueue.empty())
	    {
	      Event ev = m_responseQueue.front();
	      m_responseQueue.pop();
	      ev.response.setResponse(resp.front());
	      //ev.commandName = "commandInfo";
	      ev.emitter->sendEvent(ev);
	    }
	  nope::log::Log(Info) << "Read: [" << m_id << "] " << resp.front();
	  resp.pop();
	}
      return ret;
    }

    ::network::IClient::ClientAction Client::treatOutgoingData()
    {
      auto ret = ::network::IClient::ClientAction::FAILURE;

      if (!m_outputQueue.empty())
	{
	  nope::log::Log(Info) << "Writing data to client";
	  ret = sendNetwork(m_outputQueue.front());
	  m_outputQueue.pop();
	}
      if (ret == ::network::IClient::ClientAction::SUCCESS)
	{
	  toggleWrite();
	}

      return ret;
    }

    ::network::IClient::ClientAction
        Client::readFromNetwork(std::queue<std::string> &str)
    {
      auto ret = ::network::IClient::ClientAction::FAILURE;
      std::array<char, 0x1000> data;
      ssize_t buffLen = 0;

      if (m_socket.rec(data.data(), sizeof(data), &buffLen))
	{
	  if (!buffLen)
	    {
	      return ::network::IClient::ClientAction::DISCONNECT;
	    }
	  ret = ::network::IClient::ClientAction::SUCCESS;
	  m_receiveBuffer.write(
	      reinterpret_cast<std::uint8_t const *>(data.data()),
	      static_cast<std::size_t>(buffLen));

	  size_t cmdLen = 0;
	  do
	    {
	      cmdLen = m_receiveBuffer.hasCommand();
	      if (cmdLen)
		{
		  data.fill(0);
		  m_receiveBuffer.read(
		      reinterpret_cast<std::uint8_t *>(data.data()), cmdLen);
		  str.push(std::string(data.data()));
		}
	    }
	  while (cmdLen);
	}

      return ret;
    }

    ::network::IClient::ClientAction
        Client::sendNetwork(std::string const &str)
    {
      auto ret = ::network::IClient::ClientAction::SUCCESS;

      if (m_socket.send(str.c_str(), str.length()) == false)
	{
	  nope::log::Log(Debug) << "Failed to write data [Client]";
	  ret = ::network::IClient::ClientAction::FAILURE;
	}
      return (ret);
    }

    bool Client::operator==(Client const &other) const
    {
      if (this != &other)
	{
	  return (m_socket == other.m_socket);
	}
      return (true);
    }

    std::uint16_t Client::getId() const
    {
      return m_id;
    }

    void Client::disconnect()
    {
      nope::log::Log(Debug) << "Client disconnected #" << getSocket();
      m_socket.closeConnection();
    }

    Client::~Client()
    {
      nope::log::Log(Warning) << "Disconnecting client";
    }

    sock_t Client::getSocket() const
    {
      return m_socket.getSocket();
    }

    void Client::send(std::string const &buffer)
    {
      m_outputQueue.push(buffer);
    }

    size_t Client::receive()
    {
      return 0;
    }

    void Client::execute()
    {
      if (!m_commandQueue.empty())
	{
	  Event ev = m_commandQueue.front();
	  m_commandQueue.pop();
	  m_responseQueue.push(ev);
	  m_cmdCenter.execCommand(*this, ev);
	}
    }

    void Client::sendEvent(Event &e)
    {
      m_commandQueue.push(e);
      m_canWrite = true;
    }

    void Client::eventManager()
    {
    }

    std::string const &Client::getOS() const
    {
      return (m_os);
    }

    std::string const &Client::getIP() const
    {
      return (m_ip);
    }

    std::string const &Client::getGeo() const
    {
      return (m_geo);
    }

    std::string const &Client::getName() const
    {
      return (m_pcName);
    }

    void Client::setOS(std::string os)
    {
      m_os = os;
    }

    void Client::setIP(std::string ip)
    {
      m_ip = ip;
    }

    void Client::setGeo(std::string geo)
    {
      m_geo = geo;
    }

    void Client::setName(std::string pcName)
    {
      m_pcName = pcName;
    }
  }
}
