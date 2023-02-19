cmake_minimum_required(VERSION 3.8)

project(Ping-Pong)

add_executable(Pong WIN32 
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
			PongInput.h 
			PongInput.cpp
			GameStates.h
			GameStates.cpp
			Connection.h
			)

include_directories(C:/SFML-2.5.1/include)

#target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-system-d.lib)
#target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-window-d.lib)
#target_link_libraries(Pong C:/SFML-2.5.1/lib/freetype.lib)
#target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-graphics-d.lib)
#target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-network-d.lib)

target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-system.lib)
target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-window.lib)
target_link_libraries(Pong C:/SFML-2.5.1/lib/freetype.lib)
target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-graphics.lib)
target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-network.lib)

#target_link_libraries(Pong C:/SFML-2.5.1/lib/openal32.lib)
#target_link_libraries(Pong C:/SFML-2.5.1/lib/sfml-audio.lib)
