cmake_minimum_required(VERSION 3.1)
project(Spider)

# Configure C++ version
set(CMAKE_CXX_STANDARD 14)

# Configure Boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

# Get server sources files
file(GLOB SERVER_SOURCES project/server/*.cpp)

# Create server executable
set(SPIDER_SERVER spider_server)
add_executable(${SPIDER_SERVER} ${SERVER_SOURCES})
target_include_directories(${SPIDER_SERVER} PUBLIC "./project/server/")
target_link_libraries(${SPIDER_SERVER} PUBLIC ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

# Get servers plugin's sources
file(GLOB PLUGIN_SOURCE_EXAMPLE project/server_plugin/example/*.cpp)

# Create plugin library
set(PLUGIN_EXAMPLE plugin_example)
add_library(${PLUGIN_EXAMPLE} SHARED ${PLUGIN_SOURCE_EXAMPLE})
target_include_directories(${PLUGIN_EXAMPLE} PUBLIC "./project/server_plugin/")

install(TARGETS ${SPIDER_SERVER} DESTINATION bin)
install(TARGETS ${PLUGIN_EXAMPLE} DESTINATION bin/plugins)
