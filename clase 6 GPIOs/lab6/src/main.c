#include <asf.h>

//*************************************************************************************************
//Actividad 1

void DecimalToBinary (int binary[4], int numero);//Convertidor decimal a binario

int main(void){
	while(1){
		int numeros [4] = {0 ,0 ,0 ,0}; //Arreglo de los 4 números.
		int binary [4] = {0 ,0 ,0 ,0};//Arreglo de cuatro bits
		
		for (int i = 0; i < 4; i++){
			while (gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) == false){ //Selección de número
				DecimalToBinary(binary,numeros[i]);//Convertir el valor decimal a binario
				
				//Mostrar los  4 bits en LED3|LED2|LED1|LED0
				if (binary[0] == 0){
					gpio_set_gpio_pin(LED0_GPIO); 
				}
				else{
					gpio_clr_gpio_pin(LED0_GPIO);
				}
				if (binary[1] == 0){
					gpio_set_gpio_pin(LED1_GPIO);
				}
				else{
					gpio_clr_gpio_pin(LED1_GPIO);
				}
				if (binary[2] == 0){
					gpio_set_gpio_pin(LED2_GPIO);
				}
				else{
					gpio_clr_gpio_pin(LED2_GPIO);
				}
				if (binary[3] == 0){
					gpio_set_gpio_pin(LED3_GPIO);
				}
				else{
					gpio_clr_gpio_pin(LED3_GPIO);
				}
				
				if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_RIGHT) == true) {//Incrementar si se presionó tecla "RIGHT"
					numeros[i]++;
					if(numeros[i] >= 15){//Limitar el valor máximo a 15
						numeros[i] = 15;
					}
					while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_RIGHT) == true){//Esperar a que se suelte la tecla "RIGHT"
					}		
				}
				if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_LEFT) == true) {//Decrementar si se presionó tecla "LEFT"
					if(numeros[i] == 0){//Limitar el valor mínimo a 0
						numeros[i] = 0;
					}
					else{
						numeros[i]--;
					}
					while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_LEFT) == true){//Esperar a que se suelte tecla "LEFT"
					}
				}
			}
			while (gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) == true){//Esperar a que se suelte tecla "ENTER"
			}
		}
		
		while(1){
			if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP) == true){//Operación AND
				int variableAND = (numeros[0]&numeros[1]&numeros[2]&numeros[3]);
				DecimalToBinary(binary,variableAND);//Convertir el resultado a binario
				//Mostrar los  4 bits en LED3|LED2|LED1|LED0
				if (binary[0] == 0){
					gpio_set_gpio_pin(LED0_GPIO);
				}
				else{
					gpio_clr_gpio_pin(LED0_GPIO);
				}
				if (binary[1] == 0){
					gpio_set_gpio_pin(LED1_GPIO);
				}
				else{
					gpio_clr_gpio_pin(LED1_GPIO);
				}
				if (binary[2] == 0){
					gpio_set_gpio_pin(LED2_GPIO);
				}
				else{
					gpio_clr_gpio_pin(LED2_GPIO);
				}
				if (binary[3] == 0){
					gpio_set_gpio_pin(LED3_GPIO);
				}
				else{
					gpio_clr_gpio_pin(LED3_GPIO);
				}
				break;
			}
			else if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN) == true){//Operación OR 
				int variableOR = (numeros[0]|numeros[1]|numeros[2]|numeros[3]);//Convertir el resultado a binario
				//Mostrar los  4 bits en LED3|LED2|LED1|LED0
				DecimalToBinary(binary,variableOR);
					if (binary[0] == 0){
						gpio_set_gpio_pin(LED0_GPIO);
					}
					else{
						gpio_clr_gpio_pin(LED0_GPIO);
					}
					if (binary[1] == 0){
						gpio_set_gpio_pin(LED1_GPIO);
					}
					else{
						gpio_clr_gpio_pin(LED1_GPIO);
					}
					if (binary[2] == 0){
						gpio_set_gpio_pin(LED2_GPIO);
					}
					else{
						gpio_clr_gpio_pin(LED2_GPIO);
					}
					if (binary[3] == 0){
						gpio_set_gpio_pin(LED3_GPIO);
					}
					else{
						gpio_clr_gpio_pin(LED3_GPIO);
					}
				break;
			}
		}
		while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) == false){	//Esperar a que se suelte tecla "ENTER" y volver a empezar
		}
	}
}

//Función para convertir decimal a binario
void DecimalToBinary (int binary[4], int numero){
	binary[0] = 0;
	binary[1] = 0;
	binary[2] = 0;
	binary[3] = 0;
	int j = 0;
	while (numero>0){
		binary [3-j] = numero%2;
		numero = numero/2;
		j++;
	}
}
//****************************************************************************************************
//Actividad 2
/*void DecimalToBinary (int binary[3], int numero);

int main(void){
	int numero = 0; ////Definición del arreglo de números.
	int binary [3] = {0 ,0 ,0};
	while(1){
		DecimalToBinary(binary,numero);
		if (binary[0] == 0){
			gpio_set_gpio_pin(LED0_GPIO);
		}
		else{
			gpio_clr_gpio_pin(LED0_GPIO);
		}
		if (binary[1] == 0){
			gpio_set_gpio_pin(LED1_GPIO);
		}
		else{
			gpio_clr_gpio_pin(LED1_GPIO);
		}
		if (binary[2] == 0){
			gpio_set_gpio_pin(LED2_GPIO);
		}
		else{
			gpio_clr_gpio_pin(LED2_GPIO);
		}
		if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP) == true) {
			gpio_clr_gpio_pin(LED3_GPIO);
			numero++;
			if(numero >= 7){
				numero = 7;
			}
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP) == true){
			}
			
		}
		if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN) == true) {
			gpio_set_gpio_pin(LED3_GPIO);
			if(numero== 0){
				numero = 0;
			}
			else{
				numero--;
			}
			while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN) == true){
			}
		}
	}
}
//Función para convertir decimal a binario
void DecimalToBinary (int binary[3], int numero_dec){
	binary[0] = 0;
	binary[1] = 0;
	binary[2] = 0;
	int j = 0;
	while (numero_dec>0){
		binary [2-j] = numero_dec%2;
		numero_dec = numero_dec/2;
		j++;
	}
}*/
//****************************************************************************************************
//Actividad 3
/*void DecimalToBinary (int binary[3], int numero);

int main(void)
{
	int binary [3] = {0 ,0 ,0};
	int nuevoDato=0;
	int enter=0;
	while(1){
		if(enter==1){
			binary[0]=0;
			binary[1]=0;
			binary[2]=0;
		}
		enter=0;
		if (binary[0] == 0){
			gpio_set_gpio_pin(LED0_GPIO);
		}
		else{
			gpio_clr_gpio_pin(LED0_GPIO);
		}
		if (binary[1] == 0){
			gpio_set_gpio_pin(LED1_GPIO);
		}
		else{
			gpio_clr_gpio_pin(LED1_GPIO);
		}
		if (binary[2] == 0){
			gpio_set_gpio_pin(LED2_GPIO);
		}
		else{
			gpio_clr_gpio_pin(LED2_GPIO);
		}
		while(true){
			if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) == true) {
				enter=1;
				break;
			}
			if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP) == true) {
				nuevoDato=1;
				gpio_clr_gpio_pin(LED3_GPIO);
				while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP) == true){
				}
				break;
			
			}
			if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN) == true) {
				nuevoDato=0;
				gpio_set_gpio_pin(LED3_GPIO);
				while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN) == true){
				}
				break;
			}
				
		}
		while(enter==0){
			if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_ENTER) == true) {
				enter=1;
				break;
			}

			if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN) == true) {
				nuevoDato=0;
				gpio_set_gpio_pin(LED3_GPIO);
				while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_DOWN) == true){
				}
			}
			if (gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP) == true) {
				nuevoDato=0;
				gpio_set_gpio_pin(LED3_GPIO);
				while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_UP) == true){
				}
			}								
					
			if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_LEFT)){
				binary[0]=binary[1];
				binary[1]=binary[2];
				binary[2]=nuevoDato;
				while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_LEFT)==true){
				}
				break;				
			}
			if(gpio_get_pin_value(QT1081_TOUCH_SENSOR_RIGHT)){
				binary[2]=binary[1];
				binary[1]=binary[0];
				binary[0]=nuevoDato;
				while(gpio_get_pin_value(QT1081_TOUCH_SENSOR_RIGHT)==true){
				}
				break;		
			}
		}
	}	
}*/
//****************************************************************************************************
//Actividad 4
/*int main(void){
	int contador=0;
	int direcc=1;
	while(1){
	
		switch(contador){
			case 0:
			gpio_clr_gpio_pin(LED0_GPIO);
			gpio_set_gpio_pin(LED1_GPIO);
			gpio_set_gpio_pin(LED2_GPIO);
			gpio_set_gpio_pin(LED3_GPIO);
			gpio_clr_gpio_pin(62);
			gpio_clr_gpio_pin(63);
			direcc=1;
			delay_ms(100);
			break;
						
			case 1:
			gpio_set_gpio_pin(LED0_GPIO);
			gpio_clr_gpio_pin(LED1_GPIO);
			gpio_set_gpio_pin(LED2_GPIO);
			gpio_set_gpio_pin(LED3_GPIO);
			gpio_clr_gpio_pin(62);
			gpio_clr_gpio_pin(63);
			delay_ms(100);
			break;
			
			case 2:
			gpio_set_gpio_pin(LED0_GPIO);
			gpio_set_gpio_pin(LED1_GPIO);
			gpio_clr_gpio_pin(LED2_GPIO);
			gpio_set_gpio_pin(LED3_GPIO);
			gpio_clr_gpio_pin(62);
			gpio_clr_gpio_pin(63);
			delay_ms(100);
			break;
			
			case 3:
			gpio_set_gpio_pin(LED0_GPIO);
			gpio_set_gpio_pin(LED1_GPIO);
			gpio_set_gpio_pin(LED2_GPIO);
			gpio_clr_gpio_pin(LED3_GPIO);
			gpio_clr_gpio_pin(62);
			gpio_clr_gpio_pin(63);
			delay_ms(100);
			break;
			
			case 4:
			gpio_set_gpio_pin(LED0_GPIO);
			gpio_set_gpio_pin(LED1_GPIO);
			gpio_set_gpio_pin(LED2_GPIO);
			gpio_set_gpio_pin(LED3_GPIO);
			gpio_set_gpio_pin(62);
			gpio_clr_gpio_pin(63);
			delay_ms(100);
			break;
			
			case 5:
			gpio_set_gpio_pin(LED0_GPIO);
			gpio_set_gpio_pin(LED1_GPIO);
			gpio_set_gpio_pin(LED2_GPIO);
			gpio_set_gpio_pin(LED3_GPIO);
			gpio_clr_gpio_pin(62);
			gpio_set_gpio_pin(63);
			direcc=0;
			delay_ms(100);
			break;
		}	
		if (direcc==1){
			contador++;
		}
		else{
			contador--;
		}
	}
}*/
//************************************************************************************************
