cmake_minimum_required(VERSION 3.0)
project(BjutNetConsole)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost REQUIRED COMPONENTS system)

include_directories(
	../BjutNetService/
	${Boost_INCLUDE_DIRS}
	)

add_executable(BjutNetConsole
	../BjutNetService/ParamParse.cpp
	../BjutNetService/MessageValue.cpp
	UdpSocket.cpp
	ServiceBridge.cpp
	InterAction.cpp
	main.cpp
	)

target_link_libraries(BjutNetConsole boost_system)
