project(UeApplication)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Ports SRC_LIST)
aux_source_directory(States SRC_LIST)

add_library(${PROJECT_NAME} ${SRC_LIST}
        Sms.cpp
        Sms.cpp
        Sms.hpp
        SmsDb.cpp
        SmsDb.hpp
        States/SmsListState.cpp
        States/SmsListState.hpp
        States/ViewSmsContentState.cpp
        States/ViewSmsContentState.hpp
        States/SmsComposeState.cpp
        States/SmsComposeState.hpp
)
target_link_libraries(${PROJECT_NAME} Common)
