cmake_minimum_required(VERSION 3.8)
PROJECT(launcher)
set(BOOST_ROOT E:/boost/boost_1_66_0)
#D:/boost/bin/boost_1_66_0
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../interface)
INCLUDE_DIRECTORIES(${BOOST_ROOT})
file(GLOB INTERFACE "${PROJECT_SOURCE_DIR}/../interface/*.h") 
source_group("Interface" FILES ${INTERFACE}) 

file(GLOB CURRENT_HEADERS "*.h") 
source_group("Include" FILES ${CURRENT_HEADERS}) 

link_directories(${BOOST_ROOT}/lib64-msvc-14.1)  
AUX_SOURCE_DIRECTORY(. SRC)

#set_property(TARGET my_target PROPERTY CXX_STANDARD 17)
#set_property(TARGET my_target PROPERTY CXX_STANDARD_REQUIRED ON)

ADD_EXECUTABLE(${PROJECT_NAME} ${INTERFACE} ${CURRENT_HEADERS} ${SRC} )
file(GLOB BOOST_LIB "${BOOST_ROOT}/lib64-msvc-14.1/*.lib")  
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${BOOST_LIB})  

