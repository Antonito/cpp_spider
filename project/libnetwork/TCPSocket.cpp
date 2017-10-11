#include "network_stdafx.hpp"
#include <cerrno>
#include <cassert>
#include "TCPSocket.hpp"

namespace network
{
#if defined LIBNETWORK_HAS_SSL
  TCPSocket::TCPSocket(sock_t const sock, SSL_CTX *ctx) : ASocket(sock, ctx)
  {
  }

  TCPSocket::TCPSocket(sock_t const sock, SSL *sockSSL, SSL_CTX *ctx)
      : ASocket(sock, ctx)
  {
    if (!sockSSL)
      {
	throw std::runtime_error("Invalid SSL socket");
      }
    m_socketSSL = sockSSL;
    // Bind SSL socket and BSD socket
    SSL_set_fd(m_socketSSL, sock);

    // Do the harlem....handshake !
    if (SSL_accept(m_socketSSL) != 1)
      {
	throw std::runtime_error("SSL handshake failed");
      }
  }

#endif
  TCPSocket::TCPSocket(sock_t const sock) : ASocket(sock)
  {
  }

#if defined LIBNETWORK_HAS_SSL
  TCPSocket::TCPSocket(std::uint16_t port, std::string const &host, bool ip,
                       SocketType type, SSL_CTX *ctx)
      : ASocket(port, host, type, ctx)
#else
  TCPSocket::TCPSocket(std::uint16_t port, std::string const &host, bool ip,
                       SocketType type)
      : ASocket(port, host, type)
#endif
  {
    m_ip = ip;
  }

#if defined LIBNETWORK_HAS_SSL
  TCPSocket::TCPSocket(std::uint16_t port, std::uint32_t maxClients,
                       SocketType type, SSL_CTX *ctx)
      : ASocket(port, maxClients, type, ctx)
#else
  TCPSocket::TCPSocket(std::uint16_t port, std::uint32_t maxClients,
                       SocketType type)
      : ASocket(port, maxClients, type)
#endif
  {
  }

  TCPSocket::TCPSocket(TCPSocket &&other) : ASocket(std::move(other))
  {
  }

  TCPSocket::~TCPSocket()
  {
  }

  TCPSocket &TCPSocket::operator=(sock_t const sock)
  {
    ASocket::operator=(sock);
    return (*this);
  }

#if defined LIBNETWORK_HAS_SSL
  bool TCPSocket::openConnection(std::string const &key,
                                 std::string const &cert)
#else
  bool TCPSocket::openConnection()
#endif
  {
    bool ret;

    nope::log::Log(Debug) << "Opening TCP connection";
    assert(!isStarted());
    if (getMode() == ASocket::SERVER)
      {
	ret = true;
	try
	  {
#if defined LIBNETWORK_HAS_SSL
	    initSocket(AF_INET, SOCK_STREAM, 0, key, cert);
#else
	    initSocket(AF_INET, SOCK_STREAM, 0);
#endif
	    m_addr.sin_port = htons(m_port);
	    m_addr.sin_family = AF_INET;
	    hostConnection();
	    if (listen(m_socket, static_cast<std::int32_t>(m_maxClients)) ==
	        -1)
	      {
		throw std::runtime_error("Cannot listen on socket");
	      }
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
	ret = connectToHost(SOCK_STREAM, IPPROTO_TCP, true, key, cert);
#else
	ret = connectToHost(SOCK_STREAM, IPPROTO_TCP, true);
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

  bool TCPSocket::sendBlocking(void const *data, std::size_t len) const
  {
    ssize_t     ret;
    std::size_t off = 0;

    assert(getType() == ASocket::BLOCKING);
    do
      {
#if defined LIBNETWORK_HAS_SSL
	assert(m_socketSSL);
	ret = SSL_write(m_socketSSL, static_cast<char const *>(data) + off,
	                static_cast<std::int32_t>(len - off));
#else
#if defined(__linux__) || defined(__APPLE__)
	ret = ::send(m_socket, static_cast<char const *>(data) + off,
	             len - off, 0);
#elif defined(_WIN32)
	ret = ::send(m_socket, static_cast<char const *>(data) + off,
	             static_cast<std::int32_t>(len - off), 0);
#endif
#endif
	if (ret <= 0)
	  {
	    return (!ret);
	  }
	off += static_cast<std::size_t>(ret);
      }
    while (off != len);
    nope::log::Log(Debug) << "Sent " << ret << "/" << len;
    return (true);
  }

  bool TCPSocket::sendNonBlocking(void const *data, std::size_t len) const
  {
    std::uint8_t const *msg = static_cast<std::uint8_t const *>(data);
    std::size_t         off = 0;
    bool                success = true;

    assert(getType() == ASocket::NONBLOCKING);
    for (;;)
      {
	ssize_t ret;

#if defined LIBNETWORK_HAS_SSL
	assert(m_socketSSL);
	ret = SSL_write(m_socketSSL, reinterpret_cast<char const *>(msg + off),
	                static_cast<std::int32_t>(len - off));
#else
#if defined(__linux__) || defined(__APPLE__)
	ret = ::send(m_socket, reinterpret_cast<char const *>(msg + off),
	             len - off, 0);
#elif defined(_WIN32)
	ret = ::send(m_socket, reinterpret_cast<char const *>(msg + off),
	             static_cast<std::int32_t>(len - off), 0);
#endif
#endif
	if (ret == -1 || static_cast<std::size_t>(ret) == len - off)
	  {
	    if (ret == -1 && errno != EWOULDBLOCK && errno != EAGAIN)
	      {
		success = false;
	      }
	    break;
	  }
	off += static_cast<std::size_t>(ret);
      }
    return (success);
  }

  bool TCPSocket::send(void const *data, std::size_t len) const
  {
    assert(isStarted());
    if (getType() == ASocket::BLOCKING)
      {
	return (sendBlocking(data, len));
      }
    return (sendNonBlocking(data, len));
  }

  bool TCPSocket::recBlocking(void *buffer, std::size_t rlen,
                              ssize_t *buffLen) const
  {
    assert(getType() == ASocket::BLOCKING);

    // TODO: Remove useless debug log, do while should be one level above (use
    // header informations)
    nope::log::Log(Trace) << "recblocking Rlen -> " << rlen;
    do
      {
#if defined LIBNETWORK_HAS_SSL
	assert(m_socketSSL);
	*buffLen = SSL_read(m_socketSSL, static_cast<char *>(buffer),
	                    static_cast<std::int32_t>(rlen));
#else
#if defined(__linux__) || defined(__APPLE__)
	*buffLen = ::recv(m_socket, static_cast<char *>(buffer), rlen, 0);
#elif defined(_WIN32)
	*buffLen = ::recv(m_socket, static_cast<char *>(buffer),
	                  static_cast<std::int32_t>(rlen), 0);
#endif
#endif
      }
    while (*buffLen == -1 && errno == EINTR);
    nope::log::Log(Trace) << "recblocking BuffLen -> " << *buffLen;
    if (*buffLen <= 0)
      {
	nope::log::Log(Trace) << "recBlocking -> <= 0 [TCP]";
	return (!*buffLen);
      }
    nope::log::Log(Trace) << "Read " << *buffLen << "/" << rlen;
    return (true);
  }

  bool TCPSocket::recNonBlocking(void *buffer, std::size_t rlen,
                                 ssize_t *buffLen) const
  {
    std::uint8_t *buf;
    bool          success = true;

    assert(getType() == ASocket::NONBLOCKING);
    buf = static_cast<std::uint8_t *>(buffer);
    *buffLen = 0;
    for (;;)
      {
	ssize_t ret;

#if defined LIBNETWORK_HAS_SSL
	assert(m_socketSSL);
	ret = SSL_read(m_socketSSL, reinterpret_cast<char *>(buf + *buffLen),
	               static_cast<std::int32_t>(
	                   rlen - static_cast<std::size_t>(*buffLen)));
#else
#if defined(__linux__) || defined(__APPLE__)
	ret = ::recv(m_socket, reinterpret_cast<char *>(buf + *buffLen),
	             rlen - static_cast<std::size_t>(*buffLen), 0);
#elif defined(_WIN32)
	ret = ::recv(m_socket, reinterpret_cast<char *>(buf + *buffLen),
	             static_cast<std::int32_t>(rlen - *buffLen), 0);
#endif
#endif
	if (ret == -1)
	  {
#if EWOULDBLOCK == EAGAIN
	    if (errno != EWOULDBLOCK)
#else
	    if (errno != EWOULDBLOCK && errno != EAGAIN)
#endif
	      {
		success = false;
	      }
	    break;
	  }
	else if (ret == 0)
	  {
	    nope::log::Log(Debug) << "recNonBlocking -> 0 [TCP]";
	    *buffLen = 0;
	    break;
	  }
	if (static_cast<std::size_t>(*buffLen + ret) >= rlen)
	  {
	    success = !(static_cast<std::size_t>(*buffLen + ret) > rlen);
	    break;
	  }
	*buffLen += ret;
      }
    return (success);
  }

  bool TCPSocket::rec(void *buffer, std::size_t rlen, ssize_t *buffLen) const
  {
    assert(isStarted());
    if (getType() == ASocket::BLOCKING)
      {
	return (recBlocking(buffer, rlen, buffLen));
      }
    return (recNonBlocking(buffer, rlen, buffLen));
  }
}
