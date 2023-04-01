# driverlib CMake file
project(freertos_kernel)

# find . -name '*.c' | sort -u
add_library(${PROJECT_NAME}
        croutine.c
        event_groups.c
        list.c
        queue.c
        stream_buffer.c
        tasks.c
        timers.c
        portable/GCC/ARM_CM4F/port.c)

target_include_directories(${PROJECT_NAME} PUBLIC ../../
                                ./include
                                ./portable/GCC/ARM_CM4F
                                ${WORKING_BASE_DIRECTORY}
                                ${WORKING_BASE_DIRECTORY}/inc/
                                )

#target_link_libraries(${PROJECT_NAME} device)