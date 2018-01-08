cmake_minimum_required(VERSION 3.8)
PROJECT(lua)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../interface)
include_directories(lua)

file(GLOB INTERFACE "${PROJECT_SOURCE_DIR}/../../interface/*.h") 
source_group("Interface" FILES ${INTERFACE}) 

file(GLOB CURRENT_HEADERS "*.h") 
source_group("Include" FILES ${CURRENT_HEADERS}) 


AUX_SOURCE_DIRECTORY(. SRC)

#set_property(TARGET my_target PROPERTY CXX_STANDARD 17)
#set_property(TARGET my_target PROPERTY CXX_STANDARD_REQUIRED ON)
add_subdirectory(lua)
add_library(${PROJECT_NAME} SHARED ${INTERFACE} ${CURRENT_HEADERS} ${SRC} )
target_link_libraries(${PROJECT_NAME} Lua)

