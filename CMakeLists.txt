cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME CgEngine)

add_subdirectory(CgEditor)
add_subdirectory(CgEngineCore)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CgEditor)