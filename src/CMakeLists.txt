cmake_minimum_required(VERSION 3.26.5)

project(camarero)

add_subdirectory(./ext/clib/options)

add_subdirectory(./ext/yaml-cpp)

add_executable(${PROJECT_NAME}
    main.cpp
    server_cli.cpp server_cli.cpp
    server_yml.cpp server_yml.h
    server.cpp server.h
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ./ext/clib/options
        ./ext/yaml-cpp
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE 
        ./ext/clib/options
        ./ext/yaml-cpp
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        options
        yaml-cpp::yaml-cpp
)