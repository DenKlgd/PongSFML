cmake_minimum_required(VERSION 3.8)

project(Ping-Pong)

set(SFML_ROOT "C:/SFML-2.5.1/")
set(SFML_DIR "${SFML_ROOT}lib/cmake/SFML")

set(PONG_SOURCES 
			Main.cpp
			Ping-Pong.cpp
			Objects2D.cpp
			Physics2D.cpp
			WindowInputHandlers.cpp
			WindowInputHandlers.h 
			GUI.cpp
			GUI.h 
			Point2D.h
			PongMenues.cpp
			PongMenues.h 
			Network.cpp
			Network.h 
			GameStates.h
			GameStates.cpp
			Connection.h
			ScreenResolutions.h 
			ScreenResolutions.cpp
			)

if (WIN32)
	add_executable(Pong WIN32 ${PONG_SOURCES})
else ()
	add_executable(Pong ${PONG_SOURCES})
endif()

include_directories(${SFML_ROOT}include)

find_package(SFML 2.5 COMPONENTS graphics network REQUIRED)

target_link_libraries(Pong sfml-graphics sfml-network)
