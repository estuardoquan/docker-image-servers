cmake_minimum_required(VERSION 3.27.7)

project(main)

add_subdirectory(./options)

add_subdirectory(./yaml-cpp)

add_executable(${PROJECT_NAME}
    print_server.h
    print_server.cpp
    main.cpp
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ./options
        ${YAML_CPP_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE 
        ./options
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        options
        yaml-cpp::yaml-cpp
)