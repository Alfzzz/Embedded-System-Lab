#include <stdio.h>

void rectangle(int a,int b);
void shifted_rectangle(int a,int b);
void rect_triangle(int a);
void inverted_rect_triangle(int a);
void filled_rhombus(int a);
void not_filled_rhombus(int a);
void heart(void);
void cross(int a);
int main(void){ 
	int a,b,figure_number;
	while(1){
		printf("Enter shape number:\n1. Rectangle\n2. Shifted rectangle \n3.Rect Triange\n4. Inverted Rect triangle\n5. filled Rhombus\n6. Not filled Rhombus\n7. heart\n8. Cross ");
		scanf("%i",&figure_number);
		if (figure_number==1){
			printf("Enter length of side a\n ");
			scanf("%i",&a);
			printf("Enter length of side b\n ");
			scanf("%i",&b);
			rectangle(a,b);	
		}else if(figure_number==2){
			printf("Enter length of side a\n ");
			scanf("%i",&a);
			printf("Enter length of side b\n ");
			scanf("%i",&b);
			shifted_rectangle(a,b);	
		}else if (figure_number==3){
			printf("Enter length of side\n ");
			scanf("%i",&a);
			rect_triangle(a);	
		}else if (figure_number==4){
			printf("Enter length the vertical diagonal\n ");
			scanf("%i",&a);
			inverted_rect_triangle(a);	
		}else if (figure_number==5){
			printf("Enter length of diagonal(odd number)\n ");
			scanf("%i",&a);
			filled_rhombus(a);	
		}else if (figure_number==6){
			printf("Enter length of diagonal(odd number)\n ");
			scanf("%i",&a);
			not_filled_rhombus(a);	
		}else if (figure_number==7){
			heart();	
		}else if (figure_number==8){
			printf("Enter size(odd number)\n ");
			scanf("%i",&a);
			cross(a);	
		}
		printf("\n");
	}
}

void rectangle(int a,int b){ 
	
	int i,j;
	for(i = 0; i < a; i++){ 
    	for(j = 0; j < b; j++) putchar('*');
		printf("\n");
	}
}

void shifted_rectangle(int a,int b){ 
	
	int i,j,k;
	int	spaces=0;
	for(i = 0; i < a; i++){ 
		for(j = 0; j <spaces ; j++) putchar(' ');
    	for(k = 0; k < b; k++) putchar('*');
		printf("\n");
		spaces++;
	}
}
void rect_triangle(int a){ 
	
	int i,j,k;
	int	spaces=a-1;
	for(i = 0; i < a; i++){ 
		for(j = 0; j<spaces ; j++) putchar(' ');
    	for(k = 0; k <a-spaces; k++) putchar('*');
		printf("\n");
		spaces--;
	}
}

void inverted_rect_triangle(int a){ 
	
	int i,j,k;
	int	spaces=0;
	for(i = 0; i < a; i++){ 
		for(j = 0; j<spaces ; j++) putchar(' ');
    	for(k = 0; k <a-spaces; k++) putchar('*');
		printf("\n");
		spaces++;
	}
}

void filled_rhombus(int a){ 
	
	int i,j,k;
	int	spaces=(a-1)/2; 
	for(i = 0; i < (a-1)/2+1; i++){ 
		for(j = 0; j<spaces ; j++) putchar(' ');
    	for(k = 0; k <a-spaces*2; k++) putchar('*');
		printf("\n");
		spaces--;
	}
	spaces=1; 
	for(i = 0; i < (a-1)/2; i++){ 
		for(j = 0; j<spaces ; j++) putchar(' ');
    	for(k = 0; k <a-spaces*2; k++) putchar('*');
		printf("\n");
		spaces++;
	}
	
}

void not_filled_rhombus(int a){ 
	
	int i,j,k;
	int	outside_spaces=(a-1)/2; 
	int inside_spaces=1;
	
	for(i = 0; i<outside_spaces ; i++) putchar(' ');
	printf("*\n");
	
	for(i = 0; i < (a-1)/2; i++){ 
		for(j = 1; j<outside_spaces ; j++) putchar(' ');
    	putchar('*');
    	for(k = 0; k<inside_spaces ; k++) putchar(' ');
    	printf("*\n");
		outside_spaces--;
		inside_spaces=inside_spaces+2;

	}
	outside_spaces=1;
	 inside_spaces=inside_spaces-2;
	for(i = 1; i < (a-1)/2; i++){ 
		inside_spaces=inside_spaces-2;
		for(j =0; j<outside_spaces; j++) putchar(' ');
    	putchar('*');
    	for(k = 0; k<inside_spaces; k++) putchar(' ');
    	printf("*\n");
		outside_spaces++;
	}
	for(i = 0; i<outside_spaces; i++) putchar(' ');
	printf("*");
		
}

void heart(void){ 
	int i,j,k;
	putchar(' ');
	putchar(' ');
	for(j = 0; j<4; j++) putchar('*');
	for(j = 0; j<4; j++) putchar(' ');
	for(j = 0; j<4; j++) putchar('*');
	printf("\n ");
	for(j = 0; j<6; j++) putchar('*');
	printf("  ");
	for(j = 0; j<6; j++) putchar('*');
	printf("\n");
	
	for(i = 0; i <9; i++){ 
		for(j = 0; j<i; j++) putchar(' ');
    	for(k = 0; k <16-i*2; k++) putchar('*');
		printf("\n");
	}
}

void cross(int a){ 
	int i,j,k,l;
	for(i = 0; i<a+2; i++) putchar('*');
	printf("\n");
	
	int side_spaces=0;
	int middle_spaces=a-2;
	
	for(i = 0; i <(a-1)/2; i++){ 
		putchar('*');
		for(j = 0; j<side_spaces; j++) putchar(' ');
		putchar('*');
		for(k = 0; k<middle_spaces; k++) putchar(' ');
		putchar('*');
		for(l = 0; l<side_spaces; l++) putchar(' ');
		printf("*\n");
		side_spaces++;
		middle_spaces=middle_spaces-2;
	}
	putchar('*');
	for(l = 0; l<(a-1)/2; l++) putchar(' ');
	putchar('*');
	for(l = 0; l<(a-1)/2; l++) putchar(' ');
	printf("*\n");

	side_spaces=(a-1)/2-1;
	middle_spaces=1;
		
	for(i = 0; i <(a-1)/2; i++){ 
		putchar('*');
		for(j = 0; j<side_spaces; j++) putchar(' ');
		putchar('*');
		for(k = 0; k<middle_spaces; k++) putchar(' ');
		putchar('*');
		for(l = 0; l<side_spaces; l++) putchar(' ');
		printf("*\n");
		side_spaces--;
		middle_spaces=middle_spaces+2;
	}
	
	for(i = 0; i<a+2; i++) putchar('*');
}
