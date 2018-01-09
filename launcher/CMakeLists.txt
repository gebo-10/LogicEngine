cmake_minimum_required(VERSION 3.8)
PROJECT(launcher)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../interface)
INCLUDE_DIRECTORIES(D:/boost/bin/boost_1_66_0)
file(GLOB INTERFACE "${PROJECT_SOURCE_DIR}/../interface/*.h") 
source_group("Interface" FILES ${INTERFACE}) 

file(GLOB CURRENT_HEADERS "*.h") 
source_group("Include" FILES ${CURRENT_HEADERS}) 


AUX_SOURCE_DIRECTORY(. SRC)

#set_property(TARGET my_target PROPERTY CXX_STANDARD 17)
#set_property(TARGET my_target PROPERTY CXX_STANDARD_REQUIRED ON)

ADD_EXECUTABLE(${PROJECT_NAME} ${INTERFACE} ${CURRENT_HEADERS} ${SRC} )
file(GLOB BOOST_LIB "D:/boost/bin/boost_1_66_0/lib64-msvc-14.1/*.lib")  
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${BOOST_LIB})  

