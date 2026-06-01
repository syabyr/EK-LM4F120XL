# driverlib CMake file
project(drivers)

# find . -name '*.c' | sort -u
add_library(${PROJECT_NAME}
        retargetserial.c)

target_include_directories(${PROJECT_NAME} PUBLIC ../../../)

#target_link_libraries(${PROJECT_NAME} device)