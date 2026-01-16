
#include <stdio.h>
#include "uart.h"

/* support printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
__asm(".global __use_no_semihosting\n\t");
void _sys_exit(int x)
{
  x = x;
}
/* __use_no_semihosting was requested, but _ttywrch was */
void _ttywrch(int ch)
{
  ch = ch;
}
FILE __stdout;
#else
#  ifdef __CC_ARM
#    pragma import(__use_no_semihosting)
struct __FILE
{
  int handle;
};
FILE __stdout;
void _sys_exit(int x)
{
  x = x;
}
/* __use_no_semihosting was requested, but _ttywrch was */
void _ttywrch(int ch)
{
  ch = ch;
}
#  endif
#endif

#if defined(__GNUC__) && !defined(__clang__)
#  define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#  define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/**
 * @brief  retargets the c library printf function to the usart.
 * @param  none
 * @retval none
 */
int fputc(int ch, FILE *f)
{
  while (usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(PRINT_UART, (uint16_t)ch);
  while (usart_flag_get(PRINT_UART, USART_TDC_FLAG) == RESET);
  return ch;
}

#if (defined(__GNUC__) && !defined(__clang__)) || (defined(__ICCARM__))
#  if defined(__GNUC__) && !defined(__clang__)
int _write(int fd, char *pbuffer, int size)
#  elif defined(__ICCARM__)
#    pragma module_name = "?__write"
int __write(int fd, char *pbuffer, int size)
#  endif
{
  for (int i = 0; i < size; i++)
  {
    while (usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(PRINT_UART, (uint16_t)(*pbuffer++));
    while (usart_flag_get(PRINT_UART, USART_TDC_FLAG) == RESET);
  }

  return size;
}
#endif

/**
 * @brief  initialize uart
 * @param  baudrate: uart baudrate
 * @retval none
 */
void Uart_Print_Init(uint32_t baudrate)
{
  gpio_init_type gpio_init_struct;

#if defined(__GNUC__) && !defined(__clang__)
  setvbuf(stdout, NULL, _IONBF, 0);
#endif

  /* enable the uart and gpio clock */
  crm_periph_clock_enable(PRINT_UART_CRM_CLK, TRUE);
  crm_periph_clock_enable(PRINT_UART_TX_GPIO_CRM_CLK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the uart tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode           = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins           = PRINT_UART_TX_PIN;
  gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;
  gpio_init(PRINT_UART_TX_GPIO, &gpio_init_struct);

  /* configure uart param */
  usart_init(PRINT_UART, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(PRINT_UART, TRUE);
  usart_enable(PRINT_UART, TRUE);
}
