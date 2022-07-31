/*Sum of unidimensional array*/

#include <stdio.h>

int main(){
	int array1000[1000],i,sum=0;
	
	/*Create array of values from 0 to 1000*/
	for(i=1;i<1000+1;i++){
		array1000[i-1]=i;
	}
	
	/*Sum of the array*/
	for(i=0;i<1000;i++){
		sum=sum+array1000[i];
	}
	
	/*print result*/
	printf("Sum of the array=%d",sum);
	
	
	return 0;
}


