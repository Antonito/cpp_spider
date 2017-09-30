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
add_executable(spider_server ${SERVER_SOURCES})
target_include_directories(spider_server PUBLIC "${ROOT_SOURCE_DIR}/project/server/")
target_link_libraries(spider_server PUBLIC ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

# Get servers plugin's sources
file(GLOB SERVER_SOURCES project/server_plugin/example)

install(TARGETS spider_server DESTINATION bin)
