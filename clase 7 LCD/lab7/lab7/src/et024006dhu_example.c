//Lab7 Alfredo Zhu Chen, Miguel Alejandro Cruz banda and Luis Arturo Dan Fong

//Libraries
#include "board.h"
#include "gpio.h"
#include "power_clocks_lib.h"
#include "et024006dhu.h"
#include "delay.h"
#include "conf_clock.h"
#include "pwm.h"
#include "avr32_logo.h"
#include "image.h"
#include "time.h"

//Colors R(5 bits)|G(6 bit)|B(5 bits)
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define PURPLE 0x781F
#define CYAN 0x07FF
#define PINK 0xF81F
#define ORANGE 0xFBE0
#define ET024006_WIDTH 320 
#define ET024006_HEIGHT 240

static void tft_bl_init(void);
void blackScreenAndfadeEffect(void);
void drawChessBoard(int square_size);
void up_down_isr(void);
void leftAndRight_isr(void);
void drawAVR32Logo(void);
void drawColorBarsTest(void);
void randomPixelColors(void);
void showImage(void);
void draw3DPietMondrian(void);
void drawContinuousCircles(void);
void drawRandomColorsChessBoard(void);
void drawimpossibleGeometric(void);
void drawDeepPicture(void);
void drawBasedPietMondrian(void);
void drawFibonacciSpiral(void);
void drawTangram(void);
void drawTriangles(void);
void draw3Dsquares(void);
void drawColorTriangles(void);
int picture;
int changedSize;
int movingSquarePositionX;
int movingSquarePositionY;
// Main function
int main(void){

	// Set CPU and PBA clock
	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

	et024006_Init( FOSC0, FOSC0 );

	
  /* PWM is fed by PBA bus clock which is by default the same
   * as the CPU speed. We set a 0 duty cycle and thus keep the
   * display black*/
	tft_bl_init();
	
	Disable_global_interrupt();
	INTC_init_interrupts();
	INTC_register_interrupt(&up_down_isr,70,3);
	INTC_register_interrupt(&leftAndRight_isr,71,3);
	gpio_enable_pin_interrupt(54,2);
	gpio_enable_pin_interrupt(55,2);
	gpio_enable_pin_interrupt(56,2);
	gpio_enable_pin_interrupt(57,2);
	Enable_global_interrupt();
	
	srand(time(0));//generate random seeds
	
	while(true){
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==0){
			drawAVR32Logo();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==1){
			drawChessBoard(30);
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==2){
			randomPixelColors();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==3){
			drawColorBarsTest();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==4){
			draw3DPietMondrian();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==5){
			drawContinuousCircles();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==6){
			drawimpossibleGeometric();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==7){
			drawDeepPicture();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==8){
			drawBasedPietMondrian();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==9){
			changedSize=1;
			drawChessBoard(changedSize);
			et024006_DrawFilledRect(0 , 0, 40,ET024006_HEIGHT, BLUE);
			et024006_DrawFilledRect(40+changedSize*8,0,ET024006_WIDTH-(40+changedSize*8),ET024006_HEIGHT, BLUE);
			et024006_DrawFilledRect(40,changedSize*8,changedSize*8 ,ET024006_HEIGHT-(changedSize*8), BLUE);

			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==10){
			drawFibonacciSpiral();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==11){
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			movingSquarePositionX=0;
			movingSquarePositionY=0;
			for(int i=0;i<10;i++){
				et024006_DrawVertLine(i,0,10,WHITE);
			}
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==12){
			drawTangram();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==13){
			drawRandomColorsChessBoard();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(picture==14){
			drawRandomColorsChessBoard();
			delay_ms(1000);
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==14){
			showImage();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==15){
			drawTriangles();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==16){
			drawColorTriangles();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) && picture==17){
			draw3Dsquares();
			picture++;
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER)){
			}
		}
		if(picture==18)
			picture=0;
	}
}
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

static void tft_bl_init(void){

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
void blackScreenAndfadeEffect(void){
	/* Lets do a nice fade in by increasing the duty cycle */
	while(pwm_channel6.cdty < pwm_channel6.cprd)
	{
		pwm_channel6.cdty++;
		pwm_channel6.cupd = pwm_channel6.cdty;
		//pwm_channel6.cdty--;
		pwm_async_update_channel(AVR32_PWM_ENA_CHID6, &pwm_channel6);
		delay_ms(10);
	}
}
void drawAVR32Logo(void){
 
	U32 i;
	// Clear the display i.e. make it black
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );

	// Draw the background AVR32 logo.
	et024006_PutPixmap(avr32_logo, 320, 0, 0, 0, 0, 320, 240);

	// Display lines of colored squares.
	for( i=0 ; i<16 ; i++ ){
		// From black to white.
		et024006_DrawFilledRect(20*i,   0, 20, 20, (2*i)/*B:5*/ | ((4*i)<<5)/*G:6*/ | ((2*i)<<11)/*R:5*/ );
		// From black to blue.
		et024006_DrawFilledRect(20*i,  20, 20, 20, (2*i) /*B:5*/);
		// From black to green
		et024006_DrawFilledRect(20*i, 200, 20, 20, ((4*i)<<5) /*G:6*/);
		// From black to red
		et024006_DrawFilledRect(20*i, 220, 20, 20, ((2*i)<<11) /*R:5*/);
	}

	blackScreenAndfadeEffect();
	// Display text.
	et024006_PrintString("AVR UC3", (const unsigned char *)&FONT6x8, 80, 50, BLUE, -1);
	et024006_PrintString("AVR UC3", (const unsigned char *)&FONT8x8, 80, 60, BLACK, -1);

	// Draw a crossed square.
	et024006_DrawHorizLine(10, 50, 20, BLACK);
	et024006_DrawVertLine(10, 50, 20, BLACK);
	et024006_DrawHorizLine(10, 70, 20, BLACK);
	et024006_DrawVertLine(30, 50, 20, BLACK);
	et024006_DrawLine(10, 50, 30, 70, BLACK);
	et024006_DrawLine(30, 50, 10, 70, BLACK);
}


void drawChessBoard(int square_size){
	// Clear the display i.e. make it black
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );	
	
	//Left and Right side
	et024006_DrawFilledRect(0 , 0, 40,ET024006_HEIGHT, BLUE);
	et024006_DrawFilledRect(280, 0, 40,ET024006_HEIGHT, BLUE);
	int i;
	//Draw white squares
	for(i=0;i<4;i++){
			et024006_DrawFilledRect(40 , square_size*2*i, square_size, square_size, WHITE);
			et024006_DrawFilledRect(40+square_size*2 , square_size*2*i, square_size,square_size, WHITE);
			et024006_DrawFilledRect(40+square_size*4 , square_size*2*i, square_size,square_size, WHITE);
			et024006_DrawFilledRect(40+square_size*6 , square_size*2*i, square_size, square_size, WHITE);	
	}
	for(i=0;i<4;i++){
		et024006_DrawFilledRect(40+square_size,square_size+2*i*square_size, square_size, square_size, WHITE);
		et024006_DrawFilledRect(40+square_size*3,square_size+2*i*square_size, square_size, square_size, WHITE);
		et024006_DrawFilledRect(40+square_size*5,square_size+2*i*square_size, square_size, square_size, WHITE);
		et024006_DrawFilledRect(40+square_size*7,square_size+2*i*square_size, square_size, square_size, WHITE);
	}
}

__attribute__ ((__interrupt__))
void up_down_isr(void){
	
		if(gpio_get_pin_interrupt_flag (QT1081_TOUCH_SENSOR_UP)){	//UP
					
			gpio_clear_pin_interrupt_flag(QT1081_TOUCH_SENSOR_UP);
			if (picture==10){
				if(changedSize<16){
					changedSize=changedSize*2;
					drawChessBoard(changedSize);
						et024006_DrawFilledRect(0 , 0, 40,ET024006_HEIGHT, BLUE);
						et024006_DrawFilledRect(40+changedSize*8,0,ET024006_WIDTH-(40+changedSize*8),ET024006_HEIGHT, BLUE);
						et024006_DrawFilledRect(40,changedSize*8,changedSize*8 ,ET024006_HEIGHT-(changedSize*8), BLUE);
					
				}
			}
			if(movingSquarePositionY>0 && picture==12){
				// Clear the display i.e. make it black
				et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			
				movingSquarePositionY--;
				for(int i=0;i<10;i++){
					et024006_DrawVertLine(movingSquarePositionX+i,movingSquarePositionY,10,WHITE);
				}
			}
		}
		if(gpio_get_pin_interrupt_flag (QT1081_TOUCH_SENSOR_DOWN)){	//DOWN
		
			gpio_clear_pin_interrupt_flag(QT1081_TOUCH_SENSOR_DOWN);
			if (picture==10){
				if(changedSize>1){
					changedSize=changedSize/2;
					drawChessBoard(changedSize);
					et024006_DrawFilledRect(0 , 0, 40,ET024006_HEIGHT, BLUE);
					et024006_DrawFilledRect(40+changedSize*8,0,ET024006_WIDTH-(40+changedSize*8),ET024006_HEIGHT, BLUE);
					et024006_DrawFilledRect(40,changedSize*8,changedSize*8 ,ET024006_HEIGHT-(changedSize*8), BLUE);
				}
			}
			if(movingSquarePositionY<(ET024006_HEIGHT-10) && picture==12){
				// Clear the display i.e. make it black
				et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
							
				movingSquarePositionY++;
				for(int i=0;i<10;i++){
					et024006_DrawVertLine(movingSquarePositionX+i,movingSquarePositionY,10,WHITE);
				}
			}
		}
}

__attribute__ ((__interrupt__))
void leftAndRight_isr(void){
	int i;
	if(gpio_get_pin_interrupt_flag (QT1081_TOUCH_SENSOR_LEFT)){	//LEFT
		
		gpio_clear_pin_interrupt_flag(QT1081_TOUCH_SENSOR_LEFT);
		if(movingSquarePositionX!=0 && picture==12){
			// Clear the display i.e. make it black
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			
			movingSquarePositionX--;
			for(i=0;i<10;i++){
				et024006_DrawVertLine(movingSquarePositionX+i,movingSquarePositionY,10,WHITE);
			}
		}
	}
	if(gpio_get_pin_interrupt_flag (QT1081_TOUCH_SENSOR_RIGHT)){	//RIGHT
		
		gpio_clear_pin_interrupt_flag(QT1081_TOUCH_SENSOR_RIGHT);
		if(movingSquarePositionX<(ET024006_WIDTH-10) && picture==12){
			// Clear the display i.e. make it black
			et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
			
			movingSquarePositionX++;
			for(i=0;i<10;i++){
				et024006_DrawVertLine(movingSquarePositionX+i,movingSquarePositionY,10,WHITE);
			}
		}	
	}
}
void randomPixelColors(void){
	int n=0;
	for (int row=0;row<ET024006_WIDTH;row++){
		for (int col=0;col<ET024006_HEIGHT;col++){
			n= rand()%10;
			switch (n){
				case 0:
				et024006_DrawFilledRect(row , col , 1, 1, BLACK);
				break;
				case 1:
				et024006_DrawFilledRect(row , col , 1, 1, BLUE);
				break;
				case 2:
				et024006_DrawFilledRect(row , col , 1, 1, RED);
				break;
				case 3:
				et024006_DrawFilledRect(row , col , 1, 1, GREEN);
				break;
				case 4:
				et024006_DrawFilledRect(row,col, 1, 1, WHITE);
				
				break;
				
				case 5:
				et024006_DrawFilledRect(row ,col , 1, 1, CYAN);
				break;

				case 6:
				et024006_DrawFilledRect(row,col , 1, 1, PURPLE);
				break;
				
				case 7:
				et024006_DrawFilledRect(row,col , 1, 1, PINK);
				break;
				
				case 8:
				et024006_DrawFilledRect(row,col , 1, 1, ORANGE);
				break;
				
				case 9:
				et024006_DrawFilledRect(row,col , 1, 1, YELLOW);
				break;
			}
		}
		
	}
	
}
void drawRandomColorsChessBoard(void){
	
	//Left and Right side
	et024006_DrawFilledRect(0 , 0, 40,ET024006_HEIGHT, (rand()%(0xFFFF)));
	et024006_DrawFilledRect(280, 0, 40,ET024006_HEIGHT,(rand()%(0xFFFF)));
	int i;
	for(i=0;i<8;i++){
		et024006_DrawFilledRect(40 , 30*i, 30, 30,(rand()%(0xFFFF)));
		et024006_DrawFilledRect(40+30*2 , 30*i, 30,30,(rand()%(0xFFFF)));
		et024006_DrawFilledRect(40+30*4 , 30*i, 30,30,(rand()%(0xFFFF)));
		et024006_DrawFilledRect(40+30*6 , 30*i, 30, 30,(rand()%(0xFFFF)));
	}
	for(i=0;i<8;i++){
		et024006_DrawFilledRect(40+30,i*30, 30, 30, (rand()%(0xFFFF)));
		et024006_DrawFilledRect(40+30*3,i*30, 30, 30,(rand()%(0xFFFF)));
		et024006_DrawFilledRect(40+30*5,i*30, 30, 30,(rand()%(0xFFFF)));
		et024006_DrawFilledRect(40+30*7,i*30, 30, 30,(rand()%(0xFFFF)));
	}
}
void drawColorBarsTest(void){
	// Clear the display i.e. make it black
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
	
	et024006_DrawFilledRect(0 , 0, 45, 160, WHITE);
	et024006_DrawFilledRect(46, 0, 45,160, YELLOW);
	et024006_DrawFilledRect(46*2, 0,45,160, CYAN);
	et024006_DrawFilledRect(46*3, 0,45,160, GREEN);
	et024006_DrawFilledRect(46*4, 0,45,160, PINK);
	et024006_DrawFilledRect(46*5, 0,45,160, RED);
	et024006_DrawFilledRect(46*6, 0,44,160, BLUE);
	
	et024006_DrawFilledRect(0 , 160, 45,20, BLUE);
	et024006_DrawFilledRect(46,160, 45,20, PINK);
	et024006_DrawFilledRect(46*2 , 160,45,20, YELLOW);
	et024006_DrawFilledRect(46*3, 160,45,20, RED);
	et024006_DrawFilledRect(46*4, 160,45,20, CYAN);
	et024006_DrawFilledRect(46*5, 160,45,20, BLACK);
	et024006_DrawFilledRect(46*6, 160,44,20, WHITE);

	int i;
	int r=31;
	int b=31;
	int g=62;
	for(i=0 ; i<198; i++ ){
		if(i%6==0 && r!=0)
			r--;
		else if(i%4==0  && g!=0)
			g=g-2;
		else if(i%3==0  && b!=0)
			b--;		
		et024006_DrawFilledRect(i, 180,1, 30, (b)/*B:5*/ | ((g)<<5)/*G:6*/ | ((r)<<11)/*R:5*/);
	}
	
	
	
	for(i=0 ; i<16; i++ ){
		et024006_DrawFilledRect(12*i,210, 12, 30, (2*i)/*B:5*/ | ((4*i)<<5)/*G:6*/ | ((2*i)<<11)/*R:5*/ );
	}
	
	//Red to yellow
	for(i=0 ; i<32; i++ ){
		et024006_DrawFilledRect(32*6+i,180,i, 30, (0)/*B:5*/ | ((2*i)<<5)/*G:6*/ | ((31)<<11)/*R:5*/ );
	}
	//yellow to green
	for(i=0 ; i<16; i++ ){
		et024006_DrawFilledRect(32*7+i,180,i, 30, (0)/*B:5*/ | ((63)<<5)/*G:6*/ | ((31-i*2)<<11)/*R:5*/ );
	}
	//green to cyan
	for(i=0 ; i<32; i++ ){
		et024006_DrawFilledRect(32*7+16+i,180,i, 30, (i)/*B:5*/ | ((61)<<5)/*G:6*/ | ((0)<<11)/*R:5*/ );
	}
	//cyan to blue
	for(i=0 ; i<16; i++ ){
		et024006_DrawFilledRect(32*8+i,180,i, 30, (31)/*B:5*/ | ((62-i*2)<<5)/*G:6*/ | ((0)<<11)/*R:5*/ );
	}
	//blue to pink
	for(i=0 ; i<32; i++ ){
		et024006_DrawFilledRect(32*8+16+i,180,i, 30, (31)/*B:5*/ | ((0)<<5)/*G:6*/ | ((i)<<11)/*R:5*/ );
	}
	
	
	blackScreenAndfadeEffect();
}
void drawBasedPietMondrian(void){
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
	et024006_DrawFilledRect(0,0,45,40,YELLOW);
	et024006_DrawFilledRect(0,45,45,160,WHITE);
	et024006_DrawFilledRect(0,210,45,30,YELLOW);
	et024006_DrawFilledRect(50,0,110,40,RED);
	et024006_DrawFilledRect(50,45,110,65,BLUE);
	et024006_DrawFilledRect(50,115,110,25,WHITE);
	et024006_DrawFilledRect(50,145,110,60,RED);
	et024006_DrawFilledRect(50,210,180,30,BLACK);
	et024006_DrawFilledRect(165,0,40,40,WHITE);
	et024006_DrawFilledRect(165,45,40,65,BLACK);
	et024006_DrawFilledRect(165,115,110,45,YELLOW);
	et024006_DrawFilledRect(165,165,65,40,BLUE);
	et024006_DrawFilledRect(210,0,110,20,WHITE);
	et024006_DrawFilledRect(210,25,110,85,YELLOW);
	et024006_DrawFilledRect(280,115,40,45,RED);
	et024006_DrawFilledRect(235,165,85,40,WHITE);
	et024006_DrawFilledRect(235,210,85,30,BLUE);
	et024006_DrawFilledRect(45,0,5,240,BLACK);
	et024006_DrawFilledRect(160,0,5,210,BLACK);
	et024006_DrawFilledRect(205,0,5,115,BLACK);
	et024006_DrawFilledRect(230,160,5,80,BLACK);
	et024006_DrawFilledRect(275,110,5,55,BLACK);
	et024006_DrawFilledRect(0,40,210,5,BLACK);
	et024006_DrawFilledRect(210,20,110,5,BLACK);
	et024006_DrawFilledRect(45,110,275,5,BLACK);
	et024006_DrawFilledRect(45,140,120,5,BLACK);
	et024006_DrawFilledRect(160,160,160,5,BLACK);
	et024006_DrawFilledRect(0,205,320,5,BLACK);
}
void drawContinuousCircles(void){
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
	int i;
	//Orange to yellow
	for(i=0;i<33;i++){
		et024006_DrawFilledCircle(ET024006_WIDTH/2,ET024006_HEIGHT/2,200-i,(0)/*B:5*/ | ((31+i)<<5)/*G:6*/ | ((31)<<11)/*R:5*/,0xFF);
	}
	//Yellow to green
	for(i=0;i<32;i++){
		et024006_DrawFilledCircle(ET024006_WIDTH/2,ET024006_HEIGHT/2,167-i,(0)/*B:5*/ | ((63)<<5)/*G:6*/ | ((31-i)<<11)/*R:5*/,0xFF);
	}
	//Green to cyan
	for(i=0;i<32;i++){
		et024006_DrawFilledCircle(ET024006_WIDTH/2,ET024006_HEIGHT/2,135-i,(i)/*B:5*/ | ((63)<<5)/*G:6*/ | ((0)<<11)/*R:5*/,0xFF);
	}
	//Cyan to blue
	for(i=0;i<16;i++){
		et024006_DrawFilledCircle(ET024006_WIDTH/2,ET024006_HEIGHT/2,102-i,(31)/*B:5*/ | ((63-i)<<5)/*G:6*/ | ((0)<<11)/*R:5*/,0xFF);
	}
	//Cyan to blue
	for(i=0;i<16;i++){
		et024006_DrawFilledCircle(ET024006_WIDTH/2,ET024006_HEIGHT/2,86-i,(31)/*B:5*/ | ((47-3*i)<<5)/*G:6*/ | ((0)<<11)/*R:5*/,0xFF);
	}
	//Blue to pink
	for(i=0;i<32;i++){
		et024006_DrawFilledCircle(ET024006_WIDTH/2,ET024006_HEIGHT/2,70-i,(31)/*B:5*/ | ((0)<<5)/*G:6*/ | ((i)<<11)/*R:5*/,0xFF);
	}
	//Pink to red
	for(i=0;i<32;i++){
		et024006_DrawFilledCircle(ET024006_WIDTH/2,ET024006_HEIGHT/2,38-i,(31-i)/*B:5*/ | ((0)<<5)/*G:6*/ | ((31)<<11)/*R:5*/,0xFF);
	}
}
void showImage(void){
	et024006_PutPixmap(image, 320, 0, 0, 0, 0, 320, 240);
}
void drawDeepPicture(void){
	//Limpiar LCD, se pone un fondo blanco para la imagen
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);

	//Figura
	et024006_DrawLine(170,40,170,120,BLACK);
	et024006_DrawLine(120,90,120,140,BLACK);
	et024006_DrawLine(220,110,220,140,BLACK);
	et024006_DrawLine(120,90,170,40,BLACK);
	et024006_DrawLine(170,40,220,110,BLACK);
	et024006_DrawLine(170,40,170,120,BLACK);
	et024006_DrawLine(120,140,170,120,BLACK);
	et024006_DrawLine(170,120,220,140,BLACK);
	et024006_DrawLine(120,140,180,150,BLACK);
	et024006_DrawLine(180,150,220,140,BLACK);

	//Linea roja
	et024006_DrawLine(30,180,300,180,RED);

	//Lineas de punto de fuga
	et024006_DrawLine(60,180,120,90,BLUE);
	et024006_DrawLine(60,180,120,140,BLUE);
	et024006_DrawLine(60,180,180,120,BLUE);
	et024006_DrawLine(60,180,180,150,BLUE);
	et024006_DrawLine(170,150,290,180,BLUE);
	et024006_DrawLine(220,140,290,180,BLUE);
	et024006_DrawLine(220,110,290,180,BLUE);

	//Lineas punteadas
	et024006_DrawLine(120,90,180,120,BLACK);
	et024006_DrawLine(130,90,122,98,WHITE);
	et024006_DrawLine(130,91,122,99,WHITE);
	et024006_DrawLine(130,92,122,100,WHITE);

	et024006_DrawLine(140,90,130,100,WHITE);
	et024006_DrawLine(140,91,130,101,WHITE);
	et024006_DrawLine(140,92,130,102,WHITE);

	et024006_DrawLine(130,110,150,90,WHITE);
	et024006_DrawLine(130,111,150,91,WHITE);
	et024006_DrawLine(130,112,150,92,WHITE);

	et024006_DrawLine(150,110,160,100,WHITE);
	et024006_DrawLine(150,111,160,101,WHITE);
	et024006_DrawLine(150,112,160,102,WHITE);

	et024006_DrawLine(150,120,167,102,WHITE);
	et024006_DrawLine(150,121,167,103,WHITE);
	et024006_DrawLine(150,122,167,104,WHITE);

	et024006_DrawLine(160,118,169,111,WHITE);
	et024006_DrawLine(160,119,169,112,WHITE);
	et024006_DrawLine(160,120,169,113,WHITE);

	et024006_DrawLine(172,118,178,112,WHITE);
	et024006_DrawLine(172,119,178,113,WHITE);
	et024006_DrawLine(172,120,178,114,WHITE);

	et024006_DrawLine(180,150,180,120,BLACK);
	et024006_DrawLine(175,130,185,130,WHITE);
	et024006_DrawLine(175,131,185,131,WHITE);
	et024006_DrawLine(175,132,185,132,WHITE);
	et024006_DrawLine(175,133,185,133,WHITE);
	et024006_DrawLine(175,134,185,134,WHITE);

	et024006_DrawLine(175,140,185,140,WHITE);
	et024006_DrawLine(175,141,185,141,WHITE);
	et024006_DrawLine(175,142,185,142,WHITE);
	et024006_DrawLine(175,143,185,143,WHITE);
	et024006_DrawLine(175,144,185,144,WHITE);

	et024006_DrawLine(220,110,180,120,BLACK);
	et024006_DrawLine(180,110,195,125,WHITE);
	et024006_DrawLine(180,111,195,126,WHITE);
	et024006_DrawLine(180,112,195,127,WHITE);

	et024006_DrawLine(190,110,200,120,WHITE);
	et024006_DrawLine(190,111,200,121,WHITE);
	et024006_DrawLine(190,112,200,122,WHITE);

	et024006_DrawLine(200,110,210,120,WHITE);
	et024006_DrawLine(200,111,210,121,WHITE);
	et024006_DrawLine(200,112,210,122,WHITE);

	et024006_DrawLine(210,110,215,118,WHITE);
	et024006_DrawLine(210,111,215,119,WHITE);
	et024006_DrawLine(210,112,215,120,WHITE);
	//Puntos
	et024006_DrawFilledCircle(60,180,4,BLACK,0xFF);
	et024006_DrawFilledCircle(290,180,4,BLACK,0xFF);
}
void drawFibonacciSpiral(void){
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );

	et024006_DrawFilledRect(30,40,144,144,WHITE);//1
	et024006_DrawFilledCircle(174,184,144,BLUE,0x0c);//0x03 cuadrante 0, 0x0C cuad 1, 0x30 cuad 2,0xC0 cuad 3.
	et024006_DrawFilledCircle(174,184,142,WHITE,0x0c);

	et024006_DrawFilledRect(174,40,89,89,WHITE);//2
	et024006_DrawFilledCircle(174,129,89,BLUE,0x03);
	et024006_DrawFilledCircle(174,129,87,WHITE,0x03);

	et024006_DrawFilledRect(208,129,55,55,WHITE);//3
	et024006_DrawFilledCircle(208,129,55,BLUE,0xc0);
	et024006_DrawFilledCircle(208,129,53,WHITE,0xc0);

	et024006_DrawFilledRect(174,150,34,34,WHITE);//4
	et024006_DrawFilledCircle(208,150,34,BLUE,0x30);
	et024006_DrawFilledCircle(208,150,32,WHITE,0x30);

	et024006_DrawFilledRect(174,129,21,21,WHITE);//5
	et024006_DrawFilledCircle(195,150,21,BLUE,0x0c);
	et024006_DrawFilledCircle(195,150,19,WHITE,0x0c);

	et024006_DrawFilledRect(195,129,13,13,WHITE);//6
	et024006_DrawFilledCircle(195,142,13,BLUE,0x03);
	et024006_DrawFilledCircle(195,142,11,WHITE,0x03);

	et024006_DrawFilledRect(200,142,8,8,WHITE);//7
	et024006_DrawFilledCircle(200,142,8,BLUE,0xc0);
	et024006_DrawFilledCircle(200,142,6,WHITE,0xc0);

	et024006_DrawFilledRect(195,142,5,5,WHITE);//8
	et024006_DrawFilledCircle(200,142,5,BLUE,0x30);
	et024006_DrawFilledCircle(200,142,3,WHITE,0x30);

	et024006_DrawFilledRect(195,142,3,3,WHITE);//9
	et024006_DrawFilledCircle(198,145,3,BLUE,0x0c);
	et024006_DrawFilledCircle(198,145,1,WHITE,0x0c);

	et024006_DrawVertLine(195,129,21,BLACK);
	et024006_DrawVertLine(174,40,144,BLACK);
	et024006_DrawVertLine(208,129,55,BLACK);
	et024006_DrawHorizLine(174,129,89,BLACK);
	et024006_DrawHorizLine(174,150,34,BLACK);
	et024006_DrawHorizLine(195,142,13,BLACK);
	et024006_DrawVertLine(200,142,8,BLACK);
}
void drawTriangles(void){
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);
	int i;
	for(i=0;i<20;i++){
		et024006_DrawLine(160,0+i*3,80+4*i,120-i*3,BLACK);	
	}
	for(i=0;i<20;i++){
		et024006_DrawLine(160,0+i*3,240-4*i,120-i*3,BLACK);
	}
	for(i=0;i<20;i++){
		et024006_DrawLine(80+i*4,120-i*3,240-4*i,120-i*3,BLACK);
	}
	
	
	
	for(i=0;i<20;i++){
		et024006_DrawLine(80,120+i*3,0+4*i,240-i*3,BLACK);
	}
	for(i=0;i<20;i++){
		et024006_DrawLine(80,120+i*3,160-4*i,240-i*3,BLACK);
	}
	for(i=0;i<20;i++){
		et024006_DrawLine(0+i*4,240-i*3,160-4*i,240-i*3,BLACK);
	}
		
	for(i=0;i<20;i++){
		et024006_DrawLine(240,120+i*3,160+4*i,240-i*3,BLACK);
	}
	for(i=0;i<20;i++){
		et024006_DrawLine(240,120+i*3,320-4*i,240-i*3,BLACK);
	}
	for(i=0;i<20;i++){
		et024006_DrawLine(160+i*4,240-i*3,320-4*i,240-i*3,BLACK);
	}
	
	for(i=0;i<10;i++){
		et024006_DrawLine(240-i*8,120+i*6,160,240-i*6,BLACK);
	}
	for(i=0;i<10;i++){
		et024006_DrawLine(80+i*8,120+i*6,160,240-i*6,BLACK);
	}
	for(i=0;i<10;i++){
		et024006_DrawLine(80+i*8,120+i*6,240-8*i,120+i*6,BLACK);
	}
	
	
}

void drawimpossibleGeometric(void){
	//Limpiar LCD, se pone un fondo blanco para la imagen
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);

	
	//figura 1
	et024006_DrawLine(20,70,20,140,BLACK);
	et024006_DrawLine(20,140,40,160,BLACK);
	et024006_DrawLine(40,160,110,160,BLACK);
	et024006_DrawLine(110,160,130,140,BLACK);
	et024006_DrawLine(130,70,110,50,BLACK);
	et024006_DrawLine(110,50,40,50,BLACK);
	et024006_DrawLine(40,50,20,70,BLACK);
	et024006_DrawLine(20,70,90,70,BLACK);
	et024006_DrawLine(40,90,40,160,BLACK);
	et024006_DrawLine(60,140,130,140,BLACK);
	et024006_DrawLine(110,120,110,50,BLACK);
	et024006_DrawLine(40,90,90,90,BLACK);
	et024006_DrawLine(60,90,60,140,BLACK);
	et024006_DrawLine(60,120,110,120,BLACK);
	et024006_DrawLine(90,120,90,70,BLACK);
	et024006_DrawLine(130,70,130,140,BLACK);

	//figura 2
	et024006_DrawLine(170,50,170,150,BLACK);
	et024006_DrawLine(190,70,190,150,BLACK);
	et024006_DrawLine(210,90,210,150,BLACK);
	et024006_DrawLine(230,70,230,150,BLACK);
	et024006_DrawLine(250,50,250,150,BLACK);
	et024006_DrawLine(270,60,270,150,BLACK);
	et024006_DrawLine(170,50,250,50,BLACK);
	et024006_DrawLine(190,70,230,70,BLACK);
	et024006_DrawLine(210,90,230,90,BLACK);
	et024006_DrawLine(250,50,270,60,BLACK);
	et024006_DrawLine(190,70,210,90,BLACK);
	et024006_DrawFilledCircle(180,150,10,BLACK,0xFF);
	et024006_DrawFilledCircle(180,150,9,WHITE,0xFF);
	et024006_DrawFilledCircle(220,150,10,BLACK,0xFF);
	et024006_DrawFilledCircle(220,150,9,WHITE,0xFF);
	et024006_DrawFilledCircle(260,150,10,BLACK,0xFF);
	et024006_DrawFilledCircle(260,150,9,WHITE,0xFF);



}
void draw3DPietMondrian(void){
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);
	//Lineas verticales

	et024006_DrawVertLine(170,10,200,BLACK);
	et024006_DrawVertLine(190,20,200,BLACK);
	et024006_DrawVertLine(120,40,140,BLACK);
	
	//Lineas diagonales
	et024006_DrawLine(120,40,170,10,BLACK);
	et024006_DrawLine(120,60,190,20,BLACK);
	et024006_DrawLine(170,80,240,40,BLACK);
	et024006_DrawLine(120,180,190,140,BLACK);
	et024006_DrawLine(170,210,240,170,BLACK);
	et024006_DrawLine(190,220,240,190,BLACK);
	et024006_DrawLine(120,40,190,70,BLACK);
	et024006_DrawLine(120,60,170,80,BLACK);
	et024006_DrawLine(120,180,190,220,BLACK);
	et024006_DrawLine(170,150,240,190,BLACK);
	et024006_DrawLine(190,140,240,170,BLACK);
	et024006_DrawLine(170,10,240,43,BLACK);
	et024006_DrawLine(240,40,240,190,BLACK);
	
	int i;
	for(i=0;i<130;i++){
		et024006_DrawLine(170+1, 80-1+i, 190-1, 70-1+i, RED);
	}
	//Blue triangle
	for(i=0;i<49;i++){
		et024006_DrawVertLine(190+1+i,21+i/2,49-1-i,BLUE);
	}
	for(i=0;i<20;i++){
		et024006_DrawVertLine(120+1+i,40+i/2,20-1-i,YELLOW);
	}
	for(i=0;i<20;i++){
		et024006_DrawLine(191,200+i,223,180+i,BLUE);
	}
	for(i=0;i<16;i++){
		et024006_DrawVertLine(223+1+i,182+i/2,19-1-i,BLUE);
	}
	for(i=0;i<19;i++){
		et024006_DrawLine(191,144+i,223,160+i,YELLOW);
	}
	for(i=0;i<16;i++){
		et024006_DrawVertLine(223+1+i,163+i/2,18-1-i,YELLOW);
	}
}
void drawTangram(void){

//Limpiar LCD, se pone un fondo blanco para la imagen
et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);

//Cuadrado
et024006_DrawFilledRect(-90+170, 60, 20, 20, YELLOW);

//Triangulos
int i;
for(i=0;i<39;i++){
	et024006_DrawHorizLine(-90+170+1+i/2,59-i/2,39-1-i,RED);
}
for(i=0;i<69;i++){
	et024006_DrawHorizLine(-90+90+1+i/2,109-i/2,69-1-i,GREEN);
	et024006_DrawHorizLine(-90+126+1+i/2,75+i/2,69-1-i,ORANGE);
}
for(i=0;i<29;i++){
	et024006_DrawHorizLine(-90+102+1+i,139-i,29-1-i,BLUE);
	et024006_DrawHorizLine(-90+130+1,111+i,29-1-i,BLUE);
	et024006_DrawHorizLine(-90+100+1+i,141+i,29-1-i,PINK);
	et024006_DrawHorizLine(-90+110+1,179-i,29-1-i,CYAN);
}

//Cow
//Cuadrado
et024006_DrawFilledRect(80+40, 70, 20, 20, YELLOW);

//Triangulos

for(i=0;i<54;i++){
	et024006_DrawHorizLine(80+130+1,149-i,54-1-i,RED);
}
for(i=0;i<69;i++){
	et024006_DrawHorizLine(80+60+1+i,91+i,69-1-i,ORANGE);
	et024006_DrawHorizLine(80+128+1+i,91+i,69-1-i,GREEN);
}
for(i=0;i<29;i++){
	et024006_DrawHorizLine(80+60+1+i,89-i,29-1-i,CYAN);
}
for(i=0;i<28;i++){
	et024006_DrawVertLine(80+60+1+i/2,61+i/2,28-1-i,PINK);
}
for(i=0;i<33;i++){
	et024006_DrawHorizLine(80+197+1,120-i,33-1-i,BLUE);
	et024006_DrawHorizLine(80+197+1+i,120+i,33-1-i,BLUE);
}
}
void draw3Dsquares(void){
	et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);
	et024006_DrawFilledRect(0,20,40,40,BLACK);//1
	et024006_DrawFilledRect(0,180,40,40,BLACK);//2
	et024006_DrawFilledRect(40,60,40,40,BLACK);//3
	et024006_DrawFilledRect(20,120,40,40,BLACK);//4
	et024006_DrawFilledRect(60,160,40,40,BLACK);//5
	et024006_DrawFilledRect(80,100,40,40,BLACK);//6
	et024006_DrawFilledRect(100,40,40,40,BLACK);//7
	et024006_DrawFilledRect(100,200,40,40,BLACK);//8
	et024006_DrawFilledRect(120,140,40,40,BLACK);//9
	et024006_DrawFilledRect(140,80,40,40,BLACK);//10
	et024006_DrawFilledRect(160,20,40,40,BLACK);//11
	et024006_DrawFilledRect(160,180,40,40,BLACK);//12
	et024006_DrawFilledRect(180,120,40,40,BLACK);//13
	et024006_DrawFilledRect(200,60,40,40,BLACK);//14
	et024006_DrawFilledRect(220,160,40,40,BLACK);//15
	et024006_DrawFilledRect(240,100,40,40,BLACK);//16
	et024006_DrawFilledRect(260,40,40,40,BLACK);//17
	et024006_DrawFilledRect(260,200,40,40,BLACK);//18

	et024006_DrawLine(0,80,20,80,BLACK);
	et024006_DrawLine(0,120,20,120,BLACK);
	et024006_DrawLine(0,140,20,120,BLACK);
	et024006_DrawLine(20,240,40,220,BLACK);
	et024006_DrawLine(40,240,40,220,BLACK);
	et024006_DrawLine(40,220,80,220,BLACK);
	et024006_DrawLine(80,220,80,240,BLACK);
	et024006_DrawLine(80,220,100,200,BLACK);
	et024006_DrawLine(100,0,100,40,BLACK);
	et024006_DrawLine(120,0,120,20,BLACK);
	et024006_DrawLine(180,240,200,220,BLACK);
	et024006_DrawLine(200,220,200,240,BLACK);
	et024006_DrawLine(200,240,240,240,BLACK);
	et024006_DrawLine(240,240,240,220,BLACK);
	et024006_DrawLine(240,220,260,200,BLACK);
	et024006_DrawLine(260,0,260,40,BLACK);
	et024006_DrawLine(280,0,280,20,BLACK);
	et024006_DrawLine(300,80,320,80,BLACK);
	et024006_DrawLine(300,120,320,120,BLACK);
	et024006_DrawLine(280,140,320,140,BLACK);
	et024006_DrawLine(0,20,20,0,BLACK);
	et024006_DrawLine(60,40,100,40,BLACK);
	et024006_DrawLine(0,180,20,160,BLACK);
	et024006_DrawLine(100,40,120,20,BLACK);
	et024006_DrawLine(120,20,160,20,BLACK);
	et024006_DrawLine(160,20,180,0,BLACK);
	et024006_DrawLine(220,40,260,40,BLACK);
	et024006_DrawLine(260,40,280,20,BLACK);
	et024006_DrawLine(280,20,320,20,BLACK);
	et024006_DrawLine(280,180,320,180,BLACK);
	et024006_DrawLine(160,0,160,20,BLACK);
	et024006_DrawLine(200,220,240,220,BLACK);

	int i;
	for (i=0;i<40;i++)//A
	{
		et024006_DrawLine(40,i+20,60,i,BLUE);
	}
	for (i=0;i<40;i++)//B
	{
		et024006_DrawLine(20,i+80,40,i+60,BLUE);
	}
	for (i=0;i<40;i++)//C
	{
		et024006_DrawLine(60,i+120,80,i+100,BLUE);
	}
	for (i=0;i<40;i++)//D
	{
		et024006_DrawLine(40,i+180,60,i+160,BLUE);
	}
	for (i=0;i<40;i++)//E
	{
		et024006_DrawLine(80,i+60,100,i+40,BLUE);
	}
	for (i=0;i<40;i++)//F
	{
		et024006_DrawLine(100,i+160,120,i+140,BLUE);
	}
	for (i=0;i<40;i++)//G
	{
		et024006_DrawLine(120,i+100,140,i+80,BLUE);
	}
	for (i=0;i<40;i++)//H
	{
		et024006_DrawLine(140,i+40,160,i+20,BLUE);
	}
	for (i=0;i<40;i++)//I
	{
		et024006_DrawLine(140,i+200,160,i+180,BLUE);
	}
	for (i=0;i<40;i++)//J
	{
		et024006_DrawLine(160,i+140,180,i+120,BLUE);
	}
	for (i=0;i<40;i++)//K
	{
		et024006_DrawLine(180,i+80,200,i+60,BLUE);
	}
	for (i=0;i<40;i++)//Mc
	{
		et024006_DrawLine(200,i+20,220,i,BLUE);
	}
	for (i=0;i<40;i++)//Lc
	{
		et024006_DrawLine(200,i+180,220,i+160,BLUE);
	}
	for (i=0;i<40;i++)//Nc
	{
		et024006_DrawLine(220,i+120,240,i+100,BLUE);
	}
	for (i=0;i<40;i++)//Ñc
	{
		et024006_DrawLine(240,i+60,260,i+40,BLUE);
	}
	for (i=0;i<40;i++)//Oc
	{
		et024006_DrawLine(260,i+160,280,i+140,BLUE);
	}
	for (i=0;i<40;i++)//Pc
	{
		et024006_DrawLine(280,i+100,300,i+80,BLUE);
	}
	for (i=0;i<40;i++)//Qc
	{
		et024006_DrawLine(300,i+40,320,i+20,BLUE);
	}
	for (i=0;i<40;i++)//Rc
	{
		et024006_DrawLine(300,i+200,320,i+180,BLUE);
	}
}
void drawColorTriangles(void){
et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);
	int	i,j,k,s,color;
	for(s=0;s<2;s++){
		for(i=0;i<6;i++){
			for(j=0;j<8;j++){
				color=(rand()%(0xFFFF));
				for(k=0;k<20;k++){
					et024006_DrawVertLine(0+k+j*40+s*20,0+k+40*i+20*s,20-k,color);
				}
			}
		}
	}
	for(s=0;s<2;s++){
		for(i=0;i<6;i++){
			for(j=0;j<8;j++){
				color=(rand()%(0xFFFF));
				for(k=0;k<20;k++){
					et024006_DrawVertLine(40-k+j*40-20*s,0+k+40*i+20*s,20-k,color);
				}
			}
		}
	}
	for(s=0;s<2;s++){
		for(i=0;i<6;i++){
			for(j=0;j<8;j++){
				color=(rand()%(0xFFFF));
				for(k=0;k<20;k++){
					et024006_DrawVertLine(20+k+j*40-20*s,40*i+20*s,20-k,color);
				}
			}
		}
	}
	for(s=0;s<2;s++){
		for(i=0;i<6;i++){
			for(j=0;j<8;j++){
				color=(rand()%(0xFFFF));
				for(k=0;k<20;k++){
					et024006_DrawVertLine(20-k+j*40+20*s,40*i+20*s,20-k,color);
				}
			}
		}
	}
}