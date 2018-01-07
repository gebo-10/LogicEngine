cmake_minimum_required(VERSION 3.8)
PROJECT(file)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../interface)

file(GLOB INTERFACE "${PROJECT_SOURCE_DIR}/../../interface/*.h") 
source_group("Interface" FILES ${INTERFACE}) 

file(GLOB CURRENT_HEADERS "*.h") 
source_group("Include" FILES ${CURRENT_HEADERS}) 


AUX_SOURCE_DIRECTORY(. SRC)

#set_property(TARGET my_target PROPERTY CXX_STANDARD 17)
#set_property(TARGET my_target PROPERTY CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED ${INTERFACE} ${CURRENT_HEADERS} ${SRC} )

