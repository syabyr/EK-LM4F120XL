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
include ${ROOT}/makedefs-gcc


PROJ=main

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


${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/autoip.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/dhcp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/etharp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/icmp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/igmp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/ip4_addr.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/ip4.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv4/ip4_frag.o

${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/dhcp6.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/ethip6.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/icmp6.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/inet6.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/ip6_addr.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/ip6.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/ip6_frag.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/mld6.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ipv6/nd6.o

${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/altcp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/altcp_alloc.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/altcp_tcp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/def.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/dns.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/inet_chksum.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/init.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/ip.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/mem.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/memp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/netif.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/pbuf.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/raw.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/stats.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/sys.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/tcp_in.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/tcp_out.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/tcp.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/timeouts.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/core/udp.o


${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/netif/ethernet.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/netif/slipif.o


${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/apps/http/httpd.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/lwip/src/apps/http/fs.o

${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/networking/dhserver.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/networking/dnserver.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/${TINYUSB}/lib/networking/rndis_reports.o

${COMPILER}/${PROJ}.axf: ${COMPILER}/src/family.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/main.o
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/usb_descriptors.o

${COMPILER}/${PROJ}.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a

SCATTERgcc_${PROJ}=./main.ld
${COMPILER}/${PROJ}.axf: ${COMPILER}/src/startup_gcc.o


ENTRY_${PROJ}=ResetISR
CFLAGSgcc = -DTARGET_IS_BLIZZARD_RA1 -DPART_LM4F120H5QR -DTM4C123GH6PM
CFLAGSgcc += -DCFG_TUSB_MCU=OPT_MCU_TM4C123
CFLAGSgcc += 
CFLAGSgcc += -I./src -I${TINYUSB}/src
CFLAGSgcc += -I./inc
CFLAGSgcc += -I${TINYUSB}/lib/lwip/src/include
CFLAGSgcc += -I${TINYUSB}/lib/lwip/src/include/ipv4
CFLAGSgcc += -I${TINYUSB}/lib/lwip/src/include/lwip/apps
CFLAGSgcc += -I${TINYUSB}/lib/networking

LDFLAGSgcc =  -DTM4C123GH6PM -flto -mthumb -mabi=aapcs -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGSgcc += -DCFG_TUSB_MCU=OPT_MCU_TM4C123 -ug_pfnVectors -DTM4C123GH6PM
LDFLAGSgcc += -Wno-error=strict-prototypes -Wno-error=cast-qual -ggdb -fdata-sections
LDFLAGSgcc += -ffunction-sections -fsingle-precision-constant -fno-strict-aliasing
LDFLAGSgcc += -Wall -Wextra -Werror -Wfatal-errors -Wdouble-promotion -Wstrict-prototypes
LDFLAGSgcc += -Wstrict-overflow -Werror-implicit-function-declaration -Wfloat-equal
LDFLAGSgcc += -Wundef -Wshadow -Wwrite-strings -Wsign-compare -Wmissing-format-attribute
LDFLAGSgcc += -Wunreachable-code -Wcast-align -Wcast-function-type -Wcast-qual
LDFLAGSgcc += -Wnull-dereference -Wuninitialized -Wunused -Wredundant-decls
LDFLAGSgcc += -Os -MD -Wl,-Map=./gcc/${PROJ}.map -Wl,-cref -Wl,-gc-sections
LDFLAGSgcc += -specs=nosys.specs -specs=nano.specs

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
