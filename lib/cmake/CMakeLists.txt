project(Tonic)
cmake_minimum_required(VERSION 2.8)
message(${CMAKE_CURRENT_SOURCE_DIR})
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../src/Tonic SRC_LIST)
add_library(TonicLib ${SRC_LIST})

FILE(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/../../src/Tonic/*.h")
message(${headers})
INSTALL(FILES ${headers} DESTINATION include/Tonic)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Tonic.h DESTINATION include)

INSTALL(TARGETS TonicLib ARCHIVE DESTINATION lib)
