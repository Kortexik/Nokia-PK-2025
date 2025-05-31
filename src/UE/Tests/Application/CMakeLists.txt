project(UeApplicationUT)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Mocks SRC_LIST)
aux_source_directory(Ports SRC_LIST)
include_directories(${COMMON_DIR}/Tests)
include_directories(${UE_DIR}/Tests)

add_executable(${PROJECT_NAME} ${SRC_LIST}
        Mocks/IMockSmsDb.hpp
        Mocks/IMockSmsDb.cpp)
target_link_libraries(${PROJECT_NAME} UeApplication)
target_link_libraries(${PROJECT_NAME} CommonUtMocks)
target_link_gtest()





