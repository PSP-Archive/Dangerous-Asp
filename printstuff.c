#include <stdio.h>

void printHex( unsigned long color, int x, int y, char *str )
{
	char message[999];
	unsigned r1;
	unsigned g1;
	unsigned b1;
	unsigned a1;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	if( strlen( str ) < 800 ) {
		sprintf( message, "%s", str);
	} else {
		sprintf( message, "String too big (%d chars)", strlen(str) );
	}

	r1 = color & 0xFF000000;
	g1 = color & 0x00FF0000;
	b1 = color & 0x0000FF00;
	a1 = color & 0x000000FF;

	a = (char)(a1);
	b = (char)(b1 >> 8);
	g = (char)(g1 >> 16);
	r = (char)(r1 >> 24);

	pspDebugScreenSetXY(x,y);
	pspDebugScreenSetTextColor((a * 0x1000000) + (b * 0x10000) + (g * 0x100) + (r));
	pspDebugScreenPrintf(message);
}


void printRGBA( int r, int g, int b, int a, int x, int y, char *str )
{
	char message[999];

	if( strlen( str ) < 800 ) {
		sprintf( message, "%s", str);
	} else {
		sprintf( message, "String too big (%d chars)", strlen(str) );
	}

	pspDebugScreenSetXY(x,y);
	pspDebugScreenSetTextColor((a * 0x1000000) + (b * 0x10000) + (g * 0x100) + (r));
	pspDebugScreenPrintf(message);
}

