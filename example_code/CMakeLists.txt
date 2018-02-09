cmake_minimum_required(VERSION 3.5)
project(AsioTest)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system thread regex)
find_package( OpenCV REQUIRED )

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

add_executable(AsioTCPServer AsioTCPServer.cpp)
add_executable(AsioTCPClient AsioTCPClient.cpp)
add_executable(AsyncUDPReceiver AsyncUDPRecvTester.cpp)
add_executable(AsyncUDPSender AsyncUDPSendTester.cpp)
target_link_libraries(AsioTCPClient ${USED_LIBS} ${OpenCV_LIBS})
target_link_libraries(AsioTCPServer ${USED_LIBS} ${OpenCV_LIBS})
target_link_libraries(AsyncUDPReceiver ${USED_LIBS} ${OpenCV_LIBS})
target_link_libraries(AsyncUDPSender ${USED_LIBS} ${OpenCV_LIBS})