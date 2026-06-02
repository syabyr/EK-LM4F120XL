/**
 * @file startup_gcc.c
 * @brief Cortex-M4F startup code for LM4F120H5QR with LVGL
 *
 * Vector table, ResetISR, and SysTick handler for LVGL tick.
 */

#include <stdint.h>

/* Forward declarations */
extern void main(void);
extern void DelayMs(unsigned long ulCount);
extern void SysTickIntHandler(void);
void ResetISR(void);
void NmiSR(void);
void FaultISR(void);
void IntDefaultHandler(void);

/* Stack - 256 words = 1KB */
static uint32_t pulStack[256];

/* Vector table */
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))pulStack,    // Initial stack pointer
    ResetISR,                     // Reset handler
    NmiSR,                        // NMI handler
    FaultISR,                     // Hard fault handler
    0,                            // MemManage handler
    0,                            // BusFault handler
    0,                            // UsageFault handler
    0,                            // Reserved
    0,                            // Reserved
    0,                            // Reserved
    0,                            // Reserved
    0,                            // SVCall handler
    0,                            // Debug monitor handler
    0,                            // Reserved
    0,                            // PendSV handler
    SysTickIntHandler,            // SysTick handler (for lv_tick_inc)
    0,                            // GPIO Port A
    0,                            // GPIO Port B
    0,                            // GPIO Port C
    0,                            // GPIO Port D (touch interrupt - not used in LVGL mode)
    0,                            // GPIO Port E
    0,                            // GPIO Port F
    0,                            // UART0 Rx/Tx (not using interrupt in LVGL mode)
    0,                            // UART1 Rx/Tx
    0,                            // SSI0 Rx/Tx
    0,                            // I2C0 Master/Slave
};

/* External declarations for .data init */
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void ResetISR(void)
{
    uint32_t *pSrc, *pDest;

    /* Copy .data from Flash to SRAM */
    pSrc = &_sidata;
    for (pDest = &_sdata; pDest < &_edata;)
    {
        *pDest++ = *pSrc++;
    }

    /* Zero fill .bss */
    for (pDest = &_sbss; pDest < &_ebss;)
    {
        *pDest++ = 0;
    }

    /* Enable FPU: set CP10 and CP11 full access */
    __asm volatile(
        "ldr.w r0, =0xE000ED88   \n"
        "ldr r1, [r0]            \n"
        "orr r1, r1, #(0xF << 20)\n"
        "str r1, [r0]            \n"
        ::: "r0", "r1", "memory"
    );

    /* Call main */
    main();

    /* If main returns, loop forever */
    while (1)
    {
    }
}

void NmiSR(void)
{
    while (1)
    {
    }
}

void FaultISR(void)
{
    while (1)
    {
    }
}

void IntDefaultHandler(void)
{
    while (1)
    {
    }
}
