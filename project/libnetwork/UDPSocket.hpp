#ifndef UDP_SOCKET_HPP_
#define UDP_SOCKET_HPP_

#include "ASocket.hpp"

namespace network
{
  class UDPSocket : public ASocket
  {
  public:
#if defined LIBNETWORK_HAS_SSL
    explicit UDPSocket(sock_t const sock, SSL_CTX *ctx);
#else
    explicit UDPSocket(sock_t const sock);
#endif
    UDPSocket(std::uint16_t port, std::string const &host, bool ip,
              SocketType type);
    UDPSocket(std::uint16_t port, std::uint32_t maxClients, SocketType type);
    UDPSocket(UDPSocket &&);
    virtual ~UDPSocket();
    virtual bool send(void const *data, std::size_t len) const;
    virtual bool rec(void *buffer, std::size_t rlen, ssize_t *buffLen) const;
#if defined      LIBNETWORK_HAS_SSL
    virtual bool openConnection(std::string const &key,
                                std::string const &cert);
#else
    virtual bool openConnection();
#endif

    bool send(void const *data, std::size_t len, sockaddr_t const *dest,
              socklen_t destLen) const;
    bool rec(void *buffer, std::size_t rlen, sockaddr_t *addr,
             socklen_t *addrLen) const;

  private:
  };
}

#endif // !UDP_SOCKET_HPP_
