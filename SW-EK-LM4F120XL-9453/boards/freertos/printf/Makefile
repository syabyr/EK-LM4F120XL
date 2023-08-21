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

VPATH += ../../../third_party

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

${COMPILER}/${PROJ}.axf: ${COMPILER}/src/${PROJ}.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/retargetserial.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/startup_${COMPILER}.o

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

${COMPILER}/${PROJ}.axf: ${PROJ}.ld

${COMPILER}/${PROJ}.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a
SCATTERgcc_${PROJ}=${PROJ}.ld
ENTRY_${PROJ}=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA1
CFLAGSgcc+=-Dgcc

CFLAGSgcc+=-ug_pfnVectors -Wno-error=strict-prototypes -Wno-error=cast-qual
CFLAGSgcc+=-fdata-sections -ffunction-sections -fsingle-precision-constant
CFLAGSgcc+=-fno-strict-aliasing -Wall -Wextra -Werror -Wfatal-errors
CFLAGSgcc+=-Wdouble-promotion -Wstrict-prototypes -Wstrict-overflow
CFLAGSgcc+=-Werror-implicit-function-declaration -Wfloat-equal
CFLAGSgcc+=-Wundef -Wshadow -Wwrite-strings -Wsign-compare
CFLAGSgcc+=-Wmissing-format-attribute -Wunreachable-code
CFLAGSgcc+=-Wcast-function-type -Wcast-qual
CFLAGSgcc+=-Wnull-dereference -Wuninitialized -Wunused
CFLAGSgcc+=-Wredundant-decls -Os -Wno-error=redundant-decls -Wno-error=cast-qual


CFLAGSgcc += -I./inc
CFLAGSgcc += -I${ROOT}/third_party/FreeRTOS-Kernel/include
CFLAGSgcc += -I${ROOT}/third_party/FreeRTOS-Kernel/portable/GCC/ARM_CM4F

ifeq (${DEBUG},1)
 CFLAGSgcc+= -g
endif


LDFLAGSgcc+=-flto -mthumb -mabi=aapcs -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGSgcc+=-ug_pfnVectors
LDFLAGSgcc+=-Wno-error=strict-prototypes -Wno-error=cast-qual -fdata-sections
LDFLAGSgcc+=-ffunction-sections -fsingle-precision-constant -fno-strict-aliasing
LDFLAGSgcc+=-Wall -Wextra -Werror -Wfatal-errors -Wdouble-promotion
LDFLAGSgcc+=-Wstrict-prototypes -Wstrict-overflow -Werror-implicit-function-declaration
LDFLAGSgcc+=-Wfloat-equal -Wundef -Wshadow -Wwrite-strings -Wsign-compare
LDFLAGSgcc+=-Wmissing-format-attribute -Wunreachable-code -Wcast-align
LDFLAGSgcc+=-Wcast-function-type -Wcast-qual -Wnull-dereference -Wuninitialized
LDFLAGSgcc+=-Wunused -Wredundant-decls -Os -MD -Wl,-Map=./gcc/build.map
LDFLAGSgcc+=-Wl,-cref -Wl,-gc-sections  -specs=nosys.specs

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
