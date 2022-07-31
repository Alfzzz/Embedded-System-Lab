#include "board.h"
#include "gpio.h"
#include "power_clocks_lib.h"
#include "et024006dhu.h"
#include "delay.h"
#include "avr32_logo.h"
#include "conf_clock.h"


//Definición de colores
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define ET024006_WIDTH 320 //WIDHT MAXIMO 
#define ET024006_HEIGHT 240 //HEIGHT MAXIMO (320X240)


//INICIALIZACIÓN PWM
#if BOARD == EVK1105
#include "pwm.h"
avr32_pwm_channel_t pwm_channel6 = {
/*
  .cmr = ((PWM_MODE_LEFT_ALIGNED << AVR32_PWM_CMR_CALG_OFFSET)
    | (PWM_POLARITY_HIGH << AVR32_PWM_CMR_CPOL_OFFSET)
    | (PWM_UPDATE_DUTY << AVR32_PWM_CMR_CPD_OFFSET)
    | AVR32_PWM_CMR_CPRE_MCK_DIV_2),
    */
  //.cdty = 0,
  .cdty = 0,
  .cprd = 100
};
//TERMINA INICIALIZACIÓN PWM

//INICIALIZACIÓN TFT DISPLAY
static void tft_bl_init(void)
{
  pwm_opt_t opt = {
    .diva = 0,
    .divb = 0,
    .prea = 0,
    .preb = 0
  };
  /* MCK = OSC0 = 12MHz
   * Desired output 60kHz
   * Chosen MCK_DIV_2
   * CPRD = 12MHz / (60kHz * 2) = 100
   *
   * The duty cycle is 100% (CPRD = CDTY)
   * */
  pwm_init(&opt);
  pwm_channel6.CMR.calg = PWM_MODE_LEFT_ALIGNED;
  pwm_channel6.CMR.cpol = PWM_POLARITY_HIGH; //PWM_POLARITY_LOW;//PWM_POLARITY_HIGH;
  pwm_channel6.CMR.cpd = PWM_UPDATE_DUTY;
  pwm_channel6.CMR.cpre = AVR32_PWM_CMR_CPRE_MCK_DIV_2;

  pwm_channel_init(6, &pwm_channel6);
  pwm_start_channels(AVR32_PWM_ENA_CHID6_MASK);

}
#endif
//TERMINA INICIALIZACIÓN TFT DISPLAY


// Main function 
//AQUÍ VA EL CÓDIGO A DESPLEGAR
int main(void)
{
  U32 i;

  // Set CPU and PBA clock
  pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

  gpio_enable_gpio_pin(LED0_GPIO);
  gpio_enable_gpio_pin(LED1_GPIO);
  gpio_enable_gpio_pin(LED2_GPIO);
  gpio_enable_gpio_pin(LED3_GPIO);

  et024006_Init( FOSC0, FOSC0 );

#if BOARD == EVK1105
  /* PWM is fed by PBA bus clock which is by default the same
   * as the CPU speed. We set a 0 duty cycle and thus keep the
   * display black*/
  tft_bl_init();
#endif

  // Clear the display i.e. make it black - PINTAMOS FONDO
  et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
  while(pwm_channel6.cdty < pwm_channel6.cprd)
  {
	  pwm_channel6.cdty++;
	  pwm_channel6.cupd = pwm_channel6.cdty;
	  //pwm_channel6.cdty--;
	  pwm_async_update_channel(AVR32_PWM_ENA_CHID6, &pwm_channel6);
	  delay_ms(10);
  }

//RELLENAMOS CON CUADROS BLANCOS
//FIRST ROW
et024006_DrawFilledRect(0 , 0, 40, 30, WHITE);
et024006_DrawFilledRect(80 , 0, 40, 30, WHITE);
et024006_DrawFilledRect(160 , 0, 40, 30, WHITE);
et024006_DrawFilledRect(240 , 0, 40, 30, WHITE);
//SECOND ROW
et024006_DrawFilledRect(40 , 30, 40, 30, WHITE);
et024006_DrawFilledRect(120 , 30, 40, 30, WHITE);
et024006_DrawFilledRect(200 , 30, 40, 30, WHITE);
et024006_DrawFilledRect(280 , 30, 40, 30, WHITE);
//THIRD ROW
et024006_DrawFilledRect(0 , 60, 40, 30, WHITE);
et024006_DrawFilledRect(80 , 60, 40, 30, WHITE);
et024006_DrawFilledRect(160, 60, 40, 30, WHITE);
et024006_DrawFilledRect(240 , 60, 40, 30, WHITE);
//fOURTH ROW
et024006_DrawFilledRect(40 , 90, 40, 30, WHITE);
et024006_DrawFilledRect(120 , 90, 40, 30, WHITE);
et024006_DrawFilledRect(200 , 90, 40, 30, WHITE);
et024006_DrawFilledRect(280 , 90, 40, 30, WHITE);
//FIFTH ROW 
et024006_DrawFilledRect(0 , 120, 40, 30, WHITE);
et024006_DrawFilledRect(80 , 120, 40, 30, WHITE);
et024006_DrawFilledRect(160, 120, 40, 30, WHITE);
et024006_DrawFilledRect(240 , 120, 40, 30, WHITE);	
//SIXTH ROW 
et024006_DrawFilledRect(40 , 150, 40, 30, WHITE);
et024006_DrawFilledRect(120 , 150, 40, 30, WHITE);
et024006_DrawFilledRect(200 , 150, 40, 30, WHITE);
et024006_DrawFilledRect(280 , 150, 40, 30, WHITE);
//SEVENTH ROW
et024006_DrawFilledRect(0 , 180, 40, 30, WHITE);
et024006_DrawFilledRect(80 , 180, 40, 30, WHITE);
et024006_DrawFilledRect(160, 180, 40, 30, WHITE);
et024006_DrawFilledRect(240 , 180, 40, 30, WHITE);
//EIGTH ROW 
et024006_DrawFilledRect(40 , 210, 40, 30, WHITE);
et024006_DrawFilledRect(120 , 210, 40, 30, WHITE);
et024006_DrawFilledRect(200 , 210, 40, 30, WHITE);
et024006_DrawFilledRect(280 , 210, 40, 30, WHITE);

  while(true);
}