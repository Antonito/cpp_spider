#ifndef TCPSOCKET_HPP_
#define TCPSOCKET_HPP_

#include "ASocket.hpp"

namespace network
{
  class TCPSocket : public ASocket
  {
  public:
#if defined LIBNETWORK_HAS_SSL
    explicit TCPSocket(sock_t const sock, SSL_CTX *ctx);
    explicit TCPSocket(sock_t const sock, SSL *sockSSL, SSL_CTX *ctx);
#endif
    explicit TCPSocket(sock_t const sock);

#if defined LIBNETWORK_HAS_SSL
    TCPSocket(std::uint16_t port, std::string const &host, bool ip,
              SocketType type, SSL_CTX *ctx);
    TCPSocket(std::uint16_t port, std::uint32_t maxClients, SocketType type,
              SSL_CTX *ctx);
#else
    TCPSocket(std::uint16_t port, std::string const &host, bool ip,
              SocketType type);
    TCPSocket(std::uint16_t port, std::uint32_t maxClients, SocketType type);
#endif
    TCPSocket(TCPSocket &&);
    virtual ~TCPSocket();
    virtual bool send(void const *data, std::size_t len) const;
    virtual bool rec(void *buffer, std::size_t rlen, ssize_t *buffLen) const;
#if defined      LIBNETWORK_HAS_SSL
    virtual bool openConnection(std::string const &key,
                                std::string const &cert);
#else
    virtual bool openConnection();
#endif

    TCPSocket &operator=(sock_t const sock);

  private:
    bool sendBlocking(void const *data, std::size_t len) const;
    bool sendNonBlocking(void const *data, std::size_t len) const;
    bool recBlocking(void *buffer, std::size_t rlen, ssize_t *buffLen) const;
    bool recNonBlocking(void *buffer, std::size_t rlen,
                        ssize_t *buffLen) const;
  };
}

#endif // !TCPSOCKET_HPP_
