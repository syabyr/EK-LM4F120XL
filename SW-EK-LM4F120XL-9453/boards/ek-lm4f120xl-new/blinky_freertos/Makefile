#******************************************************************************
#
# Makefile - Rules for building the blinky example.
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

#
# The base directory for StellarisWare.
#
ROOT=../../..

#
# Include the common make definitions.
#
include ./makedefs

#
# Where to find header files that do not live in the source directory.
#
IPATH=../../..

#
# The default rule, which causes the blinky example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/blinky.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}


ENTRY_blinky=ResetISR

${COMPILER}/blinky.axf: ${COMPILER}/startup_${COMPILER}.o
SCATTERgcc_blinky=blinky.ld



${COMPILER}/blinky.axf: ${COMPILER}/blinky.o
${COMPILER}/blinky.axf: ${COMPILER}/src/freertos_hook.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/list.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/queue.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/tasks.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/timers.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/croutine.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/event_groups.o
${COMPILER}/blinky.axf: ${COMPILER}/FreeRTOS-Kernel/stream_buffer.o


${COMPILER}/blinky.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a


CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA1
CFLAGSgcc+=-Dgcc

CFLAGSgcc += -I./src -I./src/FreeRTOSConfig
CFLAGSgcc += -I./FreeRTOS-Kernel/include
CFLAGSgcc += -I./FreeRTOS-Kernel/portable/GCC/ARM_CM4F

CFLAGSgcc += -I./src -I./src/FreeRTOSConfig
CFLAGSgcc += -I./FreeRTOS-Kernel/include
CFLAGSgcc += -I./FreeRTOS-Kernel/portable/GCC/ARM_CM4F

ifeq (${DEBUG},1)
 CFLAGSgcc+= -g
endif
#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
