cmake_minimum_required(VERSION 3.27.7)

project(server)

add_executable(${PROJECT_NAME} server.cpp)

# Add <optcheck.h> lib

add_subdirectory(lib/optcheck)

target_include_directories(${PROJECT_NAME} PUBLIC lib/optcheck)

target_link_directories(${PROJECT_NAME} PRIVATE lib/optcheck)

target_link_libraries(${PROJECT_NAME} optcheck)

# Additional lib
