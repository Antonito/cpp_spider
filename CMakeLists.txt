##
## Configuration
##
cmake_minimum_required(VERSION 3.1)
project(Spider)

# Configure C++ version
set(CMAKE_CXX_STANDARD 14)

# Configure SSL
ADD_DEFINITIONS(-DLIBNETWORK_HAS_SSL)

# Configure serialization
#ADD_DEFINITIONS(-DSPIDER_SERIALIZE)

if(UNIX AND NOT APPLE)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Weffc++ -Wno-deprecated-declarations -DLIBNETWORK_HAS_SSL")
elseif(APPLE)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-exit-time-destructors -Wno-c++1z-extensions -DLIBNETWORK_HAS_SSL")
endif()
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}")

# Configure OpenSSL
if(APPLE)
set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl/")
include_directories("/usr/local/opt/openssl/include/")
elseif(WIN32)
set(OPENSSL_USE_STATIC_LIBS TRUE)
set(OPENSSL_ROOT_DIR "C:\\openssl_x64_static\\")
set(OPENSSL_LIBRARIES "C:\\openssl_x64_static\\lib\\")
set(OPENSSL_INCLUDE_DIR "C:\\openssl_x64_static\\include\\")
endif()
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Configure Boost
if (NOT WIN32)
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
endif()


##
## Libraries
##

# Get log library files
file(GLOB LIBLOGGER_SOURCES project/liblogger/*.cpp)
set(LIBLOGGER liblogger)
add_library(${LIBLOGGER} STATIC ${LIBLOGGER_SOURCES})
target_include_directories(${LIBLOGGER} PUBLIC "./project/liblogger")

# Get network library files
file(GLOB LIBNETWORK_SOURCES project/libnetwork/*.cpp)
set(LIBNETWORK libnetwork)
add_library(${LIBNETWORK} STATIC ${LIBNETWORK_SOURCES})
target_include_directories(${LIBNETWORK} PUBLIC "./project/libnetwork" "./project/liblogger")

# Get Loader library files
file(GLOB LIBLOADER_SOURCES project/libloader/*.cpp)
set(LIBLOADER libloader)
add_library(${LIBLOADER} STATIC ${LIBLOADER_SOURCES})
target_include_directories(${LIBLOADER} PUBLIC "./project/libloader")

# Get MultiThread library files
file(GLOB LIBMT_SOURCES project/libmt/*.cpp)
set(LIBMT libmt)
add_library(${LIBMT} STATIC ${LIBMT_SOURCES})
target_include_directories(${LIBMT} PUBLIC "./project/libmt")

# Get MacAddress library files
file(GLOB LIBMACADDR_SOURCES project/libmacaddr/*.cpp)
set(LIBMACADDR libmacaddr)
add_library(${LIBMACADDR} STATIC ${LIBMACADDR_SOURCES})
if (UNIX AND NOT APPLE)
  set_property(TARGET ${LIBMACADDR} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()
target_include_directories(${LIBMACADDR} PUBLIC "./project/libmacaddr" "./project/common/")

if (NOT WIN32)
##
## Server
##

# Get server sources files
file(GLOB SERVER_SOURCES project/server/*.cpp)

# Create server executable
set(SPIDER_SERVER spider_server)
add_executable(${SPIDER_SERVER} ${SERVER_SOURCES})
target_include_directories(${SPIDER_SERVER} PUBLIC "./project/server/" "./project/common/")
##########
find_package(libmongocxx QUIET)
if (libmongocxx_FOUND)
find_package(libbsoncxx QUIET)
if (libbsoncxx_FOUND)
add_definitions(-DMONGOCXX)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/lib")
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libbson-1.0")
include_directories("/usr/local/lib")
target_link_libraries(${SPIDER_SERVER} PUBLIC ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${LIBLOGGER} ${LIBLOADER} ${LIBNETWORK} ${LIBMT} ${LIBMACADDR} ${OPENSSL_LIBRARIES} ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})
else()
target_link_libraries(${SPIDER_SERVER} PUBLIC ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${LIBLOGGER} ${LIBLOADER} ${LIBNETWORK} ${LIBMT} ${LIBMACADDR} ${OPENSSL_LIBRARIES})
endif()
else()
target_link_libraries(${SPIDER_SERVER} PUBLIC ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${LIBLOGGER} ${LIBLOADER} ${LIBNETWORK} ${LIBMT} ${LIBMACADDR} ${OPENSSL_LIBRARIES})
endif()

##########
#target_link_libraries(${SPIDER_SERVER} PUBLIC ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${LIBLOGGER} ${LIBLOADER} ${LIBNETWORK} ${LIBMT} ${LIBMACADDR} ${OPENSSL_LIBRARIES})
if(UNIX AND NOT APPLE)
target_link_libraries(${SPIDER_SERVER} PUBLIC -lpthread -ldl)
endif()

##
## Server's plugins
##

# Get GetInfo plugin's sources
file(GLOB PLUGIN_SOURCE_GETINFO project/server_plugin/getinfo/*.cpp)

# Create plugin library
set(PLUGIN_GETINFO plugin_getinfo)
add_library(${PLUGIN_GETINFO} SHARED ${PLUGIN_SOURCE_GETINFO})
target_include_directories(${PLUGIN_GETINFO} PUBLIC "./project/server_plugin/" "./project/common/")

# Get Mouse plugin's sources
file(GLOB PLUGIN_SOURCE_MOUSE project/server_plugin/mouse/*.cpp)

# Create plugin library
set(PLUGIN_MOUSE plugin_mouse)
add_library(${PLUGIN_MOUSE} SHARED ${PLUGIN_SOURCE_MOUSE})
target_include_directories(${PLUGIN_MOUSE} PUBLIC "./project/server_plugin/" "./project/common/")

# Get keyboard plugin's sources
file(GLOB PLUGIN_SOURCE_KEYBOARD project/server_plugin/keyboard/*.cpp)

# Create plugin library
set(PLUGIN_KEYBOARD plugin_keyboard)
add_library(${PLUGIN_KEYBOARD} SHARED ${PLUGIN_SOURCE_KEYBOARD})
target_include_directories(${PLUGIN_KEYBOARD} PUBLIC "./project/server_plugin/" "./project/common/")

# Get replicate plugin's sources
file(GLOB PLUGIN_SOURCE_REPLICATE project/server_plugin/replicate/*.cpp)

# Create plugin library
set(PLUGIN_REPLICATE plugin_replicate)
add_library(${PLUGIN_REPLICATE} SHARED ${PLUGIN_SOURCE_REPLICATE})
target_include_directories(${PLUGIN_REPLICATE} PUBLIC "./project/server_plugin/" "./project/common/")

# Get kill plugin's sources
file(GLOB PLUGIN_SOURCE_KILL project/server_plugin/kill/*.cpp)

# Create plugin library
set(PLUGIN_KILL plugin_kill)
add_library(${PLUGIN_KILL} SHARED ${PLUGIN_SOURCE_KILL})
target_include_directories(${PLUGIN_KILL} PUBLIC "./project/server_plugin/" "./project/common/")

endif()

##
## Client
##

# Create client executable
file(GLOB CLIENT_SOURCES project/client/*.cpp)
set(SPIDER_CLIENT spider_client)
add_executable(${SPIDER_CLIENT} ${CLIENT_SOURCES})
target_include_directories(${SPIDER_CLIENT} PUBLIC "./project/client/" "./project/common/")
target_link_libraries(${SPIDER_CLIENT} PUBLIC ${LIBNETWORK} ${LIBLOGGER} ${LIBLOADER} ${LIBMT} ${LIBMACADDR} ${OPENSSL_LIBRARIES})
if(UNIX AND NOT APPLE)
target_link_libraries(${SPIDER_CLIENT} PUBLIC -lpthread -ldl)
endif()

##
## Client's plugins
##
file(GLOB CLIENT_PLUGIN_SOURCES project/client_plugin/*.cpp project/client_plugin/*.mm)
set(SPIDER_CLIENT_PLUGIN foo)
add_library(${SPIDER_CLIENT_PLUGIN} SHARED ${CLIENT_PLUGIN_SOURCES})
target_link_libraries(${SPIDER_CLIENT_PLUGIN} PUBLIC ${LIBMT} ${LIBMACADDR})
if (UNIX AND NOT APPLE)
  target_link_libraries(${SPIDER_CLIENT_PLUGIN} PUBLIC ${LIBMT} ${LIBMACADDR} -lX11)
endif()
target_include_directories(${SPIDER_CLIENT_PLUGIN} PUBLIC "./project/client_plugin/" "./project/client/" "./project/common/")

if(APPLE)
  target_link_libraries(${SPIDER_CLIENT_PLUGIN} PUBLIC "-framework ApplicationServices")
endif()

##
## Client's sentinel
##
if(WIN32)
file(GLOB CLIENT_SENTINEL_SOURCES project/client_sentinel/*.cpp)
set(SPIDER_CLIENT_SENTINEL spider_sentinel)
add_executable(${SPIDER_CLIENT_SENTINEL} ${CLIENT_SENTINEL_SOURCES})
endif()

##
## Installation
##

if(WIN32)
# Install to %AppData% on Windows
install(TARGETS ${SPIDER_CLIENT} DESTINATION $ENV{APPDATA}/spider/)
install(TARGETS ${SPIDER_CLIENT_PLUGIN} DESTINATION $ENV{APPDATA}/spider/)
install(TARGETS ${SPIDER_CLIENT_SENTINEL} DESTINATION $ENV{APPDATA}/spider/)
install(FILES spider.crt DESTINATION $ENV{APPDATA}/spider/)
install(FILES spider.key DESTINATION $ENV{APPDATA}/spider/)
install(FILES ./deps/windows/libeay32.dll DESTINATION $ENV{APPDATA}/spider/)
install(FILES ./deps/windows/ssleay32.dll DESTINATION $ENV{APPDATA}/spider/)
else()
install(TARGETS ${SPIDER_SERVER} DESTINATION bin)
install(TARGETS ${PLUGIN_EXAMPLE} DESTINATION bin/plugins)
install(TARGETS ${PLUGIN_GETINFO} DESTINATION bin/plugins)
endif()
