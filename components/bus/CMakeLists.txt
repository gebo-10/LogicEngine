cmake_minimum_required(VERSION 3.8)
PROJECT(comp_bus)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../interface)


file(GLOB INTERFACE "${PROJECT_SOURCE_DIR}/../../interface/*.h") 
source_group("Interface" FILES ${INTERFACE}) 

file(GLOB CURRENT_HEADERS "*.h") 
source_group("Include" FILES ${CURRENT_HEADERS}) 


AUX_SOURCE_DIRECTORY(. SRC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)
#set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/../../bin)
#set_property(TARGET my_target PROPERTY CXX_STANDARD 17)
#set_property(TARGET my_target PROPERTY CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED ${INTERFACE} ${CURRENT_HEADERS} ${SRC} )
#install (TARGETS comp_bus DESTINATION ${PROJECT_SOURCE_DIR}/../../bin)