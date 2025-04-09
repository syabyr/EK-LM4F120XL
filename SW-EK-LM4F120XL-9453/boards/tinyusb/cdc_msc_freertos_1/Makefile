#******************************************************************************
#
# Makefile - Rules for building the main example.
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


PROJ=main

#
# The base directory for StellarisWare.
#
ROOT=../../..

TINYUSB=${ROOT}/third_party/tinyusb

#
# Include the common make definitions.
#
include ${ROOT}/makedefs-gcc

#
# Where to find header files that do not live in the source directory.
#
IPATH=../../..

#
# The default rule, which causes the main example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/${PROJ}.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

flash:
	@lm4flash ${COMPILER}/${PROJ}.bin
#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the lcdtest example.
#
VPATH += ../../../third_party

${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/tusb.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/common/tusb_fifo.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/device/usbd.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/device/usbd_control.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/audio/audio_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/cdc/cdc_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/dfu/dfu_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/dfu/dfu_rt_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/hid/hid_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/midi/midi_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/msc/msc_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/net/ecm_rndis_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/net/ncm_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/usbtmc/usbtmc_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/video/video_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/class/vendor/vendor_device.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/portable/mentor/musb/dcd_musb.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/tinyusb/src/portable/mentor/musb/hcd_musb.o


${COMPILER}/${PROJ}.axf: ${COMPILER}/src/family.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/main.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/msc_disk.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/usb_descriptors.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/freertos_hook.o

${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/portable/MemMang/heap_4.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/list.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/queue.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/tasks.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/timers.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/croutine.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/event_groups.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/FreeRTOS-Kernel/stream_buffer.o

${COMPILER}/${PROJ}.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a

SCATTERgcc_${PROJ}=./main.ld
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/startup_gcc.o


ENTRY_${PROJ}=ResetISR

CFLAGSgcc = -DTARGET_IS_BLIZZARD_RA1 -DPART_LM4F120H5QR -DTM4C123GH6PM  -DCFG_TUSB_MCU=OPT_MCU_TM4C123


CFLAGSgcc += -ug_pfnVectors -Wno-error=strict-prototypes -Wno-error=cast-qual  -ggdb
CFLAGSgcc += -fdata-sections -ffunction-sections -fsingle-precision-constant
CFLAGSgcc += -fno-strict-aliasing -Wall -Wextra -Werror -Wfatal-errors
CFLAGSgcc += -Wdouble-promotion -Wstrict-prototypes -Wstrict-overflow
CFLAGSgcc += -Werror-implicit-function-declaration -Wfloat-equal -Wundef
CFLAGSgcc += -Wshadow -Wwrite-strings -Wsign-compare -Wmissing-format-attribute
CFLAGSgcc += -Wunreachable-code -Wcast-function-type -Wcast-qual
CFLAGSgcc += -Wnull-dereference -Wuninitialized -Wunused -Wredundant-decls
CFLAGSgcc += -Os -Wno-error=redundant-decls -Wno-error=cast-qual 



CFLAGSgcc += -I${ROOT}/third_party/FreeRTOS-Kernel/include
CFLAGSgcc += -I${ROOT}/third_party/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
CFLAGSgcc += -I${TINYUSB}/src
CFLAGSgcc += -I${TINYUSB}/hw/mcu/ti/tm4c123xx/Include/TM4C123/
CFLAGSgcc += -I./src -I./inc -I./

LDFLAGSgcc =  -DTM4C123GH6PM -flto -mthumb -mabi=aapcs -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGSgcc += -DCFG_TUSB_MCU=OPT_MCU_TM4C123 -ug_pfnVectors -DTM4C123GH6PM
LDFLAGSgcc += -Wno-error=strict-prototypes -Wno-error=cast-qual -ggdb -fdata-sections
LDFLAGSgcc += -ffunction-sections -fsingle-precision-constant -fno-strict-aliasing
LDFLAGSgcc += -Wall -Wextra -Werror -Wfatal-errors -Wdouble-promotion -Wstrict-prototypes
LDFLAGSgcc += -Wstrict-overflow -Werror-implicit-function-declaration -Wfloat-equal
LDFLAGSgcc += -Wundef -Wshadow -Wwrite-strings -Wsign-compare -Wmissing-format-attribute
LDFLAGSgcc += -Wunreachable-code -Wcast-function-type -Wcast-qual
LDFLAGSgcc += -Wnull-dereference -Wuninitialized -Wunused -Wredundant-decls
LDFLAGSgcc += -Os -MD -Wl,-Map=./gcc/${PROJ}.map -Wl,-cref -Wl,-gc-sections
LDFLAGSgcc += -specs=nosys.specs -specs=nano.specs

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
