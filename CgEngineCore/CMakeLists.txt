cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME CgEngineCore)

set(ENGINE_PUBLIC_INCLUDES 
	includes/CgEngineCore/Application.hpp)
	
set(ENGINE_PRIVATE_SOURCES
	src/CgEngineCore/Application.cpp)

add_library(${ENGINE_PROJECT_NAME} STATIC
	${ENGINE_PRIVATE_SOURCES}
	${ENGINE_PUBLIC_INCLUDES}
	)
	
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)
