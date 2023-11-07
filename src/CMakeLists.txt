cmake_minimum_required(VERSION 3.27.7)

project(server)

add_executable(${PROJECT_NAME} server.cpp)

add_subdirectory(optcheck)

target_include_directories(${PROJECT_NAME} 
    PUBLIC optcheck
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC optcheck
)

target_link_libraries(${PROJECT_NAME} optcheck)
