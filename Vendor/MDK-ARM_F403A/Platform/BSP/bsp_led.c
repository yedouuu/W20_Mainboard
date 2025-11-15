
#include "bsp_led.h"


gpio_type *led_gpio_port[LED_NUM]        = {LED2_GPIO, LED3_GPIO, LED4_GPIO, LED5_GPIO};
uint16_t led_gpio_pin[LED_NUM]           = {LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN};
crm_periph_clock_type led_gpio_crm_clk[LED_NUM] = {LED2_GPIO_CRM_CLK, LED3_GPIO_CRM_CLK, LED4_GPIO_CRM_CLK, LED5_GPIO_CRM_CLK};

void BSP_LED_Init(void)
{
  gpio_init_type gpio_init_struct;

  for (uint8_t i = 0; i < LED_NUM; i++)
  {
    crm_periph_clock_enable(led_gpio_crm_clk[i], TRUE);
    
    /* set default parameter */
    gpio_default_para_init(&gpio_init_struct);

    /* configure the led gpio */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = led_gpio_pin[i];
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

    gpio_init(led_gpio_port[i], &gpio_init_struct);
    BSP_LED_Off(i);
  }
}

void BSP_LED_On(uint8_t led)
{
  if (led < LED_NUM)
  {
    gpio_bits_reset(led_gpio_port[led], led_gpio_pin[led]);
  }
}

void BSP_LED_Off(uint8_t led)
{
  if (led < LED_NUM)
  {
    gpio_bits_set(led_gpio_port[led], led_gpio_pin[led]);
  }
}

void BSP_LED_Toggle(uint8_t led)
{
  if (led < LED_NUM)
  {
    uint8_t status = gpio_input_data_bit_read(led_gpio_port[led], led_gpio_pin[led]);
    gpio_bits_write(led_gpio_port[led], led_gpio_pin[led], (status == RESET) ? SET : RESET);
  }
}
