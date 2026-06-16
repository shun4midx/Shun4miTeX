cmake_minimum_required(VERSION 3.22)
project(shun4mitex VERSION 1.0 DESCRIPTION "A Discord bot Shun4miTeX")

add_executable(${PROJECT_NAME}
    bot/bot.cpp
    bot/env_parser/env_parser.cpp
    src/latex_render.cpp
)

find_package(PkgConfig REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(DPP REQUIRED)
target_link_libraries(shun4mitex PRIVATE ${DPP_LIBRARIES})
target_include_directories(shun4mitex PRIVATE ${DPP_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
