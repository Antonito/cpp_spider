You can create custom plugins in order to add new functionalities to the server.

Those plugins are dynamic libraries, that will be loaded automatically each time the server is started.

A plugin must be implemented as :

MyPlugin.hpp:

```cpp
#pragma once

#include "IPlugin.h" // Provided api

class MyPlugin : public spider::server::IPlugin
{
public:
	MyPlugin();
	virtual ~MyPlugin() = default;
	
	virtual std::string const &getName() const;
	virtual std::string const &getDescription() const;
	virtual void command(spider::server::IClient const *, void const *);
	
private:
    std::string const m_name;
    std::string const m_description;
};

```

MyPlugin.cpp:

```cpp
#include "MyPlugin.h"

MyPlugin::MyPlugin() : m_name("MyPlugin"), m_description("My very own plugin") {
}

std::string const &MyPlugin::getName() const {
	return m_name;
}

std::string const &MyPlugin::getDescription() const {
	return m_name;
}

void MyPlugin::command(spider::server::IClient const *, void const *) {
	// Implement your functionality here
}

static MyPlugin plugin;

// The entry point called by the server
extern "C" spider::server::IPlugin *getPlugin()
{
    return static_cast<spider::server::IPlugin *>(&plugin);
}
```