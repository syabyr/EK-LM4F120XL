# driverlib CMake file
project(tinyusb)

# find . -name '*.c' | sort -u
add_library(${PROJECT_NAME}
./hw/bsp/board.c
./src/tusb.c
./src/common/tusb_fifo.c
./src/device/usbd.c
./src/device/usbd_control.c
./src/class/audio/audio_device.c
./src/class/cdc/cdc_device.c
./src/class/dfu/dfu_device.c
./src/class/dfu/dfu_rt_device.c
./src/class/hid/hid_device.c
./src/class/midi/midi_device.c
./src/class/msc/msc_device.c
./src/class/net/ecm_rndis_device.c
./src/class/net/ncm_device.c
./src/class/usbtmc/usbtmc_device.c
./src/class/video/video_device.c
./src/class/vendor/vendor_device.c
./src/portable/mentor/musb/dcd_musb.c
./src/portable/mentor/musb/hcd_musb.c)

target_include_directories(${PROJECT_NAME} PUBLIC ../../
                                ./lib/networking
                                ./src
                                ../FreeRTOS-Kernel/include
                                ../FreeRTOS-Kernel/portable/GCC/ARM_CM4F
                                ${WORKING_BASE_DIRECTORY}
                                ${WORKING_BASE_DIRECTORY}/inc/
                                )

#target_link_libraries(${PROJECT_NAME} device)