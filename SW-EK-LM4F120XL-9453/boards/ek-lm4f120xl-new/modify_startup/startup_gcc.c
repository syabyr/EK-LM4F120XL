//*****************************************************************************
//
// startup_gcc.c - Startup code for use with GNU tools.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
//
//*****************************************************************************

#include "inc/hw_nvic.h"
#include "inc/hw_types.h"

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

void HardFaultHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void BusFaultHandler        (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UsageFaultHandler      (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void SVCallHandler          (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void DebugMonitorHandler    (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PendSVHandler          (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void SysTickHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOAHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOBHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOCHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIODHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOEHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void SSI0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void I2C0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PMW0FaultHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PWM0Generator0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PWM0Generator1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PWM0Generator2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void QEI0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC0SS0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC0SS1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC0SS2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC0SS3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WDT0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER0AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER0BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER1AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER1BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER2AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER2BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void COMP0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void COMP1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void COMP2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void SYSCTLHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void FLASHHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOFHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOGHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOHHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void SSI1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER3AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER3BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void I2C1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void QEI1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void CAN0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void CAN1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void CAN2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void HibernateHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void USB0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PWM0Generator3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UDMAHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UDMAErrHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC1SS0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC1SS1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC1SS2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void ADC1SS3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOJHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOKHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOLHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void SSI2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void SSI3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART4Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART5Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART6Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void UART7Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void I2C2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void I2C3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER4AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER4BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER5AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void TIMER5BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER0AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER0BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER1AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER1BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER2AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER2BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER3AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER3BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER4AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER4BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER5AHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void WTIMER5BHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void FPUHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void I2C4Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void I2C5Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOMHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIONHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void QEI2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP4Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP5Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP6Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOP7Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ4Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ5Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ6Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOQ7Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIORHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void GPIOSHandler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PMW1Generator0Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PMW1Generator1Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PMW1Generator2Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PMW1Generator3Handler       (void) __attribute__ ((weak, alias("IntDefaultHandler")));
void PWM1FaultHandle       (void) __attribute__ ((weak, alias("IntDefaultHandler")));

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
static unsigned long pulStack[64];

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long)pulStack + sizeof(pulStack)),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
    FaultISR,                               // The hard fault handler
    HardFaultHandler,                      // The MPU fault handler
    BusFaultHandler,                      // The bus fault handler
    UsageFaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVCallHandler,                      // SVCall handler
    DebugMonitorHandler,                      // Debug monitor handler
    0,                                      // Reserved
    PendSVHandler,                      // The PendSV handler
    SysTickHandler,                      // The SysTick handler
    GPIOAHandler,                      // GPIO Port A
    GPIOBHandler,                      // GPIO Port B
    GPIOCHandler,                      // GPIO Port C
    GPIODHandler,                      // GPIO Port D
    GPIOEHandler,                      // GPIO Port E
    UART0Handler,                      // UART0 Rx and Tx
    UART1Handler,                      // UART1 Rx and Tx
    SSI0Handler,                      // SSI0 Rx and Tx
    I2C0Handler,                      // I2C0 Master and Slave
    PMW0FaultHandler,                      // PWM Fault
    PWM0Generator0Handler,                      // PWM Generator 0
    PWM0Generator1Handler,                      // PWM Generator 1
    PWM0Generator2Handler,                      // PWM Generator 2
    QEI0Handler,                      // Quadrature Encoder 0
    ADC0SS0Handler,                      // ADC Sequence 0
    ADC0SS1Handler,                      // ADC Sequence 1
    ADC0SS2Handler,                      // ADC Sequence 2
    ADC0SS3Handler,                      // ADC Sequence 3
    WDT0Handler,                      // Watchdog timer
    TIMER0AHandler,                      // Timer 0 subtimer A
    TIMER0BHandler,                      // Timer 0 subtimer B
    TIMER1AHandler,                      // Timer 1 subtimer A
    TIMER1BHandler,                      // Timer 1 subtimer B
    TIMER2AHandler,                      // Timer 2 subtimer A
    TIMER2BHandler,                      // Timer 2 subtimer B
    COMP0Handler,                      // Analog Comparator 0
    COMP1Handler,                      // Analog Comparator 1
    COMP2Handler,                      // Analog Comparator 2
    SYSCTLHandler,                      // System Control (PLL, OSC, BO)
    FLASHHandler,                      // FLASH Control
    GPIOFHandler,                      // GPIO Port F
    GPIOGHandler,                      // GPIO Port G
    GPIOHHandler,                      // GPIO Port H
    UART2Handler,                      // UART2 Rx and Tx
    SSI1Handler,                      // SSI1 Rx and Tx
    TIMER3AHandler,                      // Timer 3 subtimer A
    TIMER3BHandler,                      // Timer 3 subtimer B
    I2C1Handler,                      // I2C1 Master and Slave
    QEI1Handler,                      // Quadrature Encoder 1
    CAN0Handler,                      // CAN0
    CAN1Handler,                      // CAN1
    CAN2Handler,                      // CAN2
    IntDefaultHandler,                      // Ethernet
    HibernateHandler,                      // Hibernate
    USB0Handler,                      // USB0
    PWM0Generator3Handler,                      // PWM Generator 3
    UDMAHandler,                      // uDMA Software Transfer
    UDMAErrHandler,                      // uDMA Error
    ADC1SS0Handler,                      // ADC1 Sequence 0
    ADC1SS1Handler,                      // ADC1 Sequence 1
    ADC1SS2Handler,                      // ADC1 Sequence 2
    ADC1SS3Handler,                      // ADC1 Sequence 3
    IntDefaultHandler,                      // I2S0
    IntDefaultHandler,                      // External Bus Interface 0
    GPIOJHandler,                      // GPIO Port J
    GPIOKHandler,                      // GPIO Port K
    GPIOLHandler,                      // GPIO Port L
    SSI2Handler,                      // SSI2 Rx and Tx
    SSI3Handler,                      // SSI3 Rx and Tx
    UART3Handler,                      // UART3 Rx and Tx
    UART4Handler,                      // UART4 Rx and Tx
    UART5Handler,                      // UART5 Rx and Tx
    UART6Handler,                      // UART6 Rx and Tx
    UART7Handler,                      // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    I2C2Handler,                      // I2C2 Master and Slave
    I2C3Handler,                      // I2C3 Master and Slave
    TIMER4AHandler,                      // Timer 4 subtimer A
    TIMER4BHandler,                      // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    TIMER5AHandler,                      // Timer 5 subtimer A
    TIMER5BHandler,                      // Timer 5 subtimer B
    WTIMER0AHandler,                      // Wide Timer 0 subtimer A
    WTIMER0BHandler,                      // Wide Timer 0 subtimer B
    WTIMER1AHandler,                      // Wide Timer 1 subtimer A
    WTIMER1BHandler,                      // Wide Timer 1 subtimer B
    WTIMER2AHandler,                      // Wide Timer 2 subtimer A
    WTIMER2BHandler,                      // Wide Timer 2 subtimer B
    WTIMER3AHandler,                      // Wide Timer 3 subtimer A
    WTIMER3BHandler,                      // Wide Timer 3 subtimer B
    WTIMER4AHandler,                      // Wide Timer 4 subtimer A
    WTIMER4BHandler,                      // Wide Timer 4 subtimer B
    WTIMER5AHandler,                      // Wide Timer 5 subtimer A
    WTIMER5BHandler,                      // Wide Timer 5 subtimer B
    FPUHandler,                      // FPU
    IntDefaultHandler,                      // PECI 0
    IntDefaultHandler,                      // LPC 0
    I2C4Handler,                      // I2C4 Master and Slave
    I2C5Handler,                      // I2C5 Master and Slave
    GPIOMHandler,                      // GPIO Port M
    GPIONHandler,                      // GPIO Port N
    QEI2Handler,                      // Quadrature Encoder 2
    IntDefaultHandler,                      // Fan 0
    0,                                      // Reserved
    GPIOP0Handler,                      // GPIO Port P (Summary or P0)
    GPIOP1Handler,                      // GPIO Port P1
    GPIOP2Handler,                      // GPIO Port P2
    GPIOP3Handler,                      // GPIO Port P3
    GPIOP4Handler,                      // GPIO Port P4
    GPIOP5Handler,                      // GPIO Port P5
    GPIOP6Handler,                      // GPIO Port P6
    GPIOP7Handler,                      // GPIO Port P7
    GPIOQ0Handler,                      // GPIO Port Q (Summary or Q0)
    GPIOQ1Handler,                      // GPIO Port Q1
    GPIOQ2Handler,                      // GPIO Port Q2
    GPIOQ3Handler,                      // GPIO Port Q3
    GPIOQ4Handler,                      // GPIO Port Q4
    GPIOQ5Handler,                      // GPIO Port Q5
    GPIOQ6Handler,                      // GPIO Port Q6
    GPIOQ7Handler,                      // GPIO Port Q7
    GPIORHandler,                      // GPIO Port R
    GPIOSHandler,                      // GPIO Port S
    PMW1Generator0Handler,                      // PWM 1 Generator 0
    PMW1Generator1Handler,                      // PWM 1 Generator 1
    PMW1Generator2Handler,                      // PWM 1 Generator 2
    PMW1Generator3Handler,                      // PWM 1 Generator 3
    PWM1FaultHandle                       // PWM 1 Fault
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

    //
    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).  Any configuration of the floating-point unit using DriverLib
    // APIs must be done here prior to the floating-point unit being enabled.
    //
    // Note that this does not use DriverLib since it might not be included in
    // this project.
    //
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) &
                         ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) |
                        NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

    //
    // Call the application's entry point.
    //
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}
