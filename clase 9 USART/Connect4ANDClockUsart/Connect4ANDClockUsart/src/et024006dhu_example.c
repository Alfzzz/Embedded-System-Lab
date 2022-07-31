#include "board.h"
#include "gpio.h"
#include "power_clocks_lib.h"
#include "et024006dhu.h"
#include "delay.h"
#include "avr32_logo.h"
#include "conf_clock.h"
#include <avr32/io.h>
#include "compiler.h"
#include "usart.h"
#include "pwm.h"
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

#include "pwm.h"
#  define EXAMPLE_USART_RX_PIN          AVR32_USART0_RXD_0_0_PIN
#  define EXAMPLE_USART_RX_FUNCTION     AVR32_USART0_RXD_0_0_FUNCTION
#  define EXAMPLE_USART_TX_PIN          AVR32_USART0_TXD_0_0_PIN
#  define EXAMPLE_USART_TX_FUNCTION     AVR32_USART0_TXD_0_0_FUNCTION
#  define EXAMPLE_USART_CLOCK_MASK      AVR32_USART0_CLK_PBA
#  define EXAMPLE_PDCA_CLOCK_HSB        AVR32_PDCA_CLK_HSB
#  define EXAMPLE_PDCA_CLOCK_PB         AVR32_PDCA_CLK_PBA

int board[6][7];
int step=0;//Step 0:ask for name, step 1: ask for image, step 2:play
char userName[10];
int userNameIndex=0;
int instruction_flag=0;
int position = 0;
char img[20*20*3];
int imgIndex=0;
int turn=1;
avr32_pwm_channel_t pwm_channel6 = {
  .cdty = 0,
  .cprd = 100
};

static void tft_bl_init(void)
{

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
void timer1ms(void);
int checkWin(void);
void play(int position);
// Main function
int main(void){
	srand(time(0));//generate random seeds
	// Set CPU and PBA clock
	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

	gpio_enable_gpio_pin(LED0_GPIO);
	gpio_enable_gpio_pin(LED1_GPIO);
	gpio_enable_gpio_pin(LED2_GPIO);
	gpio_enable_gpio_pin(LED3_GPIO);

	et024006_Init( FOSC0, FOSC0 );

	tft_bl_init();

	// Clear the display i.e. make it black
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );

	/* Lets do a nice fade in by increasing the duty cycle */
	while(pwm_channel6.cdty < pwm_channel6.cprd)
	{
	pwm_channel6.cdty++;
	pwm_channel6.cupd = pwm_channel6.cdty;
	//pwm_channel6.cdty--;
	pwm_async_update_channel(AVR32_PWM_ENA_CHID6, &pwm_channel6);
	delay_ms(10);
	}
	static const gpio_map_t USART_GPIO_MAP ={
		{EXAMPLE_USART_RX_PIN, EXAMPLE_USART_RX_FUNCTION},
		{EXAMPLE_USART_TX_PIN, EXAMPLE_USART_TX_FUNCTION}
	};
	// USART options.
	static const usart_options_t USART_OPTIONS ={
		.baudrate     = 57600,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};

	// Assign GPIO to USART.
	gpio_enable_module(USART_GPIO_MAP,
	sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

	// Initialize USART in RS232 mode.
	usart_init_rs232(&AVR32_USART0, &USART_OPTIONS, 12000000);
	static const tc_waveform_opt_t WAVEFORM_OPT ={
		.channel  = 0,                        // Channel selection.

		.bswtrg   = AVR32_TC_NONE,					   // Software trigger effect on TIOB.
		.beevt    = AVR32_TC_NONE,					   // External event effect on TIOB.
		.bcpc     = AVR32_TC_NONE,					   // RC compare effect on TIOB.
		.bcpb     = AVR32_TC_NONE,					   // RB compare effect on TIOB.

		.aswtrg   = AVR32_TC_NONE,					   // Software trigger effect on TIOA.
		.aeevt    = AVR32_TC_NONE,					   // External event effect on TIOA.
		.acpc     = AVR32_TC_NONE,					   // RC compare effect on TIOA: toggle.
		.acpa     = AVR32_TC_NONE,					   // RA compare effect on TIOA: toggle (other possibilities are none, set and clear).

		.wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,// 2 Waveform selection: Up mode with automatic trigger(reset) on RC compare.
		.enetrg   = false,                             // External event trigger enable.
		.eevt     = 0,                                 // External event selection.
		.eevtedg  = TC_SEL_NO_EDGE,                    // External event edge selection.
		.cpcdis   = false,                             // Counter disable when RC compare.
		.cpcstop  = false,                             // Counter clock stopped with RC compare.

		.burst    = false,                             // Sin Burst
		.clki     = false,                             // Sin clock inversion.
		.tcclks   = TC_CLOCK_SOURCE_TC3				   // Internal source clock 3, connected to fPBA / 8.
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
	//Desabilitar interrupciones
	Disable_global_interrupt();
	//Incialización de vectores, uno por cada grupo
	INTC_init_interrupts();

	//Asignar ISR para interrupción con IRQ con nivel
	INTC_register_interrupt(&timer1ms,448, 0);//Canal 0 IRQ=448,nivel 0 de prioridad
	// Initialize the timer/counter.
	Enable_global_interrupt();

	tc_init_waveform(&AVR32_TC, &WAVEFORM_OPT);         // Initialize the timer/counter waveform.
	tc_write_rc(&AVR32_TC, 0, 1500);            //Channel 0, Set RC value.
	tc_configure_interrupts(&AVR32_TC, 0, &TC_INTERRUPT);//Channel 0
	// Start the timer/counter.
	usart_write_line(&AVR32_USART0, "Hello, this is the AVR UC3 MCU saying hello! (press enter)\r\n");
	for(int i=0;i<6;i++){
		for(int j=0;j<7;j++){
			board[i][j]=0;
		}
	}
	tc_start(&AVR32_TC, 0);                    // And start the timer/counter with channel 0.
	while(true);
}
__attribute__ ((__interrupt__))	//Declaración de que la función trabaja por interrupción
void timer1ms(void){
	//if(gpio_get_pin_interrupt_flag (GPIO_number)){//Identifciar de qué PIN fue la interrupción
	// Aqui se ubica la rutina de atención a interrupción
	tc_read_sr(&AVR32_TC, 0);
	//Limpiar la bandera correspondiente al final de la interrupción
	//gpio_clear_pin_interrupt_flag(1);//RX
	if(step==0){
		if(instruction_flag==0){
			usart_write_line(&AVR32_USART0, "Please type the characters of your name\r\n");
			turn=1;
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			for(int i=0;i<7;i++){
				for(int j=0;j<6;j++){
					et024006_DrawFilledCircle(55+15+i*30,30+15+j*30,15,WHITE,0xFF);						
				}		
			}
			et024006_DrawFilledCircle(300,120,15,BLUE,0xFF);	
			et024006_DrawFilledCircle(300,120,12,BLACK,0xFF);
			et024006_DrawFilledCircle(300,122,7,BLUE,0xF0);
			et024006_DrawFilledCircle(300,122,5,BLACK,0xF0);
			et024006_DrawFilledCircle(295,115,2,BLUE,0xFF);
			et024006_DrawFilledCircle(305,115,2,BLUE,0xFF);
			et024006_PrintString("EVK Bot", (const unsigned char *)&FONT8x8, 265,50, BLUE, -1);					
			instruction_flag=1;
		}
		int data=usart_getchar(&AVR32_USART0);
		if(data=='\r'){
			step=step++;
			userNameIndex=0;
			et024006_PrintString(userName, (const unsigned char *)&FONT8x8, 5,50, RED, -1);
			for(int i=0;i<10;i++){
				userName[i]=' ';
			}
			instruction_flag=0;
		}
		else{
			userName[userNameIndex]=data;
			userNameIndex++;
		}
		
	}
	if(step==1){
		if(instruction_flag==0){
			usart_write_line(&AVR32_USART0, "Please send image\r\n");
			instruction_flag=1;
		}
		int data=usart_getchar(&AVR32_USART0);
		if(data==100){
			step++;
			imgIndex=0;
			int color=0;
			for(int i=0;i<(20*3);i=i+3){
				for(int j=0;j<20;j++){
					color=img[i+j*20*3]/*B:5*/ | ((img[i+j*20*3+1])<<5)/*G:6*/ | ((img[i+j*20*3+2])<<11)/*R:5*/ ;
					et024006_DrawPixel(30+i/3,100+j,color);
					//et024006_DrawQuickPixel(100+i/3,100+j,color);
				}
			}
			//color=img[0]/*B:5*/ | ((img[1])<<5)/*G:6*/ | ((img[2])<<11)/*R:5*/ ;
			//et024006_DrawFilledRect(0,0,320,240,color);
			//et024006_PrintString(userName, (const unsigned char *)&FONT8x8, 5,120, RED, -1);
			instruction_flag=0;
		}
		else{
			img[imgIndex]=data;
			imgIndex++;
		}	
	}
		
	if(step==2){
		if(instruction_flag==0){
			usart_write_line(&AVR32_USART0, "Let's play(Enter number from 1 to 7) \r\n");
			instruction_flag=1;
		}
					
		//Dibujamos el tablero
		int position;			
		if(turn==1){
			position=usart_getchar(&AVR32_USART0);
		}
		else{
			position='1';
		}
		play(position);
		//if(data=='\r'){
		//step++;
			
		//str[1]=data;
		//et024006_PrintString(str, (const unsigned char *)&FONT8x8, 10,120, RED, -1);
		//instruction_flag=0;
		//}

		int winner=checkWin();
		if(winner==1){
			gpio_clr_gpio_pin(LED0_GPIO);
			gpio_set_gpio_pin(LED1_GPIO);
			usart_write_line(&AVR32_USART0, "Congratulations! You beat EVK Bot ! \r\n");
			step=0;	
			instruction_flag=0;
			for(int i=0;i<6;i++){
				for(int j=0;j<7;j++){
					board[i][j]=0;
				}
			}
				
		}
		if(winner==2){
			gpio_clr_gpio_pin(LED1_GPIO);
			gpio_set_gpio_pin(LED0_GPIO);
			usart_write_line(&AVR32_USART0, "Sorry, you lose to EVK Bot!Try next time \r\n");
			step=0;		
			instruction_flag=0;
			for(int i=0;i<6;i++){
				for(int j=0;j<7;j++){
					board[i][j]=0;
				}
			}
		}
		if(winner==4){
			usart_write_line(&AVR32_USART0, "Tie game! \r\n");
			step=0;
			instruction_flag=0;
			for(int i=0;i<6;i++){
				for(int j=0;j<7;j++){
					board[i][j]=0;
				}
			}
		}
	}
}

int checkWin(void){
	//Vertical Check
	int countVer=0;
	int countHor=0;
	int winner=0;
	for(int col=0;col<7;col++){
		for(int row=1;row<6;row++){
			//usart_putchar(&AVR32_USART0,'0'+countVer);
			if(board[row][col]==board[row-1][col] && board[row][col]!=0){
				countVer++;
				if(countVer==3){
					winner=board[row][col];
					break;
				}
			}
			else{
				countVer=0;
			}
			if(row==5)
				countVer=0;
		}
	}
	for(int row=0;row<6;row++){
		for(int col=1;col<7;col++){
			if(board[row][col]==board[row][col-1] && board[row][col]!=0){
				countHor++;
				if(countHor==3){
					winner=board[row][col];
					break;
				}
			}
			else{
				countHor=0;
			}
			if(col==6)
				countVer=0;
		}
	}
	
	for(int row=0;row<3;row++){
		for(int col=0;col<4;col++){
			
			
			if(board[row][col]==board[row+1][col+1] && board[row][col]!=0){
				if(board[row+1][col+1]==board[row+2][col+2]){
					if(board[row+2][col+2]==board[row+3][col+3]){
						winner=board[row][col];
						break;
					}
				}
			}
			
			if(board[row][6-col]==board[row+1][6-col-1] && board[row][6-col]!=0){
				if(board[row+1][6-col-1]==board[row+2][6-col-2]){
					if(board[row+2][6-col-2]==board[row+3][6-col-3]){
						winner=board[row][6-col];
						break;
					}
				}
			}
			
		}
	}
	int sum=0;
	for(int i=0;i<7;i++){
		sum=sum+board[0][i];
	}
	
	 //usart_write_char(&AVR32_USART0,countVer);
	 //usart_write_char(&AVR32_USART0,countHor);
	 //usart_write_char(&AVR32_USART0,winner);
	if(winner!=0)
		return winner;
	else if(sum>6 && winner==0)
		return 4;
	else 
		return 0;
}

void play(int position){
	if(turn==1){
		if (position == '1'){
			for(int i=0;i<6;i++){
				if(board[0][0]!=0){
					usart_write_line(&AVR32_USART0, "This column is full(Enter another one) \r\n");
				}
				if(board[5-i][0]==0){
					et024006_DrawFilledCircle(55+15,195-i*30,15,RED,0xFF);
					board[5-i][0]=1;
					turn=2;
					break;
				}
			}
		}
		if (position == '2'){
			for(int i=0;i<6;i++){
				if(board[0][1]!=0){
					usart_write_line(&AVR32_USART0, "This column is full(Enter another one) \r\n");
				}
				if(board[5-i][1]==0){
					et024006_DrawFilledCircle(55+15+30,195-i*30,15,RED,0xFF);
					board[5-i][1]=1;
					turn=2;
					break;
				}
			}
		}
		if (position == '3'){
			for(int i=0;i<6;i++){
				if(board[0][2]!=0){
					usart_write_line(&AVR32_USART0, "This column is full(Enter another one) \r\n");
				}
				if(board[5-i][2]==0){
					et024006_DrawFilledCircle(55+15+60,195-i*30,15,RED,0xFF);
					board[5-i][2]=1;
					turn=2;
					break;
				}
			}
		}
		if (position == '4'){
			for(int i=0;i<6;i++){
				if(board[0][3]!=0){
					usart_write_line(&AVR32_USART0, "This column is full(Enter another one) \r\n");
				}
				if(board[5-i][3]==0){
					et024006_DrawFilledCircle(55+15+90,195-i*30,15,RED,0xFF);
					board[5-i][3]=1;
					turn=2;
					break;
				}
			}
		}
		if (position == '5'){
			for(int i=0;i<6;i++){
				if(board[0][4]!=0){
					usart_write_line(&AVR32_USART0, "This column is full(Enter another one) \r\n");
				}
				if(board[5-i][4]==0){
					et024006_DrawFilledCircle(55+15+120,195-i*30,15,RED,0xFF);
					board[5-i][4]=1;
					turn=2;
					break;
				}
			}
		}
		if (position == '6'){
			for(int i=0;i<6;i++){
				if(board[0][5]!=0){
					usart_write_line(&AVR32_USART0, "This column is full(Enter another one) \r\n");
				}
				if(board[5-i][5]==0){
					et024006_DrawFilledCircle(55+15+150,195-i*30,15,RED,0xFF);
					board[5-i][5]=1;
					turn=2;
					break;
				}
			}
		}
		if (position == '7'){
			for(int i=0;i<6;i++){
				if(board[0][6]!=0){
					usart_write_line(&AVR32_USART0, "This column is full(Enter another one) \r\n");
				}
				if(board[5-i][6]==0){
					et024006_DrawFilledCircle(55+15+180,195-i*30,15,RED,0xFF);
					board[5-i][6]=1;
					turn=2;
					break;
				}
			}
		}
	}
	else{
		int randomPosition=(rand()%(7));//obtain a random number from 0 to 6
		int randomCorrect=0;
		for(int i=randomPosition;i<7;i++){
			if(board[0][randomPosition]==0){
				for(int j=0;j<6;j++){
					if(board[5-j][randomPosition]==0){
						et024006_DrawFilledCircle(55+15+randomPosition*30,195-j*30,15,BLUE,0xFF);
						board[5-j][randomPosition]=2;
						randomCorrect=1;
						turn=1;
						break;
					}
				}
				break;
			}
		}
		if(randomCorrect==0){
			for(int i=randomPosition;i>=0;i--){
				if(board[0][randomPosition]==0){
					for(int j=0;j<6;j++){
						if(board[5-j][randomPosition]==0){
							et024006_DrawFilledCircle(55+15+randomPosition*30,195-j*30,15,BLUE,0xFF);
							board[5-j][randomPosition]=2;
							randomCorrect=1;
							turn=1;
							break;
						}
					}
					break;
				}
			}
		}
	}
	/*for(int q=0;q<6;q++){
		for(int p=0;p<7;p++){
			usart_putchar(&AVR32_USART0,'0'+board[q][p]);
		}
	}*/
}
