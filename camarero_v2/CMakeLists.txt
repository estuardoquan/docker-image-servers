cmake_minimum_required(VERSION 3.26.5)

project(camarero)

add_subdirectory(./ext/libc/options)

add_subdirectory(./ext/yaml-cpp)

add_executable(${PROJECT_NAME}
    ./src/main.cpp
    ./src/server_cli.cpp ./src/server_cli.h
    ./src/server_yml.cpp ./src/server_yml.h
    ./src/server.cpp ./src/server.h
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ./ext/libc/options
        ./ext/yaml-cpp
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE 
        ./ext/libc/options
        ./ext/yaml-cpp
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        options
        yaml-cpp::yaml-cpp
)
