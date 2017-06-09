#include "smarti.h"
#include <stdio.h>

#define STR_MAX 520

#define __MAX_INT (2 << ((sizeof(int) * 8) - 2))- 1
#define __MIN_INT -(__MAX_INT)-1

int main ()
{
    char valorString[STR_MAX];
    int valorInt;
    char valorChar;
    int length;

    printf("Precione tecla de cursor: ");
    switch(readArrow())
    {
    case ARROW_UP:
        printf("arriba\n");
        break;
    case ARROW_LEFT:
        printf("izquierda\n");
        break;
    case ARROW_RIGHT:
        printf("derecha\n");
        break;
    case ARROW_DOWN:
        printf("abajo\n");
        break;
    }

    printf("[PRECIONE UNA TECLA PARA CONTINUAR]");
    readch();

    clearTerminal();
    placeCursor(10, 10);

    printf("Esciba un char: ");
    length = readChar(&valorChar);
    printf("El char es: ");
    printf("%c", valorChar);
    printf("\n");
    printf("La longitud de la entrada es: ");
    printf("%d", length);
    printf("\n");

    printf("\n");

    printf("Esciba un string: ");
    length = readString(STR_MAX, valorString);
    printf("El string es: ");
    printf("%s", valorString);
    printf("\n");
    printf("La longitud de la entrada es: ");
    printf("%d", length);
    printf("\n");

    printf("\n");
	
	printf("Esciba un string (10 char max): ");
    length = readString(10, valorString);
    printf("El string es: ");
    printf("%s", valorString);
    printf("\n");
    printf("La longitud de la entrada es: ");
    printf("%d", length);
    printf("\n");

    printf("\n");

    printf("Esciba un int: ");
    length = readInt(&valorInt);
    printf("El int es: ");
    printf("%d", valorInt);
    printf("\n");
    printf("La longitud de la entrada es: ");
    printf("%d", length);
    printf("\n");

    printf("\n");
	
	printf("Esciba un string (oculto): ");
    length = readStringMask(STR_MAX, valorString, '*');
    printf("El string es: ");
    printf("%s", valorString);
    printf("\n");
    printf("La longitud de la entrada es: ");
    printf("%d", length);
    printf("\n");

    printf("\n");
	
	printf("Esciba un string (invisible): ");
    length = readStringSilent(STR_MAX, valorString);
    printf("El string es: ");
    printf("%s", valorString);
    printf("\n");
    printf("La longitud de la entrada es: ");
    printf("%d", length);
    printf("\n");

    printf("\n");

    return 0;
}
