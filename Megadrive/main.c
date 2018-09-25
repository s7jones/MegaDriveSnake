#include <genesis.h>

//moon.h is generated from moon.bmp
//just be sure to put your 16colors bitmap on 'res' folder
// file logo-blue-moon-25024.jpg is here for reference : it's the original picture I used

#include "moon.h"

const u32 tileSnake[8] =
{
	0x44444444,
	0x44444444,
	0x44444444,
	0x44444444,
	0x44444444,
	0x44444444,
	0x44444444,
	0x44444444
};

const u32 tileFruit[8] =
{
    0x00111100,
    0x01144110,
    0x11244211,
    0x11244211,
    0x11222211,
    0x11222211,
    0x01122110,
    0x00111100
};

typedef struct {
	const char* text;
	u16 x;
	u16 y;
} text;

u16 count = 0;
u16 direction = 0;
u16 steer = 0;
#define up		0
#define down	1
#define left	2
#define right	3

void myJoyHandler(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1) {
        if (state & BUTTON_UP)
        {
            if (direction != down) {
                steer = up;
            }
            VDP_drawText("up", 0, 0);
        }
        // maybe this could else if? is callback for each button state/change?
        if (state & BUTTON_DOWN)
        {
            if (direction != up) {
                steer = down;
            }
            VDP_drawText("down", 0, 1);
        }
        if (state & BUTTON_LEFT)
        {
            if (direction != right) {
                steer = left;
            }
            VDP_drawText("left", 0, 2);
        }
        if (state & BUTTON_RIGHT)
        {
            if (direction != left) {
                steer = right;
            }
            VDP_drawText("right", 0, 3);
        }

        // maybe this could be constexpr?
        u32 len;

        if (changed & ~state & BUTTON_UP)
        {
            VDP_drawText("up", 0, 4);
            len = strlen("up");
            VDP_clearText(0, 0, len);
        }
        // maybe this could else if? is callback for each button state/change?
        if (changed & ~state & BUTTON_DOWN)
        {
            VDP_drawText("down", 0, 5);
            len = strlen("down");
            VDP_clearText(0, 1, len);
        }

        if (changed & ~state & BUTTON_LEFT)
        {
            VDP_drawText("left", 0, 6);
            len = strlen("left");
            VDP_clearText(0, 2, len);
        }

        if (changed & ~state & BUTTON_RIGHT)
        {
            VDP_drawText("right", 0, 7);
            len = strlen("right");
            VDP_clearText(0, 3, len);
        }
	}
}

void steerSnake(Vect2D_u16* snakeHead)
{
	JOY_update();
	
	if (count > 5) {
		switch (steer) {
		case up:
			snakeHead->y -= 8;
			direction = up;
			break;
		case down:
			snakeHead->y += 8;
			direction = down;
			break;
		case left:
			snakeHead->x -= 8;
			direction = left;
			break;
		case right:
			snakeHead->x += 8;
			direction = right;
			break;
		}
		count = 0;
	}
	else ++count;

}

void updateSnakePosition(Vect2D_u16* snakeHead, Vect2D_u16* resolution)
{
    if (snakeHead->x >= resolution->x)
    {
        snakeHead->x = snakeHead->x % resolution->x;
    }
    else if (snakeHead->x < 0)
    {
        // % operator is remainder and so neg - not like modulus
        snakeHead->x = resolution->x + (snakeHead->x % resolution->x);
    }

    if (snakeHead->y >= resolution->y)
    {
        snakeHead->y = snakeHead->y % resolution->y;
    }
    else if (snakeHead->y < 0)
    {
        // % operator is remainder and so neg - not like modulus
        snakeHead->y = resolution->y + (snakeHead->y % resolution->y);
    }
}

u8 isSnakeEatFruit(Vect2D_u16* snakeHead, Vect2D_u16* fruit)
{
    if ((snakeHead->x == fruit->x) && (snakeHead->y == fruit->y))
    {
        return TRUE;
    }
    
    return FALSE;
}

int main()
{
    Vect2D_u16 resolution;
    resolution.x = VDP_getScreenWidth();
    resolution.y = VDP_getScreenHeight();

    u16 xPosStrLen = 0;
    u16 yPosStrLen = 0;

    u16 score = 0;

    Vect2D_u16 snakeLastPosition;
    snakeLastPosition.x = 0;
    snakeLastPosition.y = 0;

    Vect2D_u16 snakeHead;
    snakeHead.x = (screenWidth / 2) / 8 * 8;
    snakeHead.y = (screenHeight / 2) / 8 * 8;

    Vect2D_u16 fruit;
    fruit.x = (random() % screenWidth) / 8 * 8;
    fruit.y = (random() % screenHeight) / 8 * 8;

    /*
    Colour tricks from Tiago
           R     G     B
    [xxxx][***x][***x][***x]
    */

    VDP_setPaletteColor(0, 0x2a2);
    //VDP_setBackgroundColor(37);

	JOY_setEventHandler(&myJoyHandler);

	//load the tile in VRAM (check it using GensKMod CPU>Debug>Genesis>VDP)
	VDP_loadTileData((const u32 *)tileSnake, TILE_USERINDEX, 1, CPU);
    VDP_loadTileData((const u32 *)tileFruit, TILE_USERINDEX + 1, 1, CPU);

    VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE_USERINDEX + 1), fruit.x / 8, fruit.y / 8);


	//VDP_fillTileMapRect(PLAN_A, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX), 0, 0, 64, 64);

	while (TRUE)
	{
        //read input
        steerSnake(&snakeHead);

		//move sprite
        updateSnakePosition(&snakeHead, &resolution);

        //check collision
        if (isSnakeEatFruit(&snakeHead, &fruit))
        {
            fruit.x = (random() % resolution.x) / 8 * 8;
            fruit.y = (random() % resolution.y) / 8 * 8;

            VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE_USERINDEX + 1), fruit.x / 8, fruit.y / 8);
            
            //update score
            ++score;
        }

        //update sprites

        VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, 0), snakeLastPosition.x, snakeLastPosition.y);
        VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL3, 1, 0, 0, TILE_USERINDEX), snakeHead.x / 8, snakeHead.y / 8);

        snakeLastPosition.x = snakeHead.x / 8;
        snakeLastPosition.y = snakeHead.y / 8;

		//draw current screen (logo, start screen, settings, game, gameover, credits...)
        char scoreStr[10];
        int16ToStr(score, scoreStr, 0);
        VDP_drawText("Score:", 20, 0);
        VDP_drawText(scoreStr, 30, 0);
        
        char xPosStr[5], yPosStr[5];
        xPosStr[0] = yPosStr[0] = ' ';
        xPosStr[1] = yPosStr[1] = ' ';
        xPosStr[2] = yPosStr[2] = ' ';
        xPosStr[3] = yPosStr[3] = ' ';
        xPosStr[4] = yPosStr[4] = 0;

        xPosStrLen = int16ToStr(snakeHead.x, xPosStr, 0);
        if (snakeHead.x < 0) ++xPosStrLen; // temporary fix due to int16ToStr returning only size of number
        yPosStrLen = int16ToStr(snakeHead.y, yPosStr, 0);
        if (snakeHead.y < 0) ++yPosStrLen;
        xPosStr[xPosStrLen] = ' ';
        yPosStr[yPosStrLen] = ' ';

        VDP_drawText(xPosStr, 10, 0);
        VDP_drawText(yPosStr, 10, 1);

        char steerStr[2];
        uintToStr(steer, steerStr, 1);

		VDP_drawText(steerStr, 10, 2);

		//wait for screen refresh
		VDP_waitVSync();	
	}
	return (0);
}