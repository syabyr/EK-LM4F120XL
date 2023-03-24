#include "TM4C123GH6PM.h"
#include "board.h"


//--------------------------------------------------------------------+
// Forward USB interrupt events to TinyUSB IRQ Handler
//--------------------------------------------------------------------+
void USB0_Handler(void)
{
#if CFG_TUH_ENABLED
  tuh_int_handler(0);
#endif

#if CFG_TUD_ENABLED
  tud_int_handler(0);
#endif
}


extern uint32_t SystemCoreClock;
void board_init (void)
{
  //SystemCoreClockUpdate();

#if CFG_TUSB_OS == OPT_OS_NONE
  // 1ms tick timer
  SysTick_Config(SystemCoreClock / 1000);
#elif CFG_TUSB_OS == OPT_OS_FREERTOS
    // If freeRTOS is used, IRQ priority is limit by max syscall ( smaller is higher )
    NVIC_SetPriority(USB0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
#endif


}



#if CFG_TUSB_OS == OPT_OS_NONE
volatile uint32_t system_ticks = 0;
void SysTick_Handler (void)
{
  system_ticks++;
}

uint32_t board_millis (void)
{
  return system_ticks;
}
#endif

