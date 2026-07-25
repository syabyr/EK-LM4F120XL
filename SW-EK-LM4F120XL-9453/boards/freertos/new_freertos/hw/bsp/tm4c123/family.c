#include "TM4C123GH6PM.h"
#include "bsp/board.h"
#include "board.h"



static void initialize_board_led (GPIOA_Type *port, uint8_t PinMsk, uint8_t dirmsk)
{
  /* Enable PortF Clock */
  SYSCTL->RCGCGPIO |= (1 << 5);

  /* Let the clock stabilize */
  while ( !((SYSCTL->PRGPIO) & (1 << 5)) ) {}

  /* Port Digital Enable */
  port->DEN |= PinMsk;

  /* Set direction */
  port->DIR = dirmsk;
}



static void WriteGPIOPin (GPIOA_Type *port, uint8_t PinMsk, bool state)
{
  if ( state )
  {
    port->DATA |= PinMsk;
  }
  else
  {
    port->DATA &= ~(PinMsk);
  }
}

static uint32_t ReadGPIOPin (GPIOA_Type *port, uint8_t pinMsk)
{
  return (port->DATA & pinMsk);
}

void board_init (void)
{
  //SystemCoreClockUpdate();


  // 1ms tick timer
  //SysTick_Config(SystemCoreClock / 1000);

    // If freeRTOS is used, IRQ priority is limit by max syscall ( smaller is higher )
    //NVIC_SetPriority(USB0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );


  /* Reset USB */
  //SYSCTL->SRCR2 |= (1u << 16);

  for ( volatile uint8_t i = 0; i < 20; i++ ) {}

  //SYSCTL->SRCR2 &= ~(1u << 16);

  /* Open the USB clock gate */
  SYSCTL->RCGCUSB |= (1 << 0);

  /* Power-up USB PLL */
  SYSCTL->RCC2 &= ~(1u << 14);

  /* USB IO Initialization */
  SYSCTL->RCGCGPIO |= (1u << 3);

  /* Let the clock stabilize */
  while ( !(SYSCTL->PRGPIO & (1u << 3)) ) {}

  /* USB IOs to Analog Mode */
  GPIOD->AFSEL &= ~((1u << 4) | (1u << 5));
  GPIOD->DEN &= ~((1u << 4) | (1u << 5));
  GPIOD->AMSEL |= ((1u << 4) | (1u << 5));

  uint8_t leds = (1 << LED_PIN_RED) | (1 << LED_PIN_BLUE) | (1 << LED_PIN_GREEN);
  uint8_t dirmsk = (1 << LED_PIN_RED) | (1 << LED_PIN_BLUE) | (1 << LED_PIN_GREEN);

  /* Configure GPIO for board LED */
  initialize_board_led(LED_PORT, leds, dirmsk);
}

void board_led_write (bool state)
{
  WriteGPIOPin(LED_PORT, (1 << LED_PIN_BLUE), state);
}

void board_led2_write (bool state)
{
  WriteGPIOPin(LED_PORT, (1 << LED_PIN_GREEN), state);
}

uint32_t board_button_read (void)
{
  uint32_t gpio_value = ReadGPIOPin(BOARD_BTN_PORT, BOARD_BTN_Msk);
  return BUTTON_STATE_ACTIVE ? gpio_value : !gpio_value;
}

int board_uart_write (void const *buf, int len)
{
  uint8_t const * data = buf;

  for ( int i = 0; i < len; i++ )
  {
    while ( (UART0->FR & (1 << 5)) != 0 ) {} // Poll until previous data was shofted out
    UART0->DR = data[i];                     // Write UART0 DATA REGISTER
  }

  return len;
}

int board_uart_read (uint8_t *buf, int len)
{
  (void) buf;
  (void) len;
  return 0;
}
