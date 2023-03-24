#******************************************************************************
#
# Makefile - Rules for building the uart_echo example.
#
# Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
# 
# Texas Instruments (TI) is supplying this software for use solely and
# exclusively on TI's microcontroller products. The software is owned by
# TI and/or its suppliers, and is protected under applicable copyright
# laws. You may not combine this software with "viral" open-source
# software in order to form a larger program.
# 
# THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
# NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
# NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
# CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
# DAMAGES, FOR ANY REASON WHATSOEVER.
# 
# This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
#
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=LM4F120H5QR

#
# Set the processor variant.
#
VARIANT=cm4f

PWD := $(shell pwd)/
#
# The base directory for StellarisWare.
#
ROOT := ${PWD}/../../../

TINYUSB=${ROOT}/third_party/tinyusb

#
# Include the common make definitions.
#
include ./makedefs


PROJ=cdc_msc_freertos

#
# Where to find header files that do not live in the source directory.
#
IPATH=${ROOT}

#
# The default rule, which causes the lcdtest example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/${PROJ}.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

flash:
	lm4flash ${COMPILER}/${PROJ}.bin
#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the lcdtest example.
#

${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/tusb.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/common/tusb_fifo.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/device/usbd.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/device/usbd_control.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/audio/audio_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/cdc/cdc_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/dfu/dfu_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/dfu/dfu_rt_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/hid/hid_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/midi/midi_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/msc/msc_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/net/ecm_rndis_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/net/ncm_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/usbtmc/usbtmc_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/video/video_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/class/vendor/vendor_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/portable/mentor/musb/dcd_musb.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/src/portable/mentor/musb/hcd_musb.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/hw/bsp/tm4c123/family.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/hw/bsp/board.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/hw/mcu/ti/tm4c123xx/Source/system_TM4C123.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/hw/mcu/ti/tm4c123xx/Source/GCC/tm4c123_startup.o

${COMPILER}/${PROJ}.axf: ${COMPILER}/src/main.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/msc_disk.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/usb_descriptors.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/freertos_hook.o

${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/FreeRTOS-Kernel/list.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/FreeRTOS-Kernel/queue.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/FreeRTOS-Kernel/tasks.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/FreeRTOS-Kernel/timers.o

${COMPILER}/${PROJ}.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a
#${COMPILER}/${PROJ}.axf: ${TINYUSB}/hw/bsp/tm4c123/boards/ek-tm4c123gxl/tm4c123.ld
SCATTERgcc_${PROJ}=${TINYUSB}/hw/bsp/tm4c123/boards/ek-tm4c123gxl/tm4c123.ld
ENTRY_${PROJ}=Reset_Handler
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA1 -DPART_LM4F120H5QR -DTM4C123GH6PM  -DCFG_TUSB_MCU=OPT_MCU_TM4C123

CFLAGSgcc += -I${TINYUSB}/hw
CFLAGSgcc += -I${TINYUSB}/src
CFLAGSgcc += -I${TINYUSB}/hw/bsp/tm4c123/boards/ek-tm4c123gxl
CFLAGSgcc += -I${TINYUSB}/hw/mcu/ti/tm4c123xx/Include/TM4C123/
CFLAGSgcc += -I./src -I./src/FreeRTOSConfig
CFLAGSgcc += -I${TINYUSB}/lib/FreeRTOS-Kernel/include
CFLAGSgcc += -I${TINYUSB}/lib/FreeRTOS-Kernel/portable/GCC/ARM_CM4F

CFLAGSgcc += -I${TINYUSB}/hw/mcu/ti/tm4c123xx/CMSIS/5.7.0/CMSIS/Include

LDFLAGSgcc =

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
