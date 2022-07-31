/*This program prints the sizes of different types of data*/
#include <stdio.h>
int main()
{
    printf("Size of char: %lu\n", sizeof(char));
    printf("Size of unsigned char: %lu\n", sizeof(unsigned char));
    printf("Size of int: %lu\n", sizeof(int));
    printf("Size of unsigned int: %lu\n", sizeof(unsigned int));
    printf("Size of unsigned short int: %lu\n", sizeof(unsigned short int));
    printf("Size of unsigned long int: %lu\n", sizeof(unsigned long int));
    printf("Size of unsigned long long int: %lu\n",sizeof(unsigned long long int));
    printf("Size of float: %lu\n", sizeof(float));
    printf("Size of double: %lu\n", sizeof(double));
    printf("Size of long double: %lu\n", sizeof(long double));
    return 0;
}
