#指定cmake版本 
cmake_minimum_required(VERSION 3.8)

#指定项目的名称，一般和项目的文件夹名称对应
PROJECT(LogicEngine)

INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(interface)

file(GLOB CURRENT_HEADERS "interface/*.h") 
source_group("Include" FILES ${CURRENT_HEADERS}) 


SET(MAIN main.cpp)

AUX_SOURCE_DIRECTORY(src SRC)



ADD_EXECUTABLE(${PROJECT_NAME} ${MAIN} ${SRC} ${CURRENT_HEADERS} )

#set_target_properties(${PROJECT_NAME}
#    PROPERTIES
#     LINK_FLAGS /SUBSYSTEM:WINDOWS
#     )

