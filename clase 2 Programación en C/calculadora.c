#include <stdio.h>
#include <math.h>

float multiplication(float a,float b);
float summation(float a,float b);
float division(float a,float b);
float subtract(float a,float b);
float root_square(float a);
int bitwiseAND(int number1,int number2);

int bitwiseOR(int number1,int number2);
int bitwiseXOR(int number1,int number2);
int onesComplement(int number1);
int leftShift(int number1,int number2);
int rightShift(int number1,int number2);

int main(void){ 
	float number1,number2,result;
	int operation;
	while(1){
		printf("Enter first number");
		scanf("%f",&number1);
		printf("Enter second number:");
		scanf("%f",&number2);
		printf("Enter operation number:\n1. Summation \n2. Substact \n3. Multiplication \n4. Division \n5. Root square of first number \n6. Bitwise AND  \n7. Bitwise OR  \n8. Bitwise XOR \n9. Binary ones's complement of first number\n10. Left shift  \n11. Right shift' ");
		scanf("%i",&operation);
		switch(operation)
		{
		    case 1:
		        result=summation(number1,number2);	
		        break;
		    case 2:
		        result=subtract(number1,number2);
		        break;
		    case 3:
		        result=multiplication(number1,number2);
		        break;
		    case 4:
		    	result=division(number1,number2);
		    	break;
		    case 5:
		    	result=root_square(number1);
		    	break;
		    case 6:
		    	result=bitwiseAND(number1,number2);
		    	break;
			case 7:
		    	result=bitwiseOR(number1,number2);
		    	break;
			case 8:
		    	result=bitwiseXOR(number1,number2);
		    	break;
			case 9:
		    	result=onesComplement(number1);
		    	break;
			case 10:
		    	result=leftShift(number1,number2);
		    	break;
			case 11:
		    	result=rightShift(number1,number2);
		    	break;
		}
		printf("The result is: %f \n ", result);
	}
}

float summation(float a,float b){ 
	
	float mul=a+b;
	return mul;
}
float multiplication(float a,float b){ 
	
	float mul=a*b;
	return mul;
}
float division(float a,float b){ 
	
	float div=a/b;
	return div;
}
float subtract(float a,float b){ 
	
	float sub=a-b;
	return sub;
}
float root_square(float a){ 
	
	float sub=sqrt(a);
	return sub;
}
int bitwiseAND(int number1,int number2){ 
	
	int AND=number1&number2;
	return AND;
}
int bitwiseOR(int number1,int number2){ 
	
	int OR=number1|number2;
	return OR;
}
int bitwiseXOR(int number1,int number2){ 
	
	int XOR=number1^number2;
	return XOR;
}
int onesComplement(int number1){ 
	
	int oC=~number1;
	return oC;
}
int leftShift(int number1,int number2){ 
	
	int ls=number1<<number2;
	return ls;
}
int rightShift(int number1,int number2){ 
	
	int rs=number2>>number1;
	return rs;
}


