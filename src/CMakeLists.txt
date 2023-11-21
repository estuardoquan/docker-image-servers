cmake_minimum_required(VERSION 3.27.7)

project(main)

add_subdirectory(./options)

add_subdirectory(./charsum)

add_subdirectory(./yaml-cpp)

add_executable(${PROJECT_NAME}
    main.cpp
    server_cli.cpp server_cli.cpp
    server_yml.cpp server_yml.h
    server.cpp server.h
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ./options
        ./charsum
    # ${YAML_CPP_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE 
        ./options
        ./charsum
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        options
        charsum
        yaml-cpp::yaml-cpp
)