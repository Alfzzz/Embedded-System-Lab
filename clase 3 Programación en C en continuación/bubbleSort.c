/*Bubble Sort Algorithm*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define UPPER 100
#define LOWER 0
#define size 100
int main(){
	
	/*Generate array with random values*/
	int random_array[size],i;
	for(i=0;i<size;i++){
		random_array[i]=(rand()%(UPPER-LOWER+1))+LOWER;
	}
	
	/*Print unsorted array*/
	for(i=0;i<size;i++){
		printf("%i ",random_array[i]);
	}
	printf("\n");
	
	
	/*Sort array with bubble sort*/
	int aux,counter;
	while(true){	
		counter=0;
		for(i=1;i<size;i++){
			/*check if the value is less than the previous value*/
			if(random_array[i]<random_array[i-1]){
				/*swap values*/
				aux=random_array[i];
				random_array[i]=random_array[i-1];
				random_array[i-1]=aux;
				counter++;
			}
			
		}
		if(counter==0){ /*check if there are any swaps*/
			break;
		}
	}	
	
	/*print sorted array*/
	printf("The sorted array is:\n");
	for(i=0;i<size;i++){
		printf("%i ",random_array[i]);
	}
	
	return 0;
		
}
