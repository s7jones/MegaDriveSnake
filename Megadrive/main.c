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

void steerSnake(VDPSprite *snakeSprite)
{
	JOY_update();
	//if (steer==direction)
	
	if (count > 5) {
		switch (steer) {
		case up:
			snakeSprite->y -= 8;
			direction = up;
			break;
		case down:
			snakeSprite->y += 8;
			direction = down;
			break;
		case left:
			snakeSprite->x -= 8;
			direction = left;
			break;
		case right:
			snakeSprite->x += 8;
			direction = right;
			break;
		}
		count = 0;
	}
	else count++;

}

VDPSprite spawnFruit()
{
    
}

int main()
{
	JOY_setEventHandler(&myJoyHandler);

	//load the tile in VRAM (check it using GensKMod CPU>Debug>Genesis>VDP)
	VDP_loadTileData((const u32 *)tileSnake, TILE_USERINDEX, 1, CPU);
    VDP_loadTileData((const u32 *)tileFruit, TILE_USERINDEX + 1, 1, CPU);

	VDPSprite snakeSprite;
	snakeSprite.x = screenWidth / 2;
	snakeSprite.y = screenHeight / 2;
	snakeSprite.size = SPRITE_SIZE(1, 1);
	snakeSprite.attribut = TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE_USERINDEX);
	snakeSprite.link = 1;

    VDPSprite fruitSprite;
    fruitSprite.x = 8; // off-screen
    fruitSprite.y = 8;
    fruitSprite.size = SPRITE_SIZE(1, 1);
    fruitSprite.attribut = TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE_USERINDEX + 1);
    fruitSprite.link = 0;

    VDP_setSpriteFull(0, snakeSprite.x, snakeSprite.y, snakeSprite.size, snakeSprite.attribut, snakeSprite.link);
    VDP_setSpriteFull(1, fruitSprite.x, fruitSprite.y, fruitSprite.size, fruitSprite.attribut, fruitSprite.link);


	VDP_fillTileMapRect(PLAN_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, TILE_USERINDEX), 0, 0, 64, 64);

    u16 xPosStrLen = 0;
    u16 yPosStrLen = 0;

	while (1)
	{
        //spawn random fruit
        //if (!isFruitAlive)
        //{
        //    VDPSprite fruitSprite = spawnFruit();
        //    VDP_setSprite(1, )
        //}

        //read input
        steerSnake(&snakeSprite);

		//move sprite
        VDP_setSpritePosition(0, snakeSprite.x, snakeSprite.y);
        VDP_updateSprites(2, TRUE);

		//update score

		//draw current screen (logo, start screen, settings, game, gameover, credits...)
        char xPosStr[4], yPosStr[4];

        VDP_clearText(10, 0, xPosStrLen);
        VDP_clearText(10, 1, yPosStrLen);

        xPosStrLen = int16ToStr(snakeSprite.x, xPosStr, 0);
        if (snakeSprite.x < 0) ++xPosStrLen; // temporary fix due to int16ToStr returning only size of number
        yPosStrLen = int16ToStr(snakeSprite.y, yPosStr, 0);
        if (snakeSprite.y < 0) ++yPosStrLen;

        VDP_drawText(xPosStr, 10, 0);
        VDP_drawText(yPosStr, 10, 1);

        char steerStr[4];
        uintToStr(steer, steerStr, 1);

		VDP_drawText(steerStr, 10, 2);

		//wait for screen refresh
		VDP_waitVSync();	
	}
	return (0);
}