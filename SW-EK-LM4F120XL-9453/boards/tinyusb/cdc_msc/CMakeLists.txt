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
    set(CMAKE_C_${COMPILER_PREFIX}/COMPILER arm-none-eabi-gcc)
    set(CMAKE_AR ${COMPILER_PREFIX}/arm-none-eabi-ar)
endif ()


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


set(TINYUSB_LOCATION /Users/mybays/develop/protocol/usb/tinyusb)


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
project(fulltest C CXX ASM)

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

#add_subdirectory(${BASE_LOCATION}/driverlib driverlib)


set(LINKER_SCRIPT "../bsp/tm4c123/boards/ek-tm4c123gxl/tm4c123.ld")

# This is normally set in lm4f120-base.cmake, but libraries may modify it so set
# it after libraries/subdirectories have been added
set(CMAKE_EXE_LINKER_FLAGS "-DTM4C123GH6PM -flto -mthumb -mabi=aapcs -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DCFG_TUSB_MCU=OPT_MCU_TM4C123 -uvectors -DTM4C123GH6PM -Wno-error=strict-prototypes -Wno-error=cast-qual  -ggdb -fdata-sections -ffunction-sections -fsingle-precision-constant -fno-strict-aliasing -Wall -Wextra -Werror -Wfatal-errors -Wdouble-promotion -Wstrict-prototypes -Wstrict-overflow -Werror-implicit-function-declaration -Wfloat-equal -Wundef -Wshadow -Wwrite-strings -Wsign-compare -Wmissing-format-attribute -Wunreachable-code -Wcast-align -Wcast-function-type -Wcast-qual -Wnull-dereference -Wuninitialized -Wunused -Wredundant-decls -Os -MD -Wl,-Map=.map -Wl,-cref -Wl,-gc-sections -Wl,-T,/Users/mybays/develop/mcu-arch/lm4f120/EK-LM4F120XL/SW-EK-LM4F120XL-9453/boards/tinyusb-lm4s/hw/bsp/tm4c123/boards/ek-tm4c123gxl/tm4c123.ld -specs=nosys.specs -specs=nano.specs")
##### Header & Source Files #####

include_directories(${BASE_LOCATION})

include_directories(${TINYUSB_LOCATION}/src)
include_directories(${TINYUSB_LOCATION}/hw)
include_directories(./src)
include_directories(${TINYUSB_LOCATION}/hw/mcu/ti/tm4c123xx/Include/TM4C123/)
include_directories(${TINYUSB_LOCATION}/hw/mcu/ti/tm4c123xx/CMSIS/5.7.0/CMSIS/Include/)
include_directories(${TINYUSB_LOCATION}/hw/bsp/tm4c123/boards/ek-tm4c123gxl/)
add_definitions("-DCFG_TUSB_MCU=OPT_MCU_TM4C123")
add_definitions("-DTM4C123GH6PM")

set(SOURCE_FILES  src/main.c
                  src/msc_disk.c
                  src/usb_descriptors.c
                  ${TINYUSB_LOCATION}/hw/bsp/tm4c123/family.c
                  ${TINYUSB_LOCATION}/hw/bsp/board.c
                  ${TINYUSB_LOCATION}/hw/mcu/ti/tm4c123xx/Source/system_TM4C123.c
                  ${TINYUSB_LOCATION}/hw/mcu/ti/tm4c123xx/Source/GCC/tm4c123_startup.c
                  ${TINYUSB_LOCATION}/src/tusb.c
                  ${TINYUSB_LOCATION}/src/common/tusb_fifo.c
                  ${TINYUSB_LOCATION}/src/device/usbd.c
                  ${TINYUSB_LOCATION}/src/device/usbd_control.c
                  ${TINYUSB_LOCATION}/src/class/audio/audio_device.c
                  ${TINYUSB_LOCATION}/src/class/cdc/cdc_device.c
                  ${TINYUSB_LOCATION}/src/class/dfu/dfu_device.c
                  ${TINYUSB_LOCATION}/src/class/dfu/dfu_rt_device.c
                  ${TINYUSB_LOCATION}/src/class/hid/hid_device.c
                  ${TINYUSB_LOCATION}/src/class/midi/midi_device.c
                  ${TINYUSB_LOCATION}/src/class/msc/msc_device.c
                  ${TINYUSB_LOCATION}/src/class/net/ecm_rndis_device.c
                  ${TINYUSB_LOCATION}/src/class/net/ncm_device.c
                  ${TINYUSB_LOCATION}/src/class/usbtmc/usbtmc_device.c
                  ${TINYUSB_LOCATION}/src/class/video/video_device.c
                  ${TINYUSB_LOCATION}/src/class/vendor/vendor_device.c
                  ${TINYUSB_LOCATION}/src/portable/mentor/musb/dcd_musb.c
                  ${TINYUSB_LOCATION}/src/portable/mentor/musb/hcd_musb.c)



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

#target_link_libraries(${PROJECT_NAME} driver-cm4f)

##### Post build #####

# Add post build commands
include(${BASE_LOCATION}/toolchain/post-build.cmake)

# Add lm4flash commands
include(${BASE_LOCATION}/toolchain/lm4flash.cmake)

##### CMake debug prints #####
if (CMAKE_VERBOSE_MAKEFILE)
    print_debug_info()
endif()
