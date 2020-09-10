#include <stdio.h>

void printInt(int value)
{
    printf("%d\n", value);
}

int readInt()
{
    int value;
    scanf("%d", &value);
    return value;
}

void printReal(float value)
{
    printf("%f\n", value);
}

float readReal(){
    float value;
    scanf("%f", &value);
    return value;
}

void printString(char *value)
{
    printf("%s\n", value);
}
