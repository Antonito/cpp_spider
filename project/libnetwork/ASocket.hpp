#ifndef ASOCKET_HPP_
#define ASOCKET_HPP_

#include <cstring>
#include <string>
#include <cstring>

// Note: Define LIBNETWORK_HAS_SSL in order to use SSL
#if defined LIBNETWORK_HAS_SSL

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang            system_header
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif

#include "ISocket.hpp"

// Disable clang warning for implicit padding
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

// TODO: Add IPv4 / IPV6 selection, SSL / TLS
namespace network
{
  class ASocket : public ISocket
  {
  public:
    enum SocketMode
    {
      SERVER,
      CLIENT
    };
    enum SocketType
    {
      BLOCKING,
      NONBLOCKING
    };

    ASocket(ASocket const &other);
    ASocket &operator=(ASocket const &);
    ASocket(ASocket &&);
    ASocket &operator=(sock_t const sock);

    virtual ~ASocket();
    virtual bool closeConnection();

    bool          isStarted() const;
    sock_t        getSocket() const;
    std::uint16_t getPort() const;
    std::uint32_t getMaxClients() const;
    std::uint32_t getCurClients() const;
    SocketMode    getMode() const;
    SocketType    getType() const;
#if defined       LIBNETWORK_HAS_SSL
    SSL_CTX *     getCTX();
#endif

    sockaddr_in_t const &getSockAddr() const;

    bool operator==(ASocket const &other) const;

  protected:
#if defined LIBNETWORK_HAS_SSL
    explicit ASocket(sock_t const socket, SSL_CTX *ctx);
#endif
    explicit ASocket(sock_t const socket);

    ASocket(std::uint16_t port, std::string const &host,
            SocketType type = ASocket::BLOCKING);
    ASocket(std::uint16_t port, std::uint32_t maxClients,
            SocketType type = ASocket::BLOCKING);

    virtual bool connectToHost(std::int32_t const socktype,
                               std::int32_t const proto, bool shouldConnect);
    virtual void hostConnection();
    void initSocket(std::int32_t domain, std::int32_t type,
                    std::int32_t protocol);
    bool setSocketType() const;

    sock_t   m_socket;
#if defined  LIBNETWORK_HAS_SSL
    SSL *    m_socketSSL;
    SSL_CTX *m_sslctx;
#endif
    std::uint16_t m_port;
    std::string   m_host;
    bool          m_ip;
    std::uint32_t m_maxClients;
    std::uint32_t m_curClients;
    sockaddr_in_t m_addr;
    SocketType    m_type;

  private:
    explicit ASocket(SocketType type);

#if defined _WIN32 || defined LIBNETWORK_HAS_SSL
    static std::uint32_t      m_nbSockets;
#endif

// Init network DLL
#if defined(_WIN32)
    static bool m_WSAInited;
    bool        initWSA() const;
    void        deinitWSA() const;
#endif
  };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // !ASOCKET_HPP_
