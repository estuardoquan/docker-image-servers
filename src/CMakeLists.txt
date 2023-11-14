cmake_minimum_required(VERSION 3.27.7)

project(main)

add_subdirectory(./opts)

add_executable(${PROJECT_NAME}
    main.cpp
    server.cpp server.h
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ./opts
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC ./opts
)

target_link_libraries(${PROJECT_NAME} 
    checkopt
)
