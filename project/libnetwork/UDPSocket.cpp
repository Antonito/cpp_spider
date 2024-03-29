#include "network_stdafx.hpp"

namespace network
{
#if defined LIBNETWORK_HAS_SSL
  UDPSocket::UDPSocket(sock_t const sock, SSL_CTX *ctx) : ASocket(sock, ctx)
#else
  UDPSocket::UDPSocket(sock_t const sock) : ASocket(sock)
#endif
  {
  }

  UDPSocket::UDPSocket(std::uint16_t port, std::string const &host, bool ip,
                       ASocket::SocketType type)
      : ASocket(port, host, type)
  {
    m_ip = ip;
  }

  UDPSocket::UDPSocket(std::uint16_t port, std::uint32_t maxClients,
                       ASocket::SocketType type)
      : ASocket(port, maxClients, type)
  {
  }

  UDPSocket::UDPSocket(UDPSocket &&other) : ASocket(std::move(other))
  {
  }

  UDPSocket::~UDPSocket()
  {
  }

  bool UDPSocket::send(void const *, std::size_t) const
  {
    // Not implemented
    return (false);
  }

  bool UDPSocket::send(void const *data, std::size_t len,
                       sockaddr_t const *dest, socklen_t destLen) const
  {
    ssize_t ret;

    assert(isStarted());
    do
      {
#if defined(_WIN32)
	ret = sendto(m_socket, reinterpret_cast<const char *>(data),
	             static_cast<std::int32_t>(len), 0, dest,
	             static_cast<std::int32_t>(destLen));
#else
	ret = sendto(m_socket, data, len, 0, dest, destLen);
#endif // !_WIN32
      }
    while (ret == -1 && errno == EINTR);
    return (ret != -1);
  }

  bool UDPSocket::rec(void *, std::size_t, ssize_t *) const
  {
    // Not implemented
    return (false);
  }

  bool UDPSocket::rec(void *buffer, std::size_t rlen, sockaddr_t *addr,
                      socklen_t *addrLen) const
  {
    ssize_t ret;

    assert(isStarted());
    do
      {
#if defined(_WIN32)
	ret = recvfrom(m_socket, reinterpret_cast<char *>(buffer),
	               static_cast<std::int32_t>(rlen), 0, addr, addrLen);
#else
	ret = recvfrom(m_socket, buffer, rlen, 0, addr, addrLen);
#endif // !_WIN32
      }
    while (ret == -1 && errno == EINTR);
    return (ret != -1);
  }

#if defined LIBNETWORK_HAS_SSL
  bool UDPSocket::openConnection(std::string const &key,
                                 std::string const &cert)
#else
  bool UDPSocket::openConnection()
#endif
  {
    bool ret;

    nope::log::Log(Debug) << "Opening UDP connection [ " << m_port << " ]";
    assert(!isStarted());
    if (getMode() == ASocket::SERVER)
      {
	nope::log::Log(Debug) << "Hosting connection";
	ret = true;
	try
	  {
#if defined LIBNETWORK_HAS_SSL
	    initSocket(AF_INET, SOCK_DGRAM, 0, key, cert);
#else
	    initSocket(AF_INET, SOCK_DGRAM, 0);
#endif
	    nope::log::Log(Debug) << "Socket init'd.";
	    m_addr.sin_port = htons(m_port);
	    m_addr.sin_family = AF_INET;
	    hostConnection();
	  }
	catch (std::exception &e)
	  {
	    nope::log::Log(Error) << e.what();
	    ret = false;
	  }
      }
    else
      {
#if defined LIBNETWORK_HAS_SSL
	ret = connectToHost(SOCK_DGRAM, IPPROTO_UDP, false, key, cert);
#else
	ret = connectToHost(SOCK_DGRAM, IPPROTO_UDP, false);
#endif
      }
    if (ret == false)
      {
	if (m_socket > 0)
	  closesocket(m_socket);
	m_socket = -1;
      }
    return (ret && isStarted());
  }
}
