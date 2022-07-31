//TC4=12MHz/128=93750Hz
//T= 500ms

//RC=Cte de tiempo= T/Tr=500ms/(1/93750Hz)=46875 


#include <avr32/io.h>
#include "intc.h"
#include "compiler.h"
#include "board.h"
#include "pm.h"
#include "gpio.h"
#include "tc.h"
#include "et024006dhu.h"
#include "power_clocks_lib.h"
#include "pwm.h"
#include "delay.h"
#include "eic.h"
#include <stdio.h>
#include "whatsapp_image.h"
#include "brujula.h"
#include "telefono1.h"
#include "telefono2.h"


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


int time_1s=0;
int funcion=0;//funcion=0 Reloj principal,funcion=1 Ajuste de reloj,funcion=2 Ajuste de alarma
int reloj_segundos=0;
int reloj_minutos=0;
int reloj_horas=0;
int reloj_segundos_ajuste=0;
int reloj_minutos_ajuste=0;
int reloj_horas_ajuste=0;
int reloj_ajuste=0;//0 Ajustar horas,1 Ajustar minutos,2 Ajustar segundos

int reloj_segundos_alarma=0;
int reloj_minutos_alarma=0;
int reloj_horas_alarma=0;
int reloj_alarma=0;//0 Ajustar horas,1 Ajustar minutos,2 Ajustar segundos
int alarma_activa_controlOFF=0b00;//Primer bit para indicar activación de alarma, segundo bit para controlar la activación de alarma

int reloj_segundos_cronometro=0;
int reloj_minutos_cronometro=0;
int reloj_horas_cronometro=0;
int estado_reloj_cronometro=1;//0 Start,1 Stop,2 Reset

const int HorasX_decenas[2] = {7,93};
const int HorasY_decenas[2] = {120,120};
const int HorasX_unidades[9] = {50,68,78,74,60,40,26,22,22};
const int HorasY_unidades[9] = {92,99,115,134,146,146,134,115,99};
const int MinutosX_decenas[5] = {160,90+110,75+110,25+110,9+110};
const int MinutosY_decenas[5] = {77,107,155,155,107};
const int MinutosX_unidades[9] = {50+110,68+110,78+110,74+110,60+110,40+110,26+110,22+110,32+110};
const int MinutosY_unidades[9] = {92,99,115,134,146,146,134,115,99};
	
static void tc_irq(void);
static void tft_bl_init(void);
void showClock(int segundos,int minutos,int horas,int funcion);
void teclas_isr(void);
int main(void){
	volatile avr32_tc_t *tc = &AVR32_TC;
	pm_switch_to_osc0(&AVR32_PM, 12000000,4);
	pm_cksel(&AVR32_PM,0,0,0,0,0,0);
	
	// Options for waveform genration.
	static const tc_waveform_opt_t WAVEFORM_OPT ={
		.channel  = 0,                        // Channel selection.

		.bswtrg   = AVR32_TC_NONE,                // Software trigger effect on TIOB.
		.beevt    = AVR32_TC_NONE,                // External event effect on TIOB.
		.bcpc     = AVR32_TC_NONE,                // RC compare effect on TIOB.
		.bcpb     = AVR32_TC_NONE,                // RB compare effect on TIOB.

		.aswtrg   = AVR32_TC_NONE,                // Software trigger effect on TIOA.
		.aeevt    = AVR32_TC_NONE,                // External event effect on TIOA.
		.acpc     = AVR32_TC_NONE,                // RC compare effect on TIOA: toggle.
		.acpa     = AVR32_TC_NONE,                  // RA compare effect on TIOA: toggle (other possibilities are none, set and clear).

		.wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,// 2 Waveform selection: Up mode with automatic trigger(reset) on RC compare.
		.enetrg   = false,                                          // External event trigger enable.
		.eevt     = 0,                                                    // External event selection.
		.eevtedg  = TC_SEL_NO_EDGE,                   // External event edge selection.
		.cpcdis   = false,                                         // Counter disable when RC compare.
		.cpcstop  = false,                                        // Counter clock stopped with RC compare.

		.burst    = false,                                           // Burst signal selection.
		.clki     = false,                                            // Clock inversion.
		.tcclks   = TC_CLOCK_SOURCE_TC5         // Internal source clock 3, connected to fPBA / 128.
	};
	static const tc_interrupt_t TC_INTERRUPT ={
		.etrgs = 0,
		.ldrbs = 0,
		.ldras = 0,
		.cpcs  = 1,   // Habilitar interrupción por comparación con RC
		.cpbs  = 0,
		.cpas  = 0,
		.lovrs = 0,
		.covfs = 0
	};
	Disable_global_interrupt();
	
	// Initialize interrupt vectors.
	INTC_init_interrupts();

	// Register the RTC interrupt handler to the interrupt controller.
	INTC_register_interrupt(&tc_irq,448, 3);//Canal 0 IRQ=448,nivel 0 de prioridad
	INTC_register_interrupt(&teclas_isr, 33, 3); 
	
	eic_options_t eic_options;
	eic_options.eic_mode = EIC_MODE_EDGE_TRIGGERED; //Modo de flanco
	eic_options.eic_edge = EIC_EDGE_RISING_EDGE;
	eic_options.eic_async = EIC_SYNCH_MODE; //Modo síncrono
	eic_options.eic_line = 1; //EIC 1 en B NUMERO DE EIC

	//Inicializar EIC
	eic_init(&AVR32_EIC,&eic_options,1);//EIC 1
	eic_enable_line(&AVR32_EIC,1);
	eic_enable_interrupt_line(&AVR32_EIC,1);

	gpio_enable_module_pin(22,1);//GPIO 22 con funcion B (1) en este es para las 5 teclas
	
	gpio_enable_pin_interrupt(QT1081_TOUCH_SENSOR_DOWN ,1);
	gpio_enable_pin_interrupt(QT1081_TOUCH_SENSOR_UP ,1);
	gpio_enable_pin_interrupt(QT1081_TOUCH_SENSOR_LEFT ,1);
	gpio_enable_pin_interrupt(QT1081_TOUCH_SENSOR_RIGHT ,1);
	gpio_enable_pin_interrupt(QT1081_TOUCH_SENSOR_ENTER ,1);
	
	
	//Habilita interrupciones globales
	
	Enable_global_interrupt();


	et024006_Init( FOSC0, FOSC0 );
	tft_bl_init();
  for( int i=0 ; i<16 ; i++ )
  {
	  // From black to white.
	  //et024006_DrawFilledRect(20*i,   0, 20, 20, (2*i)/*B:5*/ | ((4*i)<<5)/*G:6*/ | ((2*i)<<11)/*R:5*/ );
	  // From black to blue.
	  //et024006_DrawFilledRect(20*i,  20, 20, 20, (2*i) /*B:5*/);
	  // From black to green
	  //et024006_DrawFilledRect(20*i, 200, 20, 20, ((4*i)<<5) /*G:6*/);
	  // From black to red
	  //et024006_DrawFilledRect(20*i, 220, 20, 20, ((2*i)<<11) /*R:5*/);
  }
  
	  
	  /* Lets do a nice fade in by increasing the duty cycle */
	  while(pwm_channel6.cdty < pwm_channel6.cprd)
	  {
		  pwm_channel6.cdty++;
		  pwm_channel6.cupd = pwm_channel6.cdty;
		  //pwm_channel6.cdty--;
		  pwm_async_update_channel(AVR32_PWM_ENA_CHID6, &pwm_channel6);
		  delay_ms(10);
	  }

	// Initialize the timer/counter.
	tc_init_waveform(tc, &WAVEFORM_OPT);         // Initialize the timer/counter waveform.

	// Set the compare triggers.
	// Remember TC counter is 16-bits, so counting second is not possible with fPBA = 12 MHz.
	// We configure it to count ms.
	// We want: (1/(fPBA/8)) * RC = 0.001 s, hence RC = (fPBA/8) / 1000 = 1500 to get an interrupt every 1 ms.
	tc_write_rc(tc, 0,46875);            //Channel 0, Set RC value.

	tc_configure_interrupts(tc, 0, &TC_INTERRUPT);//Channel 0

	// Start the timer/counter.
	tc_start(tc, 0);                    // And start the timer/counter with channel 0.
	while(1){
	}
	
}
__attribute__ ((__interrupt__))
void teclas_isr(void){
	if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP)==1){
		if (funcion==1){
			if(reloj_ajuste==2)
				reloj_segundos_ajuste++;
			else if(reloj_ajuste==1)
				reloj_minutos_ajuste++;
			else if(reloj_ajuste==0)
				reloj_horas_ajuste++;
					
			if(reloj_segundos_ajuste==60){
				reloj_segundos_ajuste=0;
			}
			if(reloj_minutos_ajuste==60){
				reloj_minutos_ajuste=0;
			}
			if(reloj_horas_ajuste==24){
				reloj_horas_ajuste=0;
			}
			showClock(reloj_segundos_ajuste,reloj_minutos_ajuste,reloj_horas_ajuste,1);
		}
		if (funcion==2){
			if(reloj_alarma==2)
			reloj_segundos_alarma++;
			else if(reloj_alarma==1)
			reloj_minutos_alarma++;
			else if(reloj_alarma==0)
			reloj_horas_alarma++;
					
			if(reloj_segundos_alarma==60){
				reloj_segundos_alarma=0;
			}
			if(reloj_minutos_alarma==60){
				reloj_minutos_alarma=0;
			}
			if(reloj_horas_alarma==24){
				reloj_horas_alarma=0;
			}
			showClock(reloj_segundos_alarma,reloj_minutos_alarma,reloj_horas_alarma,2);
		}
		if (funcion==3){
			estado_reloj_cronometro=0;
		}
		if(alarma_activa_controlOFF==0b11){
			alarma_activa_controlOFF=0b00;
			gpio_set_gpio_pin(LED1_GPIO);
		}
	}
	if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN)==1){
		if (funcion==1){
			if(reloj_ajuste==0 && reloj_horas_ajuste>0)
				reloj_horas_ajuste--;
			else if (reloj_ajuste==0 && reloj_horas_ajuste==0)
				reloj_horas_ajuste=23;
			if(reloj_ajuste==1 && reloj_minutos_ajuste>0)
				reloj_minutos_ajuste--;
			else if (reloj_ajuste==1 && reloj_minutos_ajuste==0)
				reloj_minutos_ajuste=59;
			if(reloj_ajuste==2 && reloj_segundos_ajuste>0)
				reloj_segundos_ajuste--;
			else if (reloj_ajuste==2 && reloj_segundos_ajuste==0)
				reloj_segundos_ajuste=59;
			showClock(reloj_segundos_ajuste,reloj_minutos_ajuste,reloj_horas_ajuste,1);			
		}
		if (funcion==2){
			if(reloj_alarma==0 && reloj_horas_alarma>0)
			reloj_horas_alarma--;
			else if (reloj_alarma==0 && reloj_horas_alarma==0)
			reloj_horas_alarma=23;
			if(reloj_alarma==1 && reloj_minutos_alarma>0)
			reloj_minutos_alarma--;
			else if (reloj_alarma==1 && reloj_minutos_alarma==0)
			reloj_minutos_alarma=59;
			if(reloj_alarma==2 && reloj_segundos_alarma>0)
			reloj_segundos_alarma--;
			else if (reloj_alarma==2 && reloj_segundos_alarma==0)
			reloj_segundos_alarma=59;
			showClock(reloj_segundos_alarma,reloj_minutos_alarma,reloj_horas_alarma,2);
		}
		if (funcion==3){
			showClock(reloj_segundos_cronometro,reloj_minutos_cronometro,reloj_horas_cronometro,3);
			estado_reloj_cronometro=1;
		}
		if(alarma_activa_controlOFF==0b11){
			alarma_activa_controlOFF=0b00;
			gpio_set_gpio_pin(LED1_GPIO);
		}
	}
	if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_LEFT)==1){
		if(funcion==1){
			if(reloj_ajuste>0)
				reloj_ajuste--;
			else
				reloj_ajuste=2;
		}
		if(funcion==2){
			if(reloj_alarma>0)
				reloj_alarma--;
			else
				reloj_alarma=2;
		}
		if (funcion==3){
			estado_reloj_cronometro=2;
			reloj_segundos_cronometro=0;
			reloj_minutos_cronometro=0;
			reloj_horas_cronometro=0;
			showClock(reloj_segundos_cronometro,reloj_minutos_cronometro,reloj_horas_cronometro,3);
		}
		if(alarma_activa_controlOFF==0b11){
			alarma_activa_controlOFF=0b00;
			gpio_set_gpio_pin(LED1_GPIO);
		}
	}
	if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_RIGHT)==1){
		funcion++;
		if(funcion==1){
			showClock(reloj_segundos_ajuste,reloj_minutos_ajuste,reloj_horas_ajuste,1);
		}
		else if(funcion==2){
			showClock(reloj_segundos_alarma,reloj_minutos_alarma,reloj_horas_alarma,2);
		}
		else if(funcion==3){
			reloj_segundos_cronometro=0;
			reloj_minutos_cronometro=0;
			reloj_horas_cronometro=0;
			estado_reloj_cronometro=1;
			showClock(reloj_segundos_alarma,reloj_minutos_alarma,reloj_horas_alarma,3);
		}
		else if(funcion==4){
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			et024006_PutPixmap(whatsapp_image, 320, 0, 0, 0, 0, 320, 240);
		}
		else if(funcion==6){
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			et024006_PutPixmap(brujula, 320, 0, 0, 0, 0, 320, 240);
		}
		else if(funcion==7){
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			et024006_PutPixmap(telefono1, 320, 0, 0, 0, 0, 320, 240);
		}
		else if(funcion==8){
			funcion=0;
		}
		if(alarma_activa_controlOFF==0b11){
			alarma_activa_controlOFF=0b00;
			gpio_set_gpio_pin(LED1_GPIO);
		}
	}

	if ((gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)==1)){
		funcion++;
		if(funcion==1){
			showClock(reloj_segundos_ajuste,reloj_minutos_ajuste,reloj_horas_ajuste,1);
		}
		else if(funcion==2){ 
			//Ajuste de tiempo de la funcion 1 cuando pasa al  funcion 2
			reloj_segundos=reloj_segundos_ajuste;
			reloj_minutos=reloj_minutos_ajuste;
			reloj_horas=reloj_horas_ajuste;
			funcion=0;
		}
		else if(funcion==3){
			alarma_activa_controlOFF=alarma_activa_controlOFF|0b10;
			funcion=0;
		}
		else if(funcion==4){
			estado_reloj_cronometro=1;
			
			
			funcion=0;
		}
		if(alarma_activa_controlOFF==0b11){
		alarma_activa_controlOFF=0b00;
		gpio_set_gpio_pin(LED1_GPIO);
		}
	}
	
	
	/*if(alarma_activa_controlOFF==0b11){
		alarma_activa_controlOFF=0b00;
		gpio_set_gpio_pin(LED1_GPIO);
	}*/
	eic_clear_interrupt_line(&AVR32_EIC,1);
}
__attribute__((__interrupt__))
static void tc_irq(void){
	// Clear the interrupt flag. This is a side effect of reading the TC SR.
	tc_read_sr(&AVR32_TC, 0);
	
	
	// Increment the ms seconds counter
	if(time_1s==1){	
		// Toggle a GPIO pin (this pin is used as a regular GPIO pin).
		gpio_tgl_gpio_pin(LED0_GPIO);
		reloj_segundos++;
		if(reloj_segundos==60){
			reloj_minutos++;
			reloj_segundos=0;
		}
		if(reloj_minutos==60){
			reloj_horas++;
			reloj_segundos=0;
			reloj_minutos=0;
		}
		if(reloj_horas==24){
			reloj_segundos=0;
			reloj_minutos=0;
			reloj_horas=0;
		}
		
		if(funcion==3 &&  estado_reloj_cronometro==0){
			reloj_segundos_cronometro++;
			if(reloj_segundos_cronometro==60){
				reloj_minutos_cronometro++;
				reloj_segundos_cronometro=0;
			}
			if(reloj_minutos_cronometro==60){
				reloj_horas_cronometro++;
				reloj_segundos_cronometro=0;
				reloj_minutos_cronometro=0;
			}
			if(reloj_horas_cronometro==24){
				reloj_segundos_cronometro=0;
				reloj_minutos_cronometro=0;
				reloj_horas_cronometro=0;
			}
			showClock(reloj_segundos_cronometro,reloj_minutos_cronometro,reloj_horas_cronometro,3);		
		}
		
		
		
		
		if(reloj_minutos==reloj_minutos_alarma && reloj_horas==reloj_horas_alarma && (alarma_activa_controlOFF&0b10)==0b10){
			alarma_activa_controlOFF=alarma_activa_controlOFF|0b01;
		}
		if(alarma_activa_controlOFF==0b11){
			gpio_tgl_gpio_pin(LED1_GPIO);
		}
		
		/*reloj_horas=17;
		reloj_minutos=45;
		reloj_segundos=3;*/
		if(funcion==0)
			showClock(reloj_segundos,reloj_minutos,reloj_horas,0);
		time_1s=0;
			
	}
	else{
		time_1s=1;
	}
}
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
void showClock(int segundos,int minutos,int horas,int funcion){
	et024006_DrawFilledRect(0,0,320,240,BLACK);
	//Ciculo exterior decenas
	et024006_DrawFilledCircle(50,120,50, RED,0xFF );
	et024006_DrawFilledCircle(160,120,50, GREEN,0xFF );
	et024006_DrawFilledCircle(270,120,50, BLUE,0xFF );
	et024006_DrawFilledCircle(50,120,48, BLACK,0xFF );
	et024006_DrawFilledCircle(160,120,48, BLACK,0xFF );
	et024006_DrawFilledCircle(270,120,48, BLACK,0xFF );
	//Circulo centro unidades
	et024006_DrawFilledCircle(50,120,36, RED,0xFF );
	et024006_DrawFilledCircle(160,120,36, GREEN,0xFF );
	et024006_DrawFilledCircle(270,120,36, BLUE,0xFF );
	et024006_DrawFilledCircle(50,120,33, BLACK,0xFF );
	et024006_DrawFilledCircle(160,120,33, BLACK,0xFF );
	et024006_DrawFilledCircle(270,120,33, BLACK,0xFF );
	//Circulo interior valores de tiempo
	et024006_DrawFilledCircle(50,120,20, RED,0xFF );
	et024006_DrawFilledCircle(160,120,20, GREEN,0xFF );
	et024006_DrawFilledCircle(270,120,20, BLUE,0xFF );
	et024006_DrawFilledCircle(50,120,18, BLACK,0xFF );
	et024006_DrawFilledCircle(160,120,18, BLACK,0xFF );
	et024006_DrawFilledCircle(270,120,18, BLACK,0xFF );
	if(horas/10==0){
		et024006_DrawFilledCircle(HorasX_decenas[0],HorasY_decenas[0],3, BLACK,0xFF );
		et024006_DrawFilledCircle(HorasX_decenas[1],HorasY_decenas[1],3, BLACK,0xFF );
	}
	else if(horas/10==1){
		et024006_DrawFilledCircle(HorasX_decenas[0],HorasY_decenas[0],3, BLACK,0xFF );
		et024006_DrawFilledCircle(HorasX_decenas[1],HorasY_decenas[1],3, RED,0xFF );
	}
	else{
		et024006_DrawFilledCircle(HorasX_decenas[0],HorasY_decenas[0],3, RED,0xFF );
		et024006_DrawFilledCircle(HorasX_decenas[1],HorasY_decenas[1],3, RED,0xFF );
	}
	
	for(int j=0;j<(horas%10);j++){
		et024006_DrawFilledCircle(HorasX_unidades[j],HorasY_unidades[j],3, RED,0xFF );
	}
	
	for(int j=0;j<5;j++){
		if(j<minutos/10)
			et024006_DrawFilledCircle(MinutosX_decenas[j],MinutosY_decenas[j],3, GREEN,0xFF );
		else
			et024006_DrawFilledCircle(MinutosX_decenas[j],MinutosY_decenas[j],3, BLACK,0xFF );	
			
			
		if(j<segundos/10)
			et024006_DrawFilledCircle(MinutosX_decenas[j]+110,MinutosY_decenas[j],3, BLUE,0xFF );
		else
			et024006_DrawFilledCircle(MinutosX_decenas[j]+110,MinutosY_decenas[j],3, BLACK,0xFF );
	}
	for(int j=0;j<9;j++){
		if(j<minutos%10)
			et024006_DrawFilledCircle(MinutosX_unidades[j],MinutosY_unidades[j],3, GREEN,0xFF );
		else
			et024006_DrawFilledCircle(MinutosX_unidades[j],MinutosY_unidades[j],3, BLACK,0xFF );
			
		if(j<segundos%10)
			et024006_DrawFilledCircle(MinutosX_unidades[j]+110,MinutosY_unidades[j],3, BLUE,0xFF );
		else
			et024006_DrawFilledCircle(MinutosX_unidades[j]+110,MinutosY_unidades[j],3, BLACK,0xFF );
	}
	char str[2];
	sprintf(str,"%d",horas);
	et024006_PrintString(str,(const unsigned char *)&FONT8x16,45,115,RED,-1);
	sprintf(str,"%d",minutos);
	et024006_PrintString(str,(const unsigned char *)&FONT8x16,45+110,115,GREEN,-1);
	sprintf(str,"%d",segundos);
	et024006_PrintString(str,(const unsigned char *)&FONT8x16,45+220,115,BLUE,-1);
	
	if(funcion==0)
		et024006_PrintString("Reloj principal",(const unsigned char *)&FONT8x16,45+110,220,WHITE,-1);
	else if(funcion==1)
		et024006_PrintString("Ajuste de hora",(const unsigned char *)&FONT8x16,45+110,220,WHITE,-1);
	else if(funcion==2)
		et024006_PrintString("Ajuste de alarma",(const unsigned char *)&FONT8x16,45+110,220,WHITE,-1);	
	else if(funcion==3)
	et024006_PrintString("Cronometro",(const unsigned char *)&FONT8x16,45+110,220,WHITE,-1);
}