#include "network_stdafx.hpp"
#if defined(__linux__) || defined(__APPLE__)
#include <fcntl.h>
#include <netdb.h>
#endif

#include <csignal>
#include <cassert>
#include <iostream>
#include "ASocket.hpp"

namespace network
{

#if defined LIBNETWORK_HAS_SSL
  void      ASocket::initSSL()
  {
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    nope::log::Log(Info) << "OpenSSL initialized.";
  }

  void ASocket::deinitSSL()
  {
    ERR_free_strings();
    EVP_cleanup();
    nope::log::Log(Info) << "OpenSSL un-initialized.";
  }
#endif

// Make sure you have to init / deinit WSA
#if defined _WIN32 || defined LIBNETWORK_HAS_SSL
  std::uint32_t               network::ASocket::m_nbSockets = 0;
#endif

#if defined(_WIN32)
  bool network::ASocket::m_WSAInited = false;
#endif

#if defined LIBNETWORK_HAS_SSL
  ASocket::ASocket(sock_t const socket, SSL_CTX *ctx)
      : m_socket(socket), m_socketSSL(nullptr), m_sslctx(ctx), m_port(0),
        m_host(""), m_ip(false), m_maxClients(0), m_curClients(0), m_addr{},
        m_type()
  {
    nope::log::Log(Debug) << "Loading socket #" << m_socket;

    m_socketSSL = SSL_new(ctx);
    if (!m_socketSSL)
      {
	throw std::runtime_error("Cannot create new SSL client");
      }

#if defined(_WIN32)
    // Do we need to load the network DLL ?
    if (!m_nbSockets && !initWSA())
      {
	nope::log::Log(Error) << "Cannot load network DLL";
	throw std::runtime_error("Cannot load network DLL");
      }
    nope::log::Log(Debug) << "Adding socket " << m_nbSockets;
#endif

#if defined _WIN32 || defined LIBNETWORK_HAS_SSL
    ++m_nbSockets;
#endif
  }
#endif

  ASocket::ASocket(sock_t const socket)
      : m_socket(socket),
#if defined LIBNETWORK_HAS_SSL
        m_socketSSL(nullptr), m_sslctx(nullptr),
#endif
        m_port(0), m_host(""), m_ip(false), m_maxClients(0),
        m_curClients(0), m_addr{}, m_type()
  {
    nope::log::Log(Debug) << "Loading socket #" << m_socket;

#if defined(_WIN32)
    // Do we need to load the network DLL ?
    if (!m_nbSockets && !initWSA())
      {
	nope::log::Log(Error) << "Cannot load network DLL";
	throw std::runtime_error("Cannot load network DLL");
      }
    nope::log::Log(Debug) << "Adding socket " << m_nbSockets;
#endif

#if defined _WIN32 || defined LIBNETWORK_HAS_SSL
    ++m_nbSockets;
#endif
  }

  ASocket &ASocket::operator=(sock_t const sock)
  {
    m_socket = sock;
    // TODO: Create SSL connection here ?
    m_port = 0;
    m_host = "";
    m_ip = false;
    m_maxClients = 0;
    m_curClients = 0;
    m_addr = sockaddr_in_t{};
    m_type = SocketType::BLOCKING;
    return (*this);
  }

#if defined LIBNETWORK_HAS_SSL
  ASocket::ASocket(SocketType type, SSL_CTX *ctx)
#else
  ASocket::ASocket(SocketType type)
#endif
      : m_socket(-1),
#if defined LIBNETWORK_HAS_SSL
        m_socketSSL(nullptr), m_sslctx(ctx),
#endif
        m_port(0), m_host(""), m_ip(false), m_maxClients(0),
        m_curClients(0), m_addr{}, m_type(type)
  {
#if defined(__linux__) || (__APPLE__)
    // If we can, ignore SIGPIPE
    std::signal(SIGPIPE, SIG_IGN);
    nope::log::Log(Debug) << "Ignoring SIGPIPE";
#endif

#if defined(_WIN32)
    // Do we need to load the network DLL ?
    if (!m_nbSockets && !initWSA())
      {
	nope::log::Log(Error) << "Cannot load network DLL";
	throw std::runtime_error("Cannot load network DLL");
      }
    nope::log::Log(Debug) << "Adding socket " << m_nbSockets;
#endif

#if defined _WIN32 || defined LIBNETWORK_HAS_SSL
    ++m_nbSockets;
#endif
  }

#if defined LIBNETWORK_HAS_SSL
  ASocket::ASocket(std::uint16_t port, std::string const &host,
                   SocketType type, SSL_CTX *ctx)
      : ASocket(type, ctx)
#else
  ASocket::ASocket(std::uint16_t port, std::string const &host,
                   SocketType type)
      : ASocket(type)
#endif
  {
    // cppcheck-suppress useInitializationList
    m_host = host;
    m_port = port;
  }

#if defined LIBNETWORK_HAS_SSL
  ASocket::ASocket(std::uint16_t port, std::uint32_t maxClients,
                   SocketType type, SSL_CTX *ctx)
      : ASocket(type, ctx)
#else
  ASocket::ASocket(std::uint16_t port, std::uint32_t maxClients,
                   SocketType type)
      : ASocket(type)
#endif
  {
    assert(maxClients);
    m_port = port;
    m_maxClients = maxClients;
  }

  ASocket::ASocket(ASocket const &other) : ASocket(other.m_type)
  {
    // cppcheck-suppress useInitializationList
    m_host = other.m_host;
    m_socket = other.m_socket;
    m_port = other.m_port;
    m_ip = other.m_ip;
    m_maxClients = other.m_maxClients;
    m_curClients = other.m_curClients;
    m_addr = other.m_addr;
  }

  ASocket::ASocket(ASocket &&other)
      : m_socket(other.m_socket),
#if defined LIBNETWORK_HAS_SSL
        m_socketSSL(other.m_socketSSL), m_sslctx(other.m_sslctx),
#endif
        m_port(other.m_port), m_host(other.m_host), m_ip(other.m_ip),
        m_maxClients(other.m_maxClients), m_curClients(other.m_curClients),
        m_addr(other.m_addr), m_type(other.m_type)
  {
    other.m_socket = -1;
  }

  ASocket::~ASocket()
  {
    closeConnection();
#if defined _WIN32 || LIBNETWORK_HAS_SSL
    if (m_nbSockets)
      assert(m_nbSockets);
    --m_nbSockets;
#endif

#if defined LIBNETWORK_HAS_SSL
    if (!m_nbSockets)
      {
	ERR_free_strings();
	EVP_cleanup();
      }
#endif

#if defined(_WIN32)
    if (!m_nbSockets)
      {
	// It is the last socket
	nope::log::Log(Debug) << "Closing last socket";
	deinitWSA();
      }
#endif
  }

  // Close the socket
  bool ASocket::closeConnection()
  {
#if defined LIBNETWORK_HAS_SSL
    if (m_socketSSL)
      {
	SSL_shutdown(m_socketSSL);
	SSL_free(m_socketSSL);
	m_socketSSL = nullptr;
#else
    if (m_socket > 0 && !closesocket(m_socket))
      {
#endif
	nope::log::Log(Debug) << "Closing socket #" << m_socket;
	m_socket = -1;
      }
    return (!isStarted());
  }

  ASocket &ASocket::operator=(ASocket const &other)
  {
    if (this != &other)
      {
	m_socket = other.m_socket;
#if defined LIBNETWORK_HAS_SSL
	m_socketSSL = other.m_socketSSL;
	m_sslctx = other.m_sslctx;
#endif
	m_port = other.m_port;
	m_host = other.m_host;
	m_ip = other.m_ip;
	m_maxClients = other.m_maxClients;
	m_curClients = other.m_curClients;
	m_addr = other.m_addr;
	m_type = other.m_type;
      }
    return (*this);
  }

  bool ASocket::operator==(ASocket const &other) const
  {
    if (this != &other)
      {
	return (m_socket == other.m_socket);
      }
    return (true);
  }

  bool ASocket::isStarted() const
  {
    return (m_socket != -1);
  }

  sock_t ASocket::getSocket() const
  {
    return (m_socket);
  }

  std::uint16_t ASocket::getPort() const
  {
    return (m_port);
  }

  std::uint32_t ASocket::getMaxClients() const
  {
    return (m_maxClients);
  }

  std::uint32_t ASocket::getCurClients() const
  {
    return (m_curClients);
  }

  ASocket::SocketMode ASocket::getMode() const
  {
    if (m_maxClients != 0)
      {
	return (ASocket::SERVER);
      }
    return (ASocket::CLIENT);
  }

  ASocket::SocketType ASocket::getType() const
  {
    return (m_type);
  }

  sockaddr_in_t const &ASocket::getSockAddr() const
  {
    return (m_addr);
  }

#if defined LIBNETWORK_HAS_SSL
  SSL_CTX * ASocket::getCTX()
  {
    return (m_sslctx);
  }
#endif

#if defined LIBNETWORK_HAS_SSL
  bool ASocket::connectToHost(std::int32_t const socktype,
                              std::int32_t const proto, bool shouldConnect,
                              std::string const &key, std::string const &cert)
#else
  bool ASocket::connectToHost(std::int32_t const socktype,
                              std::int32_t const proto, bool shouldConnect)
#endif
  {
    addrinfo_t  hints = {};
    addrinfo_t *res = nullptr;
    bool        connected = false;
    SocketType  typeBackup;

    nope::log::Log(Debug) << "Connection to host";
    assert(m_socket == -1);
    if (m_ip)
      {
	nope::log::Log(Debug) << "Provided address is an IP";
	hints.ai_flags = AI_NUMERICHOST;
      }
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = socktype;
    hints.ai_protocol = proto;
    nope::log::Log(Debug) << "Processing address resolution";
    if (getaddrinfo(m_host.c_str(), std::to_string(m_port).c_str(), &hints,
                    &res) == 0)
      {
	// Loop over all the potential addresses
	for (addrinfo_t *ptr = res; ptr; ptr = ptr->ai_next)
	  {
	    std::int32_t ret = 0;

	    typeBackup = getType();
	    m_type = ASocket::BLOCKING;
	    try
	      {
#if defined LIBNETWORK_HAS_SSL
		initSocket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol,
		           key, cert);
#else
		initSocket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
#endif
	      }
	    catch (std::exception &e)
	      {
		std::cerr << e.what() << std::endl;
		break;
	      }

	    ret = 0;
	    if (shouldConnect)
	      {
#if defined(__linux__) || defined(__APPLE__)
		ret = connect(m_socket, ptr->ai_addr, ptr->ai_addrlen);
#elif defined(_WIN32)
		ret = connect(m_socket, ptr->ai_addr,
		              static_cast<std::int32_t>(ptr->ai_addrlen));
#endif
	      }
	    if (ret != -1)
	      {
		if (typeBackup == ASocket::NONBLOCKING)
		  {
		    m_type = ASocket::NONBLOCKING;
		    if (setSocketType() == false)
		      {
			throw std::runtime_error("Cannot set socket type");
		      }
		  }
		if (res->ai_addrlen <= sizeof(m_addr))
		  {
		    nope::log::Log(Debug) << "Updating sockaddr_in content";
		    std::memcpy(&m_addr, res->ai_addr, res->ai_addrlen);
		  }
		nope::log::Log(Debug) << "Found an address, connected !";
		connected = true;
#if defined LIBNETWORK_HAS_SSL
		if (SSL_connect(m_socketSSL) != 1)
		  {
		    nope::log::Log(Error) << "SSL handshake failed";
		    connected = false;
		  }
#endif
		break;
	      }
	    closeConnection();
	  }
      }
    freeaddrinfo(res);
    return (connected);
  }

#if defined LIBNETWORK_HAS_SSL
  void ASocket::initSocket(std::int32_t domain, std::int32_t type,
                           std::int32_t protocol, std::string const &key,
                           std::string const &cert)
#else
  void ASocket::initSocket(std::int32_t domain, std::int32_t type,
                           std::int32_t protocol)
#endif
  {
    char const enable = 1;

    nope::log::Log(Debug) << "Creating socket...";
    m_socket = ::socket(domain, type, protocol);
    if (m_socket == -1)
      {
	nope::log::Log(Error) << "Cannot create socket";
	throw std::runtime_error("Cannot create socket");
      }
    nope::log::Log(Debug) << "Created socket #" << m_socket;
    if (setSocketType() == false)
      {
	nope::log::Log(Error) << "Cannot set socket type";
	throw std::runtime_error("Cannot set socket type");
      }
    if (m_port != 0)
      {
	// Try to reuse the socket
	if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &enable,
	               sizeof(enable)) < 0)
	  {
	    if (errno != EINVAL)
	      {
		nope::log::Log(Error) << "Cannot set socket options";
		throw std::runtime_error("Cannot set socket options");
	      }
	  }
      }
#if defined LIBNETWORK_HAS_SSL
    if (!m_sslctx)
      {
	throw std::runtime_error("SSL Context not set");
      }
    SSL_CTX_set_options(m_sslctx, SSL_OP_SINGLE_DH_USE);

    std::int32_t useCert =
        SSL_CTX_use_certificate_file(m_sslctx, cert.c_str(), SSL_FILETYPE_PEM);
    if (useCert != 1)
      {
	throw std::runtime_error("Cannot load " + cert);
      }
    std::int32_t useKey =
        SSL_CTX_use_PrivateKey_file(m_sslctx, key.c_str(), SSL_FILETYPE_PEM);
    if (useKey != 1)
      {
	throw std::runtime_error("Cannot load " + key);
      }
    if (!SSL_CTX_check_private_key(m_sslctx))
      {
	throw std::runtime_error(
	    "Private key does not match the certificate public key");
      }

    m_socketSSL = SSL_new(m_sslctx);
    if (!m_socketSSL)
      {
	throw std::runtime_error("Cannot create secure socket");
      }
    SSL_set_fd(m_socketSSL, m_socket);
#endif
    nope::log::Log(Debug) << "Socket created successfuly";
  }

  void ASocket::hostConnection()
  {
    nope::log::Log(Debug) << "Socket created successfuly";
    assert(m_socket != -1);
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(m_socket, reinterpret_cast<sockaddr_t *>(&m_addr),
             sizeof(m_addr)) == -1)
      {
	throw std::runtime_error("Cannot bind to socket");
      }
    if (m_port == 0)
      {
	sockaddr_in_t newAddr = {};
	socklen_t     len = sizeof(sockaddr_in_t);

	// Get the port selected by the kernel
	if (getsockname(m_socket, reinterpret_cast<sockaddr_t *>(&newAddr),
	                &len) == -1)
	  {
	    throw std::runtime_error("Cannot get port selected by the kernel");
	  }
	m_port = ntohs(newAddr.sin_port);
      }
  }

  // Set the socket to blocking or non-blocking state
  bool ASocket::setSocketType() const
  {
    bool ret;

    assert(isStarted());
#if defined(_WIN32)
    unsigned long lock = !(m_type == ASocket::BLOCKING);
    ret = ioctlsocket(m_socket, FIONBIO, &lock) == 0;
#else
    std::int32_t flags;

#if defined(O_NONBLOCK)
    flags = fcntl(m_socket, F_GETFL, 0);
    ret = false;
    if (flags >= 0)
      {
	flags = (m_type == ASocket::BLOCKING) ? (flags & ~O_NONBLOCK)
	                                      : (flags | O_NONBLOCK);
	ret = fcntl(m_socket, F_SETFL, flags) == 0;
      }
#else
    flags = 1;
    ret = ioctl(m_socket, FIOBIO, &flags) == 0;
#endif

#endif
    return (ret);
  }

#if defined(_WIN32)
  bool ASocket::initWSA() const
  {
    WSADATA wsa;

    assert(m_WSAInited == false);
    if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
      {
	// Initialization failed
	return (false);
      }
    m_WSAInited = true;
    return (true);
  }

  void ASocket::deinitWSA() const
  {
    assert(m_WSAInited == true);
    WSACleanup();
    m_WSAInited = false;
  }
#endif
}
