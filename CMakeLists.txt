# Setup CMake
#set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.20)
set(COMPILE_FLAGS="-g -Wall")
if(WIN32)
	set(CMAKE_CXX_COMPILER "C:/Users/JTK6759/Documents/apps/msys2/ucrt64/bin/g++.exe")
else()
	set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

#     ______            ______     #
#####/                        \#####
###--------< Shared Lib >--------###
#####\______            ______/#####
#                                  #
project(sockConn VERSION 1.0.0 DESCRIPTION "Simple unix socket connection abstraction layer for inter-process connectivity")
add_library(sockConn SHARED
	"include/sockConn.h"

	"src/serializer.cpp"
	"src/server.cpp"
	"src/client.cpp"
)

if(WIN32)
	target_link_libraries(sockConn PRIVATE wsock32)
	target_compile_definitions(sockConn PRIVATE DEBUG WIN32)
else()
	target_compile_definitions(sockConn PRIVITE DEBUG LINUX)
endif()

include_directories(sockConn PRIVATE "./src/" "./include/")
set_target_properties(sockConn PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	PUBLIC_HEADER "include/sockConn.h"
)
configure_file(sockConn.pc.in sockConn.pc @ONLY)	# Expand "@" macros

# Don't install anywhere if on windows
if(NOT WIN32)
	include(GNUInstallDirs)	# Use GNU install directories

	# Install
	install(TARGETS sockConn
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
	install(FILES ${CMAKE_BINARY_DIR}/sockConn.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
endif()



#     ______            ______     #
#####/                        \#####
###--------< Demo Files >--------###
#####\______            ______/#####
#                                  #
project(server VERSION 1.0)
project(client VERSION 1.0)
project(serial VERSION 1.0)

add_executable(server)
add_executable(client)
add_executable(serial)

if(WIN32)
	target_compile_definitions(server PRIVATE DEBUG WIN32)
	target_compile_definitions(client PRIVATE DEBUG WIN32)
	target_compile_definitions(serial PRIVATE DEBUG WIN32)
else()
	target_compile_definitions(server PRIVATE DEBUG LINUX)
	target_compile_definitions(client PRIVATE DEBUG LINUX)
	target_compile_definitions(serial PRIVATE DEBUG LINUX)
endif()

target_sources(server PRIVATE "demo/server.cpp")
target_sources(client PRIVATE "demo/client.cpp")
target_sources(serial PRIVATE "demo/serial.cpp")

if(WIN32)
	target_link_directories(server PRIVATE "C:/Users/JTK6759/Documents/programming/cpp/cppPrograms/socketConnections/build")
	target_link_directories(client PRIVATE "C:/Users/JTK6759/Documents/programming/cpp/cppPrograms/socketConnections/build")
	target_link_directories(serial PRIVATE "C:/Users/JTK6759/Documents/programming/cpp/cppPrograms/socketConnections/build")
	target_link_libraries(server "-lsockConn")
	target_link_libraries(client "-lsockConn")
	target_link_libraries(serial "-lsockConn")
else()
	target_link_libraries(server "-lsockConn")
	target_link_libraries(client "-lsockConn")
	target_link_libraries(serial "-lsockConn")
endif()