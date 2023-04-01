# Base EK-LM4F120XL CMake file
#
#
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

###### Project Environment #####

# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)

#set(COMPILER_PREFIX /Users/mybays/develop/efr32/toolchains/gnu_arm/7.2_2017q4/bin/)

if(NOT COMPILER_PREFIX)
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_AR arm-none-eabi-ar)
else ()
    set(CMAKE_C_COMPILER ${COMPILER_PREFIX}/arm-none-eabi-gcc)
    set(CMAKE_AR ${COMPILER_PREFIX}/arm-none-eabi-ar)
endif ()

set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../inc)

# Optional verbose mode, uncomment or pass in -DCMAKE_VERBOSE_MAKEFILE=ON
set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_BUILD_TYPE DEBUG)


# Set build
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()
message ( STATUS "BUILD_TYPE = ${CMAKE_BUILD_TYPE}.")

set(BASE_LOCATION ../../..)
message ( STATUS "BASE_LOCATION = ${BASE_LOCATION}.")

set(BOARD EK-LM4F120X)

# Set device
if (BOARD STREQUAL "EK-LM4F120X" )
    set(DEVICE "LM4F120H5QR")
    set(JLINK_TYPE "LM4F120H5QR")
elseif (BOARD STREQUAL "EK-MSP432P401R")
    set(DEVICE "MSP432P401R")
    set(JLINK_TYPE "MSP432P401R")
else ()
    message("Unknown Board type:${BOARD}")
    exit()
endif ()

message("jlinkdevice: ${JLINK_TYPE}")

# Set the compiler (must be prior to project setup)
include(${BASE_LOCATION}/toolchain/arm-gcc.cmake)

##### Project Setup #####

# Configure project and languages
project(main C CXX ASM)

# Set device
if (NOT DEVICE)
    set(DEVICE LM4F120H5QR)
endif ()

set(CPU_TYPE "m4")




##### Modules #####

# Libraries can be added to the LIBS variable
# or manually included here.

# Add base libs (emlib, CMSIS, device files)
include(${BASE_LOCATION}/toolchain/lm4f120-base.cmake)

add_subdirectory(${BASE_LOCATION}/driverlib driverlib)

add_subdirectory(${BASE_LOCATION}/third_party/FreeRTOS-Kernel FreeRTOS-Kernel)

set(LINKER_SCRIPT "../main.ld") 

# This is normally set in lm4f120-base.cmake, but libraries may modify it so set
# it after libraries/subdirectories have been added
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_DEFINITIONS} -Xlinker -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,-v --entry ResetISR -u _printf_float")

##### Header & Source Files #####

include_directories(${BASE_LOCATION}
                    ./inc
                    ${BASE_LOCATION}/third_party/FreeRTOS-Kernel/include
                    ${BASE_LOCATION}/third_party/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
                    )


#aux_source_directory(./ SOURCE_FILES)
set(SOURCE_FILES  src/startup_gcc.c
                  src/freertos_hook.c
                  src/main.c)



#printf float support
#link_libraries("-u _printf_float")

# Generate executable and link
add_executable(${PROJECT_NAME}
	${SOURCE_FILES})

stm32_configure_linker_addresses(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${LIBS} c nosys)

# Link optional libraries if available
#if (${HAS_HARDWARE})
#    target_link_libraries(${PROJECT_NAME} hardware)
#endif ()

target_link_libraries(${PROJECT_NAME} driver-cm4f freertos_kernel)

##### Post build #####

# Add post build commands
include(${BASE_LOCATION}/toolchain/post-build.cmake)

# Add lm4flash commands
include(${BASE_LOCATION}/toolchain/lm4flash.cmake)

##### CMake debug prints #####
if (CMAKE_VERBOSE_MAKEFILE)
    print_debug_info()
endif()
