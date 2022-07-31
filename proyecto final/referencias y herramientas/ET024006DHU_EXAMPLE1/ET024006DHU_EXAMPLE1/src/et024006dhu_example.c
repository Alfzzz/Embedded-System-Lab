#include <avr32/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpio.h"
#include "board.h"
#include "power_clocks_lib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "usart.h"
#include "adc.h"
#include "print_funcs.h"
#include "et024006dhu.h"
#include "avr32_logo.h"
#include "eic.h"
#include "conf_clock.h"
#include "pwm.h"
#include "delay.h"
#include "compiler.h"
#include "preprocessor.h"
#include "flashc.h"
#include "pm.h"
#include "tpa6130.h"
#include "abdac.h"
#include "conf_tpa6130.h"
#include "audio.h"
#include "sound.h"
#include "temperaturaAlta.h"
#include "bienvenido.h"
#include "lucesEncendidos.h"
#include "gasDetectado.h"
#include "twi.h"
#include "pdca.h"
#include "intc.h"
#include "sd_mmc_spi.h"
#include "conf_sd_mmc_spi.h"
#  define EXAMPLE_USART_CLOCK_MASK      AVR32_USART0_CLK_PBA
#  define EXAMPLE_PDCA_CLOCK_HSB        AVR32_PDCA_CLK_HSB
#  define EXAMPLE_PDCA_CLOCK_PB         AVR32_PDCA_CLK_PBA
#define PDCA_CHANNEL_SPI_RX	9
#define PDCA_CHANNEL_SPI_TX	10
#define PID_SPI_RX AVR32_PDCA_PID_SPI0_RX
#define PID_SPI_TX AVR32_PDCA_PID_SPI0_TX
//Definición de colores
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define ET024006_WIDTH 320 //WIDHT MAXIMO
#define ET024006_HEIGHT 240 //HEIGHT MAXIMO (320X240)#define SAMPLE_OFFSET   0x80#define SOUND_SAMPLES                256
#define FPBA_HZ                 62092800
#define TPA6130_TWI_MASTER_SPEED  100000
#define YELLOW 0xFFE0//! Welcome message to display.
#define MSG_WELCOME "\x1B[2J\x1B[H---------- Welcome to TPA6130 example ---------- \r\n"xSemaphoreHandle xMutex;xSemaphoreHandle xMutexSD;xSemaphoreHandle xMutexAudio;xTaskHandle myTask1Handle=NULL;xTaskHandle myTask2Handle=NULL;xTaskHandle myTask3Handle=NULL;xTaskHandle myTask4Handle=NULL;xTaskHandle myTask5Handle=NULL;xTaskHandle myTask6Handle=NULL;xTaskHandle myTask7Handle=NULL;signed short adc_value_temp  = -1;
signed short adc_value_light = -1;
signed short adc_value_pot   = -1;volatile short rxFlag = 0;xQueueHandle temperaturaQueue;xQueueHandle gasQueue;xQueueHandle iluminosidadQueue;xQueueHandle temperaturaSDQueue;xQueueHandle gasSDQueue;xQueueHandle iluminosidadSDQueue;
int16_t samples[SOUND_SAMPLES];
uint32_t samples_count;volatile char buffer_rx_spi[1000];volatile int buffer_rx;const char dummy[];volatile avr32_pdca_channel_t* pdca_channelrx ;
volatile avr32_pdca_channel_t* pdca_channeltx ;avr32_pwm_channel_t pwm_channel6 = {
  //.cdty = 0,
  .cdty = 0,
  .cprd = 100
};void vTareaLCD( void *pvParameters );//ADCvoid vTareaADC( void *pvParameters );//LCDvoid vTareaAudioGas( void *pvParameters );void vTareaAudioTemperatura( void *pvParameters );void vTareaAudioLuces( void *pvParameters );void vTareaSD( void *pvParameters );void vTareaSD_RX( void *pvParameters );static void tft_bl_init(void);void init_sys_clocks(void);
void master_callback(uint32_t arg);
//! Sample Count Value
void dac_reload_callback(void);
void dac_overrun_callback(void);
void adc_underrun_callback(void);
void adc_reload_callback(void);void left_right_enter_isr(void);void EIC(void);

void vTareaADC( void *pvParameters ){
	//const char *pcTaskName = "Task 1 is running\r\n";
	int sensorTemperatura;
	int sensorgas;
	int sensorIluminosidad;
	int sensorContador=0;
	temperaturaQueue=xQueueCreate(5,sizeof(sensorTemperatura));
	gasQueue=xQueueCreate(5,sizeof(sensorgas));
	iluminosidadQueue=xQueueCreate(5,sizeof(sensorIluminosidad));
	temperaturaSDQueue=xQueueCreate(5,sizeof(sensorTemperatura));
	gasSDQueue=xQueueCreate(5,sizeof(sensorgas));
	iluminosidadSDQueue=xQueueCreate(5,sizeof(sensorIluminosidad));
	while(1){
		if(xSemaphoreTake(xMutex,0xFFFFFFFF)==1){
			if(sensorContador==0){
				gpio_clr_gpio_pin(62);
				gpio_clr_gpio_pin(63);
				vTaskDelay(100);
				//usart_write_line(&AVR32_USART0, pcTaskName);
				adc_start(&AVR32_ADC);
				//adc_value_temp = adc_get_value(&AVR32_ADC,0);
				sensorTemperatura = adc_get_value(&AVR32_ADC,0);
				//gpio_clr_gpio_pin(23);
				sensorContador=1;
				usart_write_line(&AVR32_USART0,"Temperatura:");
				//usart_write_line(&AVR32_USART0,sensorTemperatura);
				//sensorTemperatura=sensorTemperatura/1023*50000/10;
				float temperatura=(float)sensorTemperatura;
				//const char *pcTaskName = "Task 1 is running\r\n";
				print_dbg_ulong(temperatura/80*11);
				//usart_write_line(&AVR32_USART0,"\r\n");
				xQueueSend(temperaturaQueue,&sensorTemperatura,0);
				vTaskDelay(100);
				xQueueSend(temperaturaSDQueue,&sensorTemperatura,0);
				temperatura=sensorTemperatura;
			}
			else if(sensorContador==1){
				gpio_set_gpio_pin(62);
				gpio_clr_gpio_pin(63);
				vTaskDelay(100);
				//usart_write_line(&AVR32_USART0, pcTaskName);
				adc_start(&AVR32_ADC);
				//adc_value_temp = adc_get_value(&AVR32_ADC,0);
				sensorgas = adc_get_value(&AVR32_ADC,0);
				//gpio_clr_gpio_pin(21);
				sensorContador=2;
				usart_write_line(&AVR32_USART0,"    Gas:");
				//usart_write_line(&AVR32_USART0,sensorgas);
				print_dbg_ulong(sensorgas);
				//usart_write_line(&AVR32_USART0,"\r\n");
				xQueueSend(gasQueue,&sensorgas,0);
				vTaskDelay(100);
				xQueueSend(gasSDQueue,&sensorgas,0);
			}
			else if(sensorContador==2){
				gpio_clr_gpio_pin(62);
				gpio_set_gpio_pin(63);
				vTaskDelay(100);
				//usart_write_line(&AVR32_USART0, pcTaskName);
				adc_start(&AVR32_ADC);
				//adc_value_temp = adc_get_value(&AVR32_ADC,0);
				sensorIluminosidad = adc_get_value(&AVR32_ADC,0);
				//gpio_clr_gpio_pin(21);
				sensorContador=0;
				usart_write_line(&AVR32_USART0,"    Iluminosidad:");
				//usart_write_line(&AVR32_USART0,sensorgas);
				print_dbg_ulong(sensorIluminosidad);
				usart_write_line(&AVR32_USART0,"\r\n");
				xQueueSend(iluminosidadQueue,&sensorIluminosidad,0);
				vTaskDelay(100);
				xQueueSend(iluminosidadSDQueue,&sensorIluminosidad,0);
			}
			xSemaphoreGive(xMutex);
		}
		vTaskDelay(1000);
	}
}void vTareaAudioTemperatura( void *pvParameters ){
	int i=0;
	int count=0;
	while(true){
		if(xSemaphoreTake(xMutexAudio,0xFFFFFFFF)==1){
			count=0;
			while(count < (SOUND_SAMPLES)){
				samples[count++] = ((uint8_t)temperaturaAlta[i]+0x80) << 8;
				samples[count++] = ((uint8_t)temperaturaAlta[i]+0x80) << 8;
				i++;
				if (i >= sizeof(temperaturaAlta)) i = 0;
			}
			tpa6130_dac_output((void *) samples,SOUND_SAMPLES/2);
			while(!tpa6130_dac_output(NULL, 0));
			xSemaphoreGive(xMutexAudio);
		}
		
	}
}void vTareaAudioGas( void *pvParameters ){
	int i=0;
	int count=0;
	while(true){
		if(xSemaphoreTake(xMutexAudio,0xFFFFFFFF)==1){
		count=0;
		while(count < (SOUND_SAMPLES)){
			samples[count++] = ((uint8_t)gasDetectado[i]+0x80) << 8;
			samples[count++] = ((uint8_t)gasDetectado[i]+0x80) << 8;
			i++;
			if (i >= sizeof(gasDetectado)) i = 0;
		}
		tpa6130_dac_output((void *) samples,SOUND_SAMPLES/2);
		while(!tpa6130_dac_output(NULL, 0));
		xSemaphoreGive(xMutexAudio);
	}
	}}void vTareaAudioLuces( void *pvParameters ){
	int i=0;
	int count=0;
	while(true){
		if(xSemaphoreTake(xMutexAudio,0xFFFFFFFF)==1){
		count=0;
		while(count < (SOUND_SAMPLES)){
			samples[count++] = ((uint8_t)lucesEncendidos[i]+0x80) << 8;
			samples[count++] = ((uint8_t)lucesEncendidos[i]+0x80) << 8;
			i++;
			if (i >= sizeof(lucesEncendidos)) i = 0;
		}
		tpa6130_dac_output((void *) samples,SOUND_SAMPLES/2);
		while(!tpa6130_dac_output(NULL, 0));
		xSemaphoreGive(xMutexAudio);
	}
	}}void vTareaLCD( void *pvParameters ){
	//const char *pcTaskName = "Task 3 is running\r\n";
	int temperaturaRecibido;
	int gasRecibido;
	int iluminosidadRecibido;
	int temperaturaPrimeraVez=1;
	int gasPrimeraVez=1;
	while(1){
		if(temperaturaQueue!=0){
			xQueueReceive(temperaturaQueue,(void*)&temperaturaRecibido,5);	
			float temperatura=(float)temperaturaRecibido;
			if((temperatura/80*11)>=14){
				if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)==0){
					et024006_PrintString("Mensajes guardados en SD:", (const unsigned char *)&FONT6x8, 0, 90, BLACK, -1);
 					et024006_DrawFilledRect(10 , 40, 20, 140, RED );
				}
				gpio_clr_gpio_pin(LED0_GPIO);
				//if(temperaturaPrimeraVez==1){
					//temperaturaPrimeraVez=0;
					//tpa6130_dac_mute(0);
					vTaskResume(myTask2Handle);
					//vTaskDelay(10000);
					//tpa6130_dac_mute(1);
					//vTaskSuspend(myTask2Handle);	
				//}
 			}
 			else{
				 if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)==0){
					 et024006_PrintString("Mensajes guardados en SD:", (const unsigned char *)&FONT6x8, 0, 90, BLACK, -1);
 					et024006_DrawFilledRect(10 , 40, 20, 140,BLUE );
				 }
					gpio_set_gpio_pin(LED0_GPIO);
					vTaskSuspend(myTask2Handle);
			}
		}
		if(gasQueue!=0){
			xQueueReceive(gasQueue,(void*)&gasRecibido,5);
			if(gasRecibido>=250){
				
 				et024006_DrawFilledRect(140, 100, 60, 40, RED );
 				et024006_DrawHorizLine(120,100,100,RED);
 				et024006_DrawLine(120,100,170,60,RED);
 				et024006_DrawLine(170,60,220,100,RED);
				 gpio_clr_gpio_pin(LED1_GPIO);
				//if(gasPrimeraVez==1){
					//gasPrimeraVez=0;
					//tpa6130_dac_mute(0);
					vTaskResume(myTask4Handle);
					//vTaskDelay(10000);
					//tpa6130_dac_mute(1);
					//vTaskSuspend(myTask4Handle);
				//}
 			}
 			else{
				 
				 et024006_DrawFilledRect(140, 100, 60, 40, RED );
				 et024006_DrawFilledRect(145, 105, 50, 30, BLACK );
				 et024006_DrawHorizLine(120,100,100,RED);
				 et024006_DrawLine(120,100,170,60,RED);
				 et024006_DrawLine(170,60,220,100,RED);
				 gpio_set_gpio_pin(LED1_GPIO);
				 vTaskSuspend(myTask4Handle);
			 }
		}
		if(iluminosidadQueue!=0){
			xQueueReceive(iluminosidadQueue,(void*)&iluminosidadRecibido,5);
			if(iluminosidadRecibido>=900){
				
				et024006_DrawFilledCircle(280,90,30,BLUE,0xFF);
				et024006_DrawFilledCircle(280,90,29,BLACK,0xFF);
				et024006_DrawLine(240,120,250,110,BLUE);
				et024006_DrawLine(240,100,245,90,BLUE);
				et024006_DrawLine(240,70,245,80,BLUE);
				et024006_DrawLine(240,50,250,60,BLUE);
				et024006_DrawVertLine(270,40,10,BLUE);
				et024006_DrawVertLine(290,40,10,BLUE);
				et024006_DrawLine(310,60,320,50,BLUE);
				et024006_DrawLine(315,80,320,70,BLUE);
				et024006_DrawLine(315,90,320,100,BLUE);
				et024006_DrawLine(310,110,320,120,BLUE);
				et024006_DrawLine(270,140, 290, 140, BLUE);
				et024006_DrawLine(270,135, 290, 135, BLUE);
				et024006_DrawLine(270,130, 290, 130, BLUE);
				et024006_DrawLine(270,125, 290, 125, BLUE);
				et024006_DrawLine(270,120, 290, 120, BLUE);
				et024006_DrawLine(270,140, 270, 120, BLUE);
				et024006_DrawLine(290,140, 290, 120, BLUE);
				gpio_set_gpio_pin(LED2_GPIO);
				//if(gasPrimeraVez==1){
				//gasPrimeraVez=0;
				//tpa6130_dac_mute(0);
				vTaskResume(myTask5Handle);
				//vTaskDelay(10000);
				//tpa6130_dac_mute(1);
				//vTaskSuspend(myTask4Handle);
				//}
			}
			else{
				et024006_DrawFilledCircle(280,90,30,BLUE,0xFF);
				et024006_DrawFilledCircle(280,90,29,YELLOW,0xFF);
				et024006_DrawLine(240,120,250,110,BLUE);
				et024006_DrawLine(240,100,245,90,BLUE);
				et024006_DrawLine(240,70,245,80,BLUE);
				et024006_DrawLine(240,50,250,60,BLUE);
				et024006_DrawVertLine(270,40,10,BLUE);
				et024006_DrawVertLine(290,40,10,BLUE);
				et024006_DrawLine(310,60,320,50,BLUE);
				et024006_DrawLine(315,80,320,70,BLUE);
				et024006_DrawLine(315,90,320,100,BLUE);
				et024006_DrawLine(310,110,320,120,BLUE);
				et024006_DrawLine(270,140, 290, 140, BLUE);
				et024006_DrawLine(270,135, 290, 135, BLUE);
				et024006_DrawLine(270,130, 290, 130, BLUE);
				et024006_DrawLine(270,125, 290, 125, BLUE);
				et024006_DrawLine(270,120, 290, 120, BLUE);
				et024006_DrawLine(270,140, 270, 120, BLUE);
				et024006_DrawLine(290,140, 290, 120, BLUE);
				gpio_clr_gpio_pin(LED2_GPIO);
				vTaskSuspend(myTask5Handle);
			}
		}
		vTaskDelay(100);
	}
}void vTareaSD( void *pvParameters ){
	int temperaturaSDRecibido;
	int gasSDRecibido;
	int iluminosidadSDRecibido;
	int seccion=5;
	while(1){
		if(xSemaphoreTake(xMutexSD,0xFFFFFFFF)==1){
			if(temperaturaSDQueue!=0){
				xQueueReceive(temperaturaSDQueue,(void*)&temperaturaSDRecibido,5);
				float temperatura=(float)temperaturaSDRecibido;
				sprintf(buffer_rx,"%d",10000);//+(temperatura/80*23));
				if (sd_mmc_spi_mem_check()){
					sd_mmc_spi_write_open(seccion);
					//et024006_PrintString(buffer_rx,(const unsigned char *)&FONT8x16,45,115,RED,-1);
					sd_mmc_spi_write_sector_from_ram(&buffer_rx);
					sd_mmc_spi_write_close();
					seccion++;
					if(seccion >= 20){
						seccion = 5;
					}
				}
			}
			if(gasSDQueue!=0){
				xQueueReceive(gasSDQueue,(void*)&gasSDRecibido,5);
				if (sd_mmc_spi_mem_check()){
					sd_mmc_spi_write_open(seccion);
					sprintf(buffer_rx,"%d",20000+gasSDRecibido);
					//et024006_PrintString(buffer_rx,(const unsigned char *)&FONT8x16,45,115,RED,-1);
					sd_mmc_spi_write_sector_from_ram(&buffer_rx);
					sd_mmc_spi_write_close();
					seccion++;
					if(seccion >= 20){
						seccion = 5;
					}
				}
			}
			if(iluminosidadSDQueue!=0){
				xQueueReceive(iluminosidadSDQueue,(void*)&iluminosidadSDRecibido,5);
				if (sd_mmc_spi_mem_check()){
					sd_mmc_spi_write_open(seccion);
					sprintf(buffer_rx,"%d",30000+iluminosidadSDRecibido);
					//et024006_PrintString(buffer_rx,(const unsigned char *)&FONT8x16,45,115,RED,-1);
					sd_mmc_spi_write_sector_from_ram(&buffer_rx);
					sd_mmc_spi_write_close();
					seccion++;
					if(seccion >= 20){
						seccion = 5;
					}
				}
			}
			xSemaphoreGive(xMutexSD);
			vTaskDelay(100);
		}
	}}void vTareaSD_RX( void *pvParameters ){
	while(1){
		if(xSemaphoreTake(xMutexSD,0xFFFFFFFF)==1){
			if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)==1){
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
			et024006_PrintString("Mensajes guardados en SD:", (const unsigned char *)&FONT6x8, 0, 90, RED, -1);
				for(int k = 5; k <=20; k++){
				/*DMA_SPI_RX();
				DMA_SPI_TX();
				pdca_load_channel(PDCA_CHANNEL_SPI_RX, &buffer_rx_spi, 512);
				pdca_load_channel(PDCA_CHANNEL_SPI_TX, (void *)&dummy, 512); //send dummy to activate the clock
				sd_mmc_spi_read_open_PDCA (k);
				spi_write(SD_MMC_SPI,0xFF); // Write a first dummy data to synchronize transfer
				pdca_enable_interrupt_transfer_complete(PDCA_CHANNEL_SPI_RX);
				pdca_channelrx =(volatile avr32_pdca_channel_t*) pdca_get_handler(PDCA_CHANNEL_SPI_RX); // get the correct PDCA channel pointer
				pdca_channeltx =(volatile avr32_pdca_channel_t*) pdca_get_handler(PDCA_CHANNEL_SPI_TX); // get the correct PDCA channel pointer
				pdca_channelrx->cr = AVR32_PDCA_TEN_MASK; // Enable RX PDCA transfer first
				pdca_channeltx->cr = AVR32_PDCA_TEN_MASK; // and TX PDCA transfer
				while(rxFlag == 0);
				et024006_PrintString(buffer_rx_spi, (const unsigned char *)&FONT6x8, 0, 100+(k*10), RED, -1);
				rxFlag = 0;*/
					if (sd_mmc_spi_mem_check()){
						sd_mmc_spi_read_open(k);
						spi_write(SD_MMC_SPI,0xFF); // Write a first dummy data to synchronize transfer
						sd_mmc_spi_read_sector_to_ram(&buffer_rx_spi);
						et024006_PrintString(buffer_rx_spi, (const unsigned char *)&FONT6x8, 0, 100+(k*5), RED, -1);
						
						sd_mmc_spi_read_close();
				
					}
				}
			}
			//gpio_clr_gpio_pin(LED3_GPIO);
			xSemaphoreGive(xMutexSD);
		}
	}}int main( void ){
	init_sys_clocks();
	
	et024006_Init( FPBA_HZ, FPBA_HZ );
	tft_bl_init();
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
	while(pwm_channel6.cdty < pwm_channel6.cprd){
		pwm_channel6.cdty++;
		pwm_channel6.cupd = pwm_channel6.cdty;
		//pwm_channel6.cdty--;
		pwm_async_update_channel(AVR32_PWM_ENA_CHID6, &pwm_channel6);
		delay_ms(10);
	}
	//
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
	//Temperatura
	et024006_DrawFilledRect(10 , 40, 20, 140, WHITE );
	//gas
	et024006_DrawFilledRect(140, 100, 60, 40, RED );
	et024006_DrawFilledRect(145, 105, 50, 30, BLACK );
	et024006_DrawHorizLine(120,100,100,RED);
	et024006_DrawLine(120,100,170,60,RED);
	et024006_DrawLine(170,60,220,100,RED);
	//iluminosidad
	et024006_DrawFilledCircle(280,90,30,BLUE,0xFF);
	et024006_DrawFilledCircle(280,90,29,BLACK,0xFF);
	et024006_DrawLine(240,120,250,110,BLUE);
	et024006_DrawLine(240,100,245,90,BLUE);
	et024006_DrawLine(240,70,245,80,BLUE);
	et024006_DrawLine(240,50,250,60,BLUE);
	et024006_DrawVertLine(270,40,10,BLUE);
	et024006_DrawVertLine(290,40,10,BLUE);
	et024006_DrawLine(310,60,320,50,BLUE);
	et024006_DrawLine(315,80,320,70,BLUE);
	et024006_DrawLine(315,90,320,100,BLUE);
	et024006_DrawLine(310,110,320,120,BLUE);
	et024006_DrawLine(270,140, 290, 140, BLUE);
	et024006_DrawLine(270,135, 290, 135, BLUE);
	et024006_DrawLine(270,130, 290, 130, BLUE);
	et024006_DrawLine(270,125, 290, 125, BLUE);
	et024006_DrawLine(270,120, 290, 120, BLUE);
	et024006_DrawLine(270,140, 270, 120, BLUE);
	et024006_DrawLine(290,140, 290, 120, BLUE);
	
	static const gpio_map_t GPIO_MAP ={
		{0, 0},//RX GPIO 0, función 0 RX
		{1, 0},//TX GPIO 1,función 0 TX
		{21, 0},//ADC canal 0, función 0 
		{23, 0},//ADC canal 2, función 0
		{TPA6130_TWI_SCL_PIN, TPA6130_TWI_SCL_FUNCTION},//TWI
		{TPA6130_TWI_SDA_PIN, TPA6130_TWI_SDA_FUNCTION},//TWI
		{SD_MMC_SPI_SCK_PIN,  SD_MMC_SPI_SCK_FUNCTION },  // SPI Clock.
		{SD_MMC_SPI_MISO_PIN, SD_MMC_SPI_MISO_FUNCTION},  // MISO.
		{SD_MMC_SPI_MOSI_PIN, SD_MMC_SPI_MOSI_FUNCTION},  // MOSI.
		{SD_MMC_SPI_NPCS_PIN, SD_MMC_SPI_NPCS_FUNCTION}   // Chip Select NPCS.
	};
	
	gpio_enable_module(GPIO_MAP,sizeof(GPIO_MAP) / sizeof(GPIO_MAP[0]));
	  const twi_options_t TPA6130_TWI_OPTIONS ={
		  .pba_hz = 62092800,
		  .speed  = TPA6130_TWI_MASTER_SPEED,
		  .chip   = TPA6130_TWI_ADDRESS
	  };
	  // Initialize as master.
	twi_master_init(TPA6130_TWI, &TPA6130_TWI_OPTIONS);
	tpa6130_init();
	tpa6130_dac_start(DEFAULT_DAC_SAMPLE_RATE_HZ,//Sample Rate
		DEFAULT_DAC_NUM_CHANNELS,//Channel number 1-2
		DEFAULT_DAC_BITS_PER_SAMPLE,//Bits per sample
		DEFAULT_DAC_SWAP_CHANNELS,//Swap Left right channel
		master_callback,
		AUDIO_DAC_OUT_OF_SAMPLE_CB
		| AUDIO_DAC_RELOAD_CB,
		62092800);//Frecuencia del reloj síncrono PLL0
	tpa6130_set_volume(0x3F);//6 bits
	 
// USART options.
	static const usart_options_t USART_OPTIONS ={
		.baudrate     = 57600,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};

	// Initialize USART in RS232 mode.
	usart_init_rs232(&AVR32_USART0, &USART_OPTIONS,62092800);	
	// SPI options.
	
	spi_options_t spiOptions =
	{
		.reg          = SD_MMC_SPI_NPCS,
		.baudrate     = SD_MMC_SPI_MASTER_SPEED,  // Defined in conf_sd_mmc_spi.h.
		.bits         = SD_MMC_SPI_BITS,          // Defined in conf_sd_mmc_spi.h.
		.spck_delay   = 0,
		.trans_delay  = 0,
		.stay_act     = 1,
		.spi_mode     = 0,
		.modfdis      = 1
	};
	// Initialize as master.
	spi_initMaster(SD_MMC_SPI, &spiOptions);
	// Set SPI selection mode: variable_ps, pcs_decode, delay.
	spi_selectionMode(SD_MMC_SPI, 0, 0, 0);
	// Enable SPI module.
	spi_enable(SD_MMC_SPI);
	// Initialize SD/MMC driver with SPI clock (PBA).
	sd_mmc_spi_init(spiOptions, FPBA_HZ);
	
	AVR32_ADC.mr |= 0x1 << AVR32_ADC_MR_PRESCAL_OFFSET;
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, 0);//Habilitar ADC canal 0
	adc_enable(&AVR32_ADC, 2);//Habilitar ADC canal 2
	
	
	xTaskCreate( vTareaADC, "Task 1",	1000, NULL,1,  &myTask1Handle);
	xTaskCreate( vTareaAudioTemperatura, "Task 2", 1000, NULL, 1, &myTask2Handle);
	xTaskCreate( vTareaLCD, "Task 3", 1000, NULL, 1, &myTask3Handle);
	xTaskCreate( vTareaAudioGas, "Task 4", 1000, NULL, 1, &myTask4Handle);
	xTaskCreate( vTareaAudioLuces, "Task 5", 1000, NULL, 1, &myTask5Handle);
	xTaskCreate( vTareaSD, "Task 6", 1000, NULL, 1, &myTask6Handle);
	xTaskCreate( vTareaSD_RX, "Task 7", 1000, NULL, 1, &myTask7Handle);
	vTaskSuspend(myTask2Handle);
	vTaskSuspend(myTask4Handle);
	vTaskSuspend(myTask5Handle);
	//vTaskSuspend(myTask7Handle);
	xMutex=xSemaphoreCreateMutex();
	xMutexSD=xSemaphoreCreateMutex();
	xMutexAudio=xSemaphoreCreateMutex();    
	 //EIC();
	 //Enable_global_interrupt();
// 	Disable_global_interrupt();
// 	INTC_register_interrupt(&left_right_enter_isr, 71, 3);
// 	gpio_enable_pin_interrupt(58, 1);
// 	gpio_enable_pin_interrupt(57, 1);
	
//	Enable_global_interrupt();

	//Start the scheduler so the tasks start executing. 
	vTaskStartScheduler(); 
	while(true){
	}
}
__attribute__ ((__interrupt__)) // atributo para interrupciones
void left_right_enter_isr(void){
	if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)==1){
		/*et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
		et024006_PrintString("Mensajes guardados en SD:", (const unsigned char *)&FONT6x8, 0, 90, RED, -1);
		for(int k = 5; k <= 9; k++){
			DMA_SPI_RX();
			DMA_SPI_TX();
			pdca_load_channel(PDCA_CHANNEL_SPI_RX, &buffer_rx_spi, 512);
			pdca_load_channel(PDCA_CHANNEL_SPI_TX, (void *)&dummy, 512); //send dummy to activate the clock
			sd_mmc_spi_read_open_PDCA (k);
			spi_write(SD_MMC_SPI,0xFF); // Write a first dummy data to synchronize transfer
			pdca_enable_interrupt_transfer_complete(PDCA_CHANNEL_SPI_RX);
			pdca_channelrx =(volatile avr32_pdca_channel_t*) pdca_get_handler(PDCA_CHANNEL_SPI_RX); // get the correct PDCA channel pointer
			pdca_channeltx =(volatile avr32_pdca_channel_t*) pdca_get_handler(PDCA_CHANNEL_SPI_TX); // get the correct PDCA channel pointer
			pdca_channelrx->cr = AVR32_PDCA_TEN_MASK; // Enable RX PDCA transfer first
			pdca_channeltx->cr = AVR32_PDCA_TEN_MASK; // and TX PDCA transfer
			while(rxFlag == 0);
			et024006_PrintString(buffer_rx_spi, (const unsigned char *)&FONT6x8, 0, 100+(k*10), RED, -1);
			rxFlag = 0;
		}*/
		//gpio_tgl_gpio_pin(LED3_GPIO);
		//xTaskResumeFromISR(myTask7Handle);

		//gpio_clear_pin_interrupt_flag(QT1081_TOUCH_SENSOR_ENTER);
		//gpio_clear_pin_interrupt_flag(QT1081_TOUCH_SENSOR_LEFT);
		//gpio_clear_pin_interrupt_flag(QT1081_TOUCH_SENSOR_RIGHT);
		
	}

	eic_clear_interrupt_line(&AVR32_EIC,1);
}
static void tft_bl_init(void){
	pwm_opt_t opt = {
		.diva = 0,
		.divb = 0,
		.prea = 0,
		.preb = 0
	};
	pwm_init(&opt);
	pwm_channel6.CMR.calg = PWM_MODE_LEFT_ALIGNED;
	pwm_channel6.CMR.cpol = PWM_POLARITY_HIGH; //PWM_POLARITY_LOW;//PWM_POLARITY_HIGH;
	pwm_channel6.CMR.cpd = PWM_UPDATE_DUTY;
	pwm_channel6.CMR.cpre = AVR32_PWM_CMR_CPRE_MCK_DIV_2;

	pwm_channel_init(6, &pwm_channel6);
	pwm_start_channels(AVR32_PWM_ENA_CHID6_MASK);
}

void master_callback(uint32_t arg){
	if( arg == AUDIO_DAC_OUT_OF_SAMPLE_CB ){
		dac_overrun_callback();
	}
	else if( arg == AUDIO_DAC_RELOAD_CB ){
		dac_reload_callback();
	}
	else if( arg == AUDIO_ADC_OUT_OF_SAMPLE_CB ){
		adc_underrun_callback();;
	}
	else if( arg == AUDIO_ADC_RELOAD_CB ){
		adc_reload_callback();;
	}
}

void dac_reload_callback(void){
  // Nothing todo
}

void dac_overrun_callback(void){
  // Nothing todo
}


void adc_underrun_callback(void){
  // Nothing todo
}


void adc_reload_callback(void){
  // Nothing todo
}


void init_sys_clocks(void)	{
	// Switch to OSC0 to speed up the booting
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

	// Start oscillator1
	pm_enable_osc1_crystal(&AVR32_PM, FOSC1);
	pm_enable_clk1(&AVR32_PM, OSC1_STARTUP);

	// Set PLL0 (fed from OSC1 = 11.2896 MHz) to 124.1856 MHz
	// We use OSC1 since we need a correct master clock for the SSC module to generate
	// the correct sample rate
	pm_pll_setup(&AVR32_PM, 0,  // pll.
	10,  // mul.
	1,   // div.
	1,   // osc.
	16); // lockcount.

	// Set PLL operating range and divider (fpll = fvco/2)
	// -> PLL0 output = 62.0928 MHz
	pm_pll_set_option(&AVR32_PM, 0, // pll.
	1,  // pll_freq.
	1,  // pll_div2.
	0); // pll_wbwdisable.

	// start PLL0 and wait for the lock
	pm_pll_enable(&AVR32_PM, 0);
	pm_wait_for_pll0_locked(&AVR32_PM);
	// Set all peripheral clocks torun at master clock rate
	pm_cksel(&AVR32_PM,
	0,   // pbadiv.
	0,   // pbasel.
	0,   // pbbdiv.
	0,   // pbbsel.
	0,   // hsbdiv.
	0);  // hsbsel.

	// Set one waitstate for the flash
	flashc_set_wait_state(1);

	// Switch to PLL0 as the master clock
	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCCTRL_MCSEL_PLL0);

	// Use 12MHz from OSC0 and generate 96 MHz
	pm_pll_setup(&AVR32_PM, 1,  // pll.
	7,   // mul.
	1,   // div.
	0,   // osc.
	16); // lockcount.

	pm_pll_set_option(&AVR32_PM, 1, // pll.
	1,  // pll_freq: choose the range 80-180MHz.
	1,  // pll_div2.
	0); // pll_wbwdisable.

	// start PLL1 and wait forl lock
	pm_pll_enable(&AVR32_PM, 1);

	// Wait for PLL1 locked.
	pm_wait_for_pll1_locked(&AVR32_PM);

}
void EIC(void) {
	Disable_global_interrupt();
	INTC_init_interrupts();
	INTC_register_interrupt(&left_right_enter_isr, 33, 1); //Handler (Nombre de la funcion de interrupcion), IRQ, Nivel de prioridad (0,1,2,3 /mayor prioridad 3)
	eic_options_t eic_options;
	eic_options.eic_mode  = EIC_MODE_EDGE_TRIGGERED; //Interrupcion por nivel o por frente
	eic_options.eic_edge  = EIC_EDGE_RISING_EDGE; //Rising o Falling de acuerdo con el modo anterior
	eic_options.eic_async = EIC_SYNCH_MODE; //Modo sincrono o asincrono
	eic_options.eic_line  = 1; //Numero de linea
	eic_init(&AVR32_EIC, &eic_options, 1); //Inicializar el EIC
	eic_enable_line(&AVR32_EIC, 1); //Habilitar la linea
	eic_enable_interrupt_line(&AVR32_EIC, 1); //Habilitar la interrupcion para la linea
}