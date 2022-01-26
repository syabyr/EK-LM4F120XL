# driverlib CMake file
project(usb-cm4f)

cmake_minimum_required(VERSION 2.8.4)

# find . -name '*.c' | sort -u
add_library(${PROJECT_NAME}
        device/usbdaudio.c
        device/usbdcdc.c
        device/usbdcomp.c
        device/usbddfu-rt.c
        device/usbdhandler.c
        device/usbdhidkeyb.c
        device/usbdmsc.c
        device/usbdbulk.c
        device/usbdcdesc.c
        device/usbdconfig.c
        device/usbdenum.c
        device/usbdhid.c
        device/usbdhidmouse.c
        host/usbhaudio.c
        host/usbhhidkeyboard.c
        host/usbhhub.c
        host/usbhostenum.c
        host/usbhhid.c
        host/usbhhidmouse.c
        host/usbhmsc.c
        host/usbhscsi.c
        usbbuffer.c
        usbdesc.c
        usbkeyboardmap.c
        usbmode.c
        usbringbuf.c
        usbtick.c)

target_include_directories(${PROJECT_NAME} PUBLIC ../)

#target_link_libraries(${PROJECT_NAME} device)