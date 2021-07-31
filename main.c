#include <psputils.h>
#include <time.h>
#include <math.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "callbacks.c"
#include "printstuff.c"

PSP_MODULE_INFO("Experiments", 0, 1, 1);

#define	printf pspDebugScreenPrintf
#define	cls pspDebugScreenClear
#define	pause sceKernelDelayThread

#define	UP 1
#define	RIGHT 2
#define	DOWN 3
#define	LEFT 4

SceCtrlData		pad;
int			infoX = 51;
unsigned long	mainColor = 0x00FF00FF;
unsigned long	mainColorDarker = 0x007F00FF;
unsigned long	snake1Color = 0xFF0000FF;
unsigned long	snake2Color = 0x0000FFFF;
int			direction;
int			direction2;
int			previousDirection;
int			previousDirection2;
int			length;
int			length2;
int			levelWin = FALSE;
int			level;
int			lives;
int			lives2;
int			oldLives;
int			oldLives2;
char			message2[99];
int			foodX;
int			foodY;
int			randVal;
int			currentFood;
int			currentFood2;
int			players;
int			snakeFailed;
int			quitGame;
int			colorCountero;

int			snakeX[999];
int			snakeY[999];
int			snakeX2[999];
int			snakeY2[999];


void drawBorder( void )
{
	int	i2;
	printHex(mainColor, 0, 0, "--------------------------------------------------------------------");
	for( i2=1; i2<33; i2++ ) {
		printHex(mainColor, 0, i2, "|                                                ||                |");
	}
	printHex(mainColor, 0, 33, "-------------------------------------------------------------------");
}


void greetings( void )
{
	int	gRed = 0xFF;
	int	gGreen = 0x00;
	int	gBlue = 0x00;
	int	gCC = 1;
	int	finishedGreetings = FALSE;
	int	yC = 1;

	while( ! finishedGreetings ) {
		printRGBA(gRed, gGreen, gBlue, 0xFF, infoX+1, yC, "Dangerous Asp!");
		printRGBA(gRed, gGreen, gBlue, 0xFF, infoX+2, yC + 2, "Hold Any Key");
		printRGBA(gRed, gGreen, gBlue, 0xFF, infoX+2, yC + 3, "To Continue!");
		pause(500*1000);
		sceCtrlReadBufferPositive(&pad, 1);
		if( pad.Buttons != 0 ) {
			finishedGreetings = TRUE;
		}
		if( gCC==1 ) {
			gGreen = 0xFF;
		} else if( gCC==2 ) {
			gRed = 0x00;
		} else if( gCC==3 ) {
			gBlue = 0xFF;
		} else if( gCC==4 ) {
			gGreen = 0x00;
		} else if( gCC==5 ) {
			gRed = 0xFF;
		} else if( gCC==6 ) {
			gBlue = 0x00;
			gCC = 0;
		}
		gCC++;
		if( yC==29 ) {
			yC=1;
			printHex(0x000000FF, infoX, 29, "                ");
			printHex(0x000000FF, infoX, 31, "                ");
			printHex(0x000000FF, infoX, 32, "                ");
		} else {
			yC++;
			printHex(0x000000FF, infoX, yC-1, "                ");
			printHex(0x000000FF, infoX, yC+1, "                ");
			printHex(0x000000FF, infoX, yC+2, "                ");
		}
	}	
}

void printStats()
{
	char	message1[99];
	if( players==1 ) {
		printHex(mainColor, infoX + 4, 2, "PLAYER 1");
		sprintf(message1, "LIVES: %d", lives);
		printHex(mainColor, infoX + 1, 8, message1);
		sprintf(message1, "LEVEL: %d", level);
		printHex(mainColor, infoX + 1, 10, message1);
		sprintf(message1, "FOOD EATEN: %d", length-5);
		printHex(mainColor, infoX + 1, 12, message1);
	} else {
		printHex(mainColor, infoX + 4, 2, "PLAYER 1");
		sprintf(message1, "LIVES: %d", lives);
		printHex(mainColor, infoX + 1, 8, message1);
		sprintf(message1, "FOOD EATEN: %d", length-5);
		printHex(mainColor, infoX + 1, 10, message1);

		printHex(mainColor, infoX + 4, 20, "PLAYER 2");
		sprintf(message1, "LIVES: %d", lives2);
		printHex(mainColor, infoX + 1, 26, message1);
		sprintf(message1, "FOOD EATEN: %d", length2-5);
		printHex(mainColor, infoX + 1, 28, message1);
	}
}

void countIn( void )
{
	drawBorder();
	printStats();
	
	printHex(mainColor, 21, 12, " 333 ");
	printHex(mainColor, 21, 13, "3   3");
	printHex(mainColor, 21, 14, "    3");
	printHex(mainColor, 21, 15, "   3 ");
	printHex(mainColor, 21, 16, "    3");
	printHex(mainColor, 21, 17, "3   3");
	printHex(mainColor, 21, 18, " 333 ");
	pause(1000*1000);
	
	printHex(mainColor, 21, 12, " 222 ");
	printHex(mainColor, 21, 13, "2   2");
	printHex(mainColor, 21, 14, "    2");
	printHex(mainColor, 21, 15, "   2 ");
	printHex(mainColor, 21, 16, "  2  ");
	printHex(mainColor, 21, 17, " 2   ");
	printHex(mainColor, 21, 18, "22222");
	pause(1000*1000);

	printHex(mainColor, 21, 12, " 11  ");
	printHex(mainColor, 21, 13, "1 1  ");
	printHex(mainColor, 21, 14, "  1  ");
	printHex(mainColor, 21, 15, "  1  ");
	printHex(mainColor, 21, 16, "  1  ");
	printHex(mainColor, 21, 17, "  1  ");
	printHex(mainColor, 21, 18, "11111");
	pause(1000*1000);

	printHex(mainColor, 1, 6, "            Y   Y   OOOO    U    U              ");
	printHex(mainColor, 1, 7, "             Y Y   O    O   U    U              ");
	printHex(mainColor, 1, 8, "              Y    O    O   U    U              ");
	printHex(mainColor, 1, 9, "              Y    O    O   U    U              ");
	printHex(mainColor, 1, 10, "              Y    O    O   U    U              ");
	printHex(mainColor, 1, 11, "              Y     OOOO     UUUU               ");
	printHex(mainColor, 1, 12, "                                                ");
	printHex(mainColor, 1, 13, "                  GGGG    OOOO                  ");
	printHex(mainColor, 1, 14, "                 G    G  O    O                 ");
	printHex(mainColor, 1, 15, "                 G       O    O                 ");
	printHex(mainColor, 1, 16, "                 G  GGG  O    O                 ");
	printHex(mainColor, 1, 17, "                 G    G  O    O                 ");
	printHex(mainColor, 1, 18, "                  GGGG    OOOO                  ");
	printHex(mainColor, 1, 19, "                                                ");
	printHex(mainColor, 1, 20, "           FFFF  III  RRR    SSSS  TTTTT        ");
	printHex(mainColor, 1, 21, "           F      I   R  R  S        T          ");
	printHex(mainColor, 1, 22, "           FFF    I   RRR    SSS     T          ");
	printHex(mainColor, 1, 23, "           F      I   R R       S    T          ");
	printHex(mainColor, 1, 24, "           F     III  R  R  SSSS     T          ");

	pause(1000*1000);
	
	drawBorder();
}

void makeFood( void )
{
	int	i3;
	int	realFood;
	realFood = FALSE;
	srand ( time(NULL) );
	while( ! realFood ) {
		realFood = TRUE;
		foodX = ( rand() % 48 ) + 1;
		foodY = ( rand() % 32 ) + 1;
		for( i3=0; i3<length-1; i3++ ) {
			if( foodX == snakeX[i3] && foodY == snakeY[i3] ) {
				realFood = FALSE;
			}
		}
		if( players==2 ) {
			for( i3=0; i3<length2-1; i3++ ) {
				if( foodX == snakeX2[i3] && foodY == snakeY2[i3] ) {
					realFood = FALSE;
				}
			}
		}
	}
	printHex(mainColor, foodX, foodY, "*");
}

void pauseMenu( void )
{
	if( colorCountero==1 ) {
		mainColor = 0xFF0000FF;
		mainColorDarker = 0x7F0000FF;
	} else if( colorCountero==2 ) {
		mainColor = 0xFFFF00FF;
		mainColorDarker = 0x7F7F00FF;
	} else if( colorCountero==3 ) {
		mainColor = 0x00FF00FF;
		mainColorDarker = 0x007F00FF;
	} else if( colorCountero==4 ) {
		mainColor = 0x00FFFFFF;
		mainColorDarker = 0x007F7FFF;
	} else if( colorCountero==5 ) {
		mainColor = 0x0000FFFF;
		mainColorDarker = 0x00007FFF;
	} else if( colorCountero==6 ) {
		mainColor = 0xFF00FFFF;
		mainColorDarker = 0x7F007FFF;
	}
	drawBorder();
	printStats();
	printHex(mainColor, 22, 6, "PAUSED!");
	printHex(mainColor, 2, 9, "Start: Continue");
	printHex(mainColor, 2, 10, "Triangle: Quit");
	printHex(mainColor, 2, 11, "L-Trigger: Change House Color");
	printHex(mainColor, 2, 12, "R-Trigger: Change House Color");
	pause(250*1000);
}

void pauseGame( void )
{
	int	i3;
	int	notHeldDown;
	int	notPaused;
	notPaused = FALSE;
	notHeldDown = FALSE;
	drawBorder();
	printStats();
	printHex(mainColor, 22, 6, "PAUSED!");
	printHex(mainColor, 2, 9, "Start: Continue");
	printHex(mainColor, 2, 10, "Triangle: Quit");
	printHex(mainColor, 2, 11, "L-Trigger: Change House Color");
	printHex(mainColor, 2, 12, "R-Trigger: Change House Color");
	while( ! notHeldDown ) {
		sceCtrlReadBufferPositive(&pad, 1);
		if( ! ( pad.Buttons & PSP_CTRL_START ) ) {
			notHeldDown = TRUE;
		}
	}
	while( ! notPaused ) {
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_START){
			notPaused = TRUE;
		}
		if( pad.Buttons & PSP_CTRL_TRIANGLE ) {
			quitGame = TRUE;
			notPaused = TRUE;
		}
		if( pad.Buttons & PSP_CTRL_LTRIGGER ) {
			if( colorCountero==1 ) {
				colorCountero = 6;
			} else {
				colorCountero--;
			}
			pauseMenu();
		}
		if( pad.Buttons & PSP_CTRL_RTRIGGER ) {
			if( colorCountero==6 ) {
				colorCountero = 1;
			} else {
				colorCountero++;
			}
			pauseMenu();
		}
	}
	if( ! quitGame ) {
		drawBorder();
		printStats();
		printHex(mainColor, foodX, foodY, "*");
		if( players==1 ) {
			printHex(mainColor, snakeX[0], snakeY[0], "%%");
			for( i3=1; i3<length-1; i3++ ) {
				printHex(mainColor, snakeX[i3], snakeY[i3], "o");
			}
		} else {
			printHex(snake1Color, snakeX[0], snakeY[0], "%%");
			for( i3=1; i3<length-1; i3++ ) {
				printHex(snake1Color, snakeX[i3], snakeY[i3], "o");
			}
			printHex(snake2Color, snakeX2[0], snakeY2[0], "%%");
			for( i3=1; i3<length2-1; i3++ ) {
				printHex(snake2Color, snakeX2[i3], snakeY2[i3], "o");
			}
		}
	}
}

void checkDirection( void )
{
	int	notHeldDownEither;
	notHeldDownEither = FALSE;
	sceCtrlReadBufferPositive(&pad, 1);
	if( pad.Buttons != 0 ) {
		if (pad.Buttons & PSP_CTRL_UP){
			if( previousDirection != DOWN ) {
				if( previousDirection != UP ) {
					direction = UP;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_DOWN){
			if( previousDirection != UP ) {
				if( previousDirection != DOWN ) {
					direction = DOWN;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_LEFT){
			if( previousDirection != RIGHT ) {
				if( previousDirection != LEFT ) {
					direction = LEFT;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_RIGHT){
			if( previousDirection != LEFT ) {
				if( previousDirection != RIGHT ) {
					direction = RIGHT;
				}
			}
		}
		if (pad.Buttons & PSP_CTRL_START){
			pauseGame();
			while( ! notHeldDownEither ) {
				sceCtrlReadBufferPositive(&pad, 1);
				if( ! ( pad.Buttons & PSP_CTRL_START ) ) {
					notHeldDownEither = TRUE;
				}
			}
		}
	}
}

void checkMultiDirection( void )
{
	int	notHeldDownEither;
	notHeldDownEither = FALSE;
	sceCtrlReadBufferPositive(&pad, 1);
	if( pad.Buttons != 0 ) {		
		if (pad.Buttons & PSP_CTRL_TRIANGLE){
			if( previousDirection2 != DOWN ) {
				if( previousDirection2 != UP ) {
					direction2 = UP;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_CROSS){
			if( previousDirection2 != UP ) {
				if( previousDirection2 != DOWN ) {
					direction2 = DOWN;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_SQUARE){
			if( previousDirection2 != RIGHT ) {
				if( previousDirection2 != LEFT ) {
					direction2 = LEFT;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_CIRCLE){
			if( previousDirection2 != LEFT ) {
				if( previousDirection2 != RIGHT ) {
					direction2 = RIGHT;
				}
			}
		}
		if (pad.Buttons & PSP_CTRL_UP){
			if( previousDirection != DOWN ) {
				if( previousDirection != UP ) {
					direction = UP;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_DOWN){
			if( previousDirection != UP ) {
				if( previousDirection != DOWN ) {
					direction = DOWN;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_LEFT){
			if( previousDirection != RIGHT ) {
				if( previousDirection != LEFT ) {
					direction = LEFT;
				}
			}
		} 
		if (pad.Buttons & PSP_CTRL_RIGHT){
			if( previousDirection != LEFT ) {
				if( previousDirection != RIGHT ) {
					direction = RIGHT;
				}
			}
		}
		if (pad.Buttons & PSP_CTRL_START){
			pauseGame();
			while( ! notHeldDownEither ) {
				sceCtrlReadBufferPositive(&pad, 1);
				if( ! ( pad.Buttons & PSP_CTRL_START ) ) {
					notHeldDownEither = TRUE;
				}
			}
		}

	}
}



void startGame( void )
{
	int	finished;
	int 	i;
	int	j;
	finished = FALSE;

	sceCtrlReadBufferPositive(&pad, 1);
	snakeX[0] = 2;
	snakeY[0] = 16;
	for( j=1; j<length; j++ ) {
		snakeX[j] = 0;
		snakeY[j] = 0;
	}
	direction = RIGHT;
	previousDirection = RIGHT;
	currentFood = 0;
	printStats();
	makeFood();

	while( ! finished ) {

		printHex(mainColor, snakeX[0], snakeY[0], "%%");
		sceCtrlReadBufferPositive(&pad, 1);
		for( j=0;j<(26-level);j++ ) {
			pause( 5*1000 ) ;
			checkDirection();
		}
		if( quitGame ) {
			finished = TRUE;
		} else {
			previousDirection = direction;
		
			for( i=0;i<length-1;i++ ) {
				snakeX[length-1-i] = snakeX[length-2-i];
				snakeY[length-1-i] = snakeY[length-2-i];
			}

			if( direction==UP ) {
				if( snakeY[0]==1 ) {
					pause(1000*1000);
					finished = TRUE;
				} else {
					snakeY[0]--;
				}
			} else if( direction==RIGHT ) {
				if( snakeX[0]==48 ) {
					pause(1000*1000);
					finished = TRUE;
				} else {
					snakeX[0]++;
				}
			} else if( direction==DOWN ) {
				if( snakeY[0]==32 ) {
					pause(1000*1000);
					finished = TRUE;
				} else {
					snakeY[0]++;
				}
			} else if( direction==LEFT ) {
				if( snakeX[0]==1 ) {
					pause(1000*1000);
					finished = TRUE;
				} else {
					snakeX[0]--;
				}
			}
			for( i=1;i<length-1;i++ ) {
				if( snakeX[0] == snakeX[i] ) {
					if( snakeY[0] == snakeY[i] ) {
						pause(1000*1000);
						finished = TRUE;
					}
				}
			}
			if( snakeX[0]==foodX && snakeY[0]==foodY ) {
				length++;
				currentFood++;
				if( currentFood==4 ) {
					currentFood = 0;
					if( level != 20 ) {
						level++;
					}
				}
				printStats();
				makeFood();
			} else {
				printHex(mainColor, snakeX[length-1], snakeY[length-1], " ");
			}
			printHex(mainColor, snakeX[1], snakeY[1], "o");
		}
	}
}

void singleSetup()
{
	int	ii;
	int	gameFinished = FALSE;
	drawBorder();
	while( ! gameFinished ) {
		level = 1;
		lives = 4;
		length = 5;
		while( lives != 0) {
			countIn();
			startGame();
			if( quitGame ) {
				oldLives = lives;
				lives=0;
			} else {
				for( ii=1;ii<33;ii++ ) {
					printHex(mainColor, 1, ii, "************************************************");
					pause(50*1000);
				}
				for( ii=1;ii<33;ii++ ) {
					printHex(mainColor, 1, ii, "                                                ");
					pause(50*1000);
				}
				lives--;
			}
		}
		lives=oldLives;
		if( quitGame ) {
			printHex(mainColor, 1, 4, "               You Ended The Game.              ");
			quitGame = FALSE;
		} else {
			printHex(mainColor, 1, 4, "                  GAME OVER!!!                  ");
		}
		drawBorder();
		printStats();
		printHex(mainColor, 1, 6, "          Would you like to play again?         ");
		printHex(mainColor, 1, 12, " Triangle: Quit.  Any Other Button: Play again! ");
		pause(1000*1000);
		sceCtrlReadBufferPositive(&pad, 1);
		while(pad.Buttons == 0) {
			sceCtrlReadBufferPositive(&pad, 1);
		}
		if (pad.Buttons & PSP_CTRL_TRIANGLE){
			gameFinished = TRUE;
		}
	}
}





void startMultiGame()
{
	int	finished;
	int 	i;
	int	i3;
	int	j;
	int	realFood;
	finished = FALSE;

	sceCtrlReadBufferPositive(&pad, 1);
	snakeX[0] = 2;
	snakeY[0] = 16;
	snakeX2[0] = 46;
	snakeY2[0] = 16;
	for( j=1; j<length; j++ ) {
	snakeX[j] = 0;
	snakeY[j] = 0;	
	}
	for( j=1; j<length2; j++ ) {
	snakeX2[j] = 0;
	snakeY2[j] = 0;	
	}
	direction = RIGHT;
	previousDirection = RIGHT;
	direction2 = LEFT;
	previousDirection2 = LEFT;
	printStats();
	makeFood();

	while( ! finished ) {

		printHex(snake1Color, snakeX[0], snakeY[0], "%%");
		printHex(snake2Color, snakeX2[0], snakeY2[0], "%%");
		sceCtrlReadBufferPositive(&pad, 1);
		for( j=0;j<(10);j++ ) {
			pause( 5*1000 ) ;
			checkMultiDirection();
		}
		
		if( quitGame ) {
			finished = TRUE;
		}
		
		previousDirection = direction;
		previousDirection2 = direction2;
		
		for( i=0;i<length-1;i++ ) {
			snakeX[length-1-i] = snakeX[length-2-i];
			snakeY[length-1-i] = snakeY[length-2-i];
		}

		for( i=0;i<length2-1;i++ ) {
			snakeX2[length2-1-i] = snakeX2[length2-2-i];
			snakeY2[length2-1-i] = snakeY2[length2-2-i];
		}

		if( direction==UP ) {
			if( snakeY[0]==1 ) {
				pause(1000*1000);
				snakeFailed = 1;
				finished = TRUE;
			} else {
				snakeY[0]--;
			}
		} else if( direction==RIGHT ) {
			if( snakeX[0]==48 ) {
				pause(1000*1000);
				snakeFailed = 1;
				finished = TRUE;
			} else {
				snakeX[0]++;
			}
		} else if( direction==DOWN ) {
			if( snakeY[0]==32 ) {
				pause(1000*1000);
				snakeFailed = 1;
				finished = TRUE;
			} else {
				snakeY[0]++;
			}
		} else if( direction==LEFT ) {
			if( snakeX[0]==1 ) {
				pause(1000*1000);
				snakeFailed = 1;
				finished = TRUE;
			} else {
				snakeX[0]--;
			}
		}
		
		if( direction2==UP ) {
			if( snakeY2[0]==1 ) {
				pause(1000*1000);
				snakeFailed = 2;
				finished = TRUE;
			} else {
				snakeY2[0]--;
			}
		} else if( direction2==RIGHT ) {
			if( snakeX2[0]==48 ) {
				pause(1000*1000);
				snakeFailed = 2;
				finished = TRUE;
			} else {
				snakeX2[0]++;
			}
		} else if( direction2==DOWN ) {
			if( snakeY2[0]==32 ) {
				pause(1000*1000);
				snakeFailed = 2;
				finished = TRUE;
			} else {
				snakeY2[0]++;
			}
		} else if( direction2==LEFT ) {
			if( snakeX2[0]==1 ) {
				pause(1000*1000);
				snakeFailed = 2;
				finished = TRUE;
			} else {
				snakeX2[0]--;
			}
		}

		
		for( i=1;i<length-1;i++ ) {
			if( snakeX[0] == snakeX[i] ) {
				if( snakeY[0] == snakeY[i] ) {
					pause(1000*1000);
					snakeFailed = 1;
					finished = TRUE;
				}
			}
		}
		for( i=1;i<length2-1;i++ ) {
			if( snakeX2[0] == snakeX2[i] ) {
				if( snakeY2[0] == snakeY2[i] ) {
					pause(1000*1000);
					snakeFailed = 2;
					finished = TRUE;
				}
			}
		}
		
		for( i=1;i<length2-1;i++ ) {
			if( snakeX[0] == snakeX2[i] ) {
				if( snakeY[0] == snakeY2[i] ) {
					pause(1000*1000);
					snakeFailed = 1;
					finished = TRUE;
				}
			}
		}
		for( i=1;i<length-1;i++ ) {
			if( snakeX2[0] == snakeX[i] ) {
				if( snakeY2[0] == snakeY[i] ) {
					pause(1000*1000);
					snakeFailed = 2;
					finished = TRUE;
				}
			}
		}
		if( (snakeX2[0] == snakeX[0]) &&  ( snakeY2[0] == snakeY[0]) ) {
			pause(1000*1000);
			snakeFailed = 3;
			finished = TRUE;
		}
		
		if( snakeX[0]==foodX && snakeY[0]==foodY ) {
			length++;
			printStats();
			makeFood();
		} else {
			printHex(snake1Color, snakeX[length-1], snakeY[length-1], " ");
		}

		if( snakeX2[0]==foodX && snakeY2[0]==foodY ) {
			length2++;
			printStats();
			realFood = FALSE;
			while( ! realFood ) {
				realFood = TRUE;
				makeFood();
				for( i3=0; i3<length2-1; i3++ ) {
					if( foodX == snakeX2[i3] && foodY == snakeY2[i3] ) {
						realFood = FALSE;
					}
				}
				for( i3=0; i3<length-1; i3++ ) {
					if( foodX == snakeX[i3] && foodY == snakeY[i3] ) {
						realFood = FALSE;
					}
				}
			}
		} else {
			printHex(snake2Color, snakeX2[length2-1], snakeY2[length2-1], " ");
		}
		printHex(snake1Color, snakeX[1], snakeY[1], "o");
		printHex(snake2Color, snakeX2[1], snakeY2[1], "o");
	}
}

void multiSetup()
{
	int	ii;
	int	gameFinished = FALSE;
	drawBorder();
	while( ! gameFinished ) {
		lives = 4;
		length = 5;
		lives2 = 4;
		length2 = 5;
		
		while( (lives != 0) && (lives2 != 0) ) {
			countIn();
			startMultiGame();
			if( quitGame ) {
				oldLives = lives;
				oldLives2 = lives2;
				lives = 0;
			} else {
				for( ii=1;ii<33;ii++ ) {
					printHex(mainColor, 1, ii, "************************************************");
					pause(50*1000);
				}
				for( ii=1;ii<33;ii++ ) {
					printHex(mainColor, 1, ii, "                                                ");
					pause(50*1000);
				}
				if( snakeFailed==1 ) {
					lives--;
				} else if( snakeFailed==2 ) {
					lives2--;
				} else {
					lives--;
					lives2--;
				}
			}
		}
		drawBorder();
		if( quitGame ) {
			lives = oldLives;
			lives2 = oldLives2;
			printHex(mainColor, 1, 4, "                 GAME EXITED!!!                 ");
			quitGame = FALSE;
		} else {
			printHex(mainColor, 1, 4, "                  GAME OVER!!!                  ");
			if( lives==0 && lives2==0 ) {
				printHex(mainColor, 1, 6, "                  TIE GAME!!!                   ");
			} else if( lives==0 ) {
				printHex(mainColor, 1, 6, "    PLAYER 1 LOSES!           PLAYER 2 WINS!    ");
			} else {
				printHex(mainColor, 1, 6, "    PLAYER 1 WINS!           PLAYER 2 LOSES!    ");
			}
		}
		printStats();
		printHex(mainColor, 1, 12, " Triangle: Quit.  Any Other Button: Play again! ");
		pause(1000*1000);
		sceCtrlReadBufferPositive(&pad, 1);
		while(pad.Buttons == 0) {
			sceCtrlReadBufferPositive(&pad, 1);
		}
		if (pad.Buttons & PSP_CTRL_TRIANGLE){
			gameFinished = TRUE;
		}
	}

}

void multiCreate()
{
	int	gameWillStart = FALSE;
	int	colorCounter1;
	int	colorCounter2;
	
	colorCounter1 = 1;
	colorCounter2 = 5;
	drawBorder();
	printHex(mainColor, 1, 3, "                Pick Your Color!                ");
	printHex(mainColor, 1, 5, "            Then press Start to play!           ");
	printHex(snake1Color, 11, 8, "PLAYER 1");
	printHex(snake2Color, 31, 8, "PLAYER 2");
	while( ! gameWillStart ) {
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_LEFT){
			if( colorCounter1==1 ) {
				colorCounter1 = 6;
			} else {
				colorCounter1--;
			}
			if( colorCounter1==1 ) {
				snake1Color = 0xFF0000FF;
			} else if( colorCounter1==2 ) {
				snake1Color = 0xFFFF00FF;
			} else if( colorCounter1==3 ) {
				snake1Color = 0x00FF00FF;
			} else if( colorCounter1==4 ) {
				snake1Color = 0x00FFFFFF;
			} else if( colorCounter1==5 ) {
				snake1Color = 0x0000FFFF;
			} else if( colorCounter1==6 ) {
				snake1Color = 0xFF00FFFF;
			}
			printHex(snake1Color, 11, 8, "PLAYER 1");
			pause(300*1000);
		}
		if (pad.Buttons & PSP_CTRL_RIGHT){
			if( colorCounter1==6 ) {
				colorCounter1 = 1;
			} else {
				colorCounter1++;
			}
			if( colorCounter1==1 ) {
				snake1Color = 0xFF0000FF;
			} else if( colorCounter1==2 ) {
				snake1Color = 0xFFFF00FF;
			} else if( colorCounter1==3 ) {
				snake1Color = 0x00FF00FF;
			} else if( colorCounter1==4 ) {
				snake1Color = 0x00FFFFFF;
			} else if( colorCounter1==5 ) {
				snake1Color = 0x0000FFFF;
			} else if( colorCounter1==6 ) {
				snake1Color = 0xFF00FFFF;
			}
			printHex(snake1Color, 11, 8, "PLAYER 1");
			pause(300*1000);
		}
		if (pad.Buttons & PSP_CTRL_SQUARE){
			if( colorCounter2==1 ) {
				colorCounter2 = 6;
			} else {
				colorCounter2--;
			}
			if( colorCounter2==1 ) {
				snake2Color = 0xFF0000FF;
			} else if( colorCounter2==2 ) {
				snake2Color = 0xFFFF00FF;
			} else if( colorCounter2==3 ) {
				snake2Color = 0x00FF00FF;
			} else if( colorCounter2==4 ) {
				snake2Color = 0x00FFFFFF;
			} else if( colorCounter2==5 ) {
				snake2Color = 0x0000FFFF;
			} else if( colorCounter2==6 ) {
				snake2Color = 0xFF00FFFF;
			}
			printHex(snake2Color, 31, 8, "PLAYER 2");
			pause(300*1000);
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE){
			if( colorCounter2==6 ) {
				colorCounter2 = 1;
			} else {
				colorCounter2++;
			}
			if( colorCounter2==1 ) {
				snake2Color = 0xFF0000FF;
			} else if( colorCounter2==2 ) {
				snake2Color = 0xFFFF00FF;
			} else if( colorCounter2==3 ) {
				snake2Color = 0x00FF00FF;
			} else if( colorCounter2==4 ) {
				snake2Color = 0x00FFFFFF;
			} else if( colorCounter2==5 ) {
				snake2Color = 0x0000FFFF;
			} else if( colorCounter2==6 ) {
				snake2Color = 0xFF00FFFF;
			}
			printHex(snake2Color, 31, 8, "PLAYER 2");
			pause(300*1000);
		}
		if (pad.Buttons & PSP_CTRL_START){
			gameWillStart = TRUE;
		}
	}
	multiSetup();
}

int main( void )
{
	int	menuFinished = FALSE;
	int	menuSelected;

	colorCountero = 3;

	pspDebugScreenInit();
	SetupCallbacks();
	
	while( ! menuFinished ) {
		quitGame = FALSE;
		players = 3;
		menuSelected = FALSE;
		drawBorder();
		greetings();
		drawBorder();
		printHex(mainColor, 1, 4, "                 Dangerous Asp!                 ");
		printHex(mainColor, 1, 8, "          How many players? (X to select)       ");
		printHex(mainColorDarker, 16, 11, "One");
		printHex(mainColorDarker, 30, 11, "Two");
		while( ! menuSelected ) {
			sceCtrlReadBufferPositive(&pad, 1);
			if (pad.Buttons & PSP_CTRL_LEFT){
				printHex(mainColor, 16, 11, "One");
				printHex(mainColorDarker, 30, 11, "Two");
				players = 1;
			} 
			if (pad.Buttons & PSP_CTRL_RIGHT){
				printHex(mainColorDarker, 16, 11, "One");
				printHex(mainColor, 30, 11, "Two");
				players = 2;
			}
			if (pad.Buttons & PSP_CTRL_CROSS){
				if( players != 3 ) {
					menuSelected = TRUE;
				}
			}
		}
		if( players==1 ) {
			singleSetup();
		} else {
			multiCreate();
		}
	}
	
	sceKernelSleepThread(); 
	//sceKernelExitGame();
	return 0;
}
