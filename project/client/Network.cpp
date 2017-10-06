#include <chrono>
#include <thread>
#include "Network.h"

namespace spider
{
namespace client
{
Network::Network(mt::Queue<SystemMsg> &sendToNetwork, mt::Queue<library::IPayload::Order> &receivedFromNetwork) : m_sendToNetwork(sendToNetwork), m_receivedFromNetwork(receivedFromNetwork), m_cmdResponse{}, m_isConnected(false), m_sock(nullptr), m_sockData(nullptr), m_cmdReceived{}
{
}

Network::~Network()
{
}

void Network::run(std::uint32_t const port, std::string const &addr, bool const isIP)
{
    using namespace std::chrono_literals;

    while (1)
    {
        // Try to connect
        m_sock = std::make_unique<::network::TCPSocket>(port, addr, isIP, ::network::ASocket::SocketType::BLOCKING);

        m_isConnected = m_sock->openConnection();

        while (m_isConnected)
        {
            fd_set readfds, writefds, exceptfds;

            // Network loop here
            std::int32_t rc = multiplex(readfds, writefds, exceptfds);

            if (rc < 0)
            {
                // Something happened
                break;
            }
            else if (rc)
            {
                if (treatEvents(readfds, writefds, exceptfds) < 0)
                {
                    // Disconnect from the server
                    break;
                }
            }
        }

        // Reset connection
        m_sockData = nullptr;
        m_sock = nullptr;
        m_isConnected = false;

        // Prevent high cpu usage
        std::this_thread::sleep_for(100ms);
    }
}

std::int32_t Network::multiplex(fd_set &readfds, fd_set &writefds, fd_set &exceptfds)
{
    std::int32_t rc = -1;
    std::int32_t const sock = m_sock->getSocket();

    do
    {
        std::int32_t maxSock = sock;
        struct timeval tv;

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        tv.tv_sec = 2;
        tv.tv_usec = 50;

        FD_SET(sock, &readfds);
        FD_SET(sock, &exceptfds);
        if (!m_cmdResponse.empty())
        {
            FD_SET(sock, &writefds);
        }
        if (m_sockData)
        {
            std::int32_t const dataSock = m_sockData->getSocket();

            if (!m_sendToNetwork.empty())
            {
                FD_SET(dataSock, &writefds);
            }
            FD_SET(dataSock, &exceptfds);
            if (dataSock > maxSock)
            {
                maxSock = dataSock;
            }
        }
        rc = select(maxSock + 1, &readfds, &writefds, &exceptfds, &tv);
    } while (rc == -1 && errno == EINTR);
    return (rc);
}

std::int32_t Network::receivedCommand()
{
    std::array<char, 0x1000> data;
    ssize_t buffLen = 0;

    if (!m_sock->rec(data.data(), sizeof(data), &buffLen))
    {
        return -1;
    }
    // Write received data to ring buffer
    if (buffLen)
    {
        m_cmdReceived.write(reinterpret_cast<std::uint8_t const *>(data.data()), buffLen);

        // Send all received datas to main thread
        size_t cmdLen = 0;
        do
        {
            cmdLen = m_cmdReceived.hasCommand();
            if (cmdLen)
            {
                data.fill(0);
                m_cmdReceived.read(reinterpret_cast<std::uint8_t *>(data.data()), cmdLen);
                m_receivedFromNetwork.push(library::IPayload::Order(data.data()));
            }

        } while (cmdLen);
    }
    return 0;
}

std::int32_t Network::writeCommandResponse()
{
    std::int32_t rc = 0;

    // Send a reponse stored in the cmdResponse queue
    if (!m_cmdResponse.empty())
    {
        std::string const &cur = m_cmdResponse.front();
        if (!m_sock->send(cur.c_str(), cur.length()))
        {
            rc = -1;
        }
        m_cmdResponse.pop();
    }
    return rc;
}

std::int32_t Network::treatEvents(fd_set const &readfds, fd_set const &writefds, fd_set const &exceptfds)
{
    std::int32_t const sock = m_sock->getSocket();
    std::int32_t rc = 0;

    // Check commands
    if (FD_ISSET(sock, &readfds))
    {
        // Read some commands
        rc = receivedCommand();
    }
    if (!rc && FD_ISSET(sock, &writefds))
    {
        // Write response to commands
        rc = writeCommandResponse();
    }
    if (!rc && FD_ISSET(sock, &exceptfds))
    {
        // Disconnect from the server
        rc = -1;
    }

    if (m_sockData)
    {
        std::int32_t const dataSock = m_sockData->getSocket();

        if (!rc && FD_ISSET(dataSock, &writefds))
        {
            // Write some datas
            if (!m_sendToNetwork.empty())
            {
                // Send data
            }
        }
        if (!rc && FD_ISSET(dataSock, &exceptfds))
        {
            // Disconnect from the server
            rc = -1;
        }
    }

    return rc;
}
}
}