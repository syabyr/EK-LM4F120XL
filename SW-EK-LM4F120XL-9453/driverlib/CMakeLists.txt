# driverlib CMake file
project(driver-cm4f)

# find . -name '*.c' | sort -u
add_library(${PROJECT_NAME}
        adc.c
        can.c
        comp.c
        cpu.c
        eeprom.c
        epi.c
        ethernet.c
        fan.c
        flash.c
        fpu.c
        gpio.c
        hibernate.c
        i2c.c
        i2s.c
        interrupt.c
        lpc.c
        mpu.c
        peci.c
        pwm.c
        qei.c
        ssi.c
        sysctl.c
        sysexc.c
        systick.c
        timer.c
        uart.c
        udma.c
        usb.c
        watchdog.c)

target_include_directories(${PROJECT_NAME} PUBLIC ../)

#target_link_libraries(${PROJECT_NAME} device)