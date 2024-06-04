# Setup CMake
#set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.20)

project(server VERSION 1.0)
project(client VERSION 1.0)

set(SERVER_SOURCES 
	"src/server.cpp"
)
set(CLIENT_SOURCES 
	"src/client.cpp"
)

add_executable(server)
add_executable(client)

set(COMPILE_FLAGS="-g -Wall")
if(WIN32)
	set(CMAKE_CXX_COMPILER "C:/Users/JTK6759/Documents/apps/msys2/ucrt64/bin/g++.exe")
	
	target_compile_definitions(server PRIVATE DEBUG WIN32)
	target_compile_definitions(client PRIVATE DEBUG WIN32)
else()
	set(CMAKE_CXX_COMPILER "/usr/bin/g++")

	target_compile_definitions(server PRIVATE DEBUG LINUX)
	target_compile_definitions(client PRIVATE DEBUG LINUX)
endif()

include_directories(client PRIVATE "src/" "include/")
target_sources(server PRIVATE ${SERVER_SOURCES})
target_sources(client PRIVATE ${CLIENT_SOURCES})