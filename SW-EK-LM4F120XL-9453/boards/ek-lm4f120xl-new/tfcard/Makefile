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

VPATH += ../drivers
VPATH += ../../../utils/
#
# Include the common make definitions.
#
include ${ROOT}/makedefs

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
# Rules for building the main example.
#
#${COMPILER}/${PROJ}.axf: ${COMPILER}/src/${PROJ}.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/sd_card.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/startup_${COMPILER}.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/uartstdio.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/cmdline.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/retargetserial.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/fatfs/port/mmc-ek-lm4f232h5qd.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/fatfs/src/ff.o
${COMPILER}/${PROJ}.axf: ${PROJ}.ld

${COMPILER}/${PROJ}.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a
SCATTERgcc_${PROJ}=${PROJ}.ld
ENTRY_${PROJ}=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA1
CFLAGSgcc+=-Dgcc

CFLAGSgcc+=-I./

ifeq (${DEBUG},1)
 CFLAGSgcc+= -g
endif
#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
