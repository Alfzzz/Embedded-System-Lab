#include <stdio.h>

#define name "Ismael"
const float PI = 3.1415;
float circum(float radius);
float area(float radius);

int main(void) {

    float radius;
    float circumV, areaV;

    printf("Enter radius: ");
    scanf("%f", &radius);

    circumV = circum(radius);
    areaV = area(radius);
     
    printf("Circumference: %f\n", circumV);
    printf("Area: %f\n", areaV);
    printf("Hello %s %s", name, ", you calculate the area and circumference of a circle");
}

float circum(float radius) {
  return 2 * PI * radius;
}

float area(float radius) {
  return PI * radius * radius;
}