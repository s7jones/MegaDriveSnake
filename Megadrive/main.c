#include <genesis.h>

//moon.h is generated from moon.bmp
//just be sure to put your 16colors bitmap on 'res' folder
// file logo-blue-moon-25024.jpg is here for reference : it's the original picture I used

#include "moon.h"

#define TILE1   1

//const u32 tile[8] =
//{
//	0x00111100,
//	0x01144110,
//	0x11244211,
//	0x11244211,
//	0x11222211,
//	0x11222211,
//	0x01122110,
//	0x00111100
//};

const u32 tile[8] =
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

const u32 tile2[8] =
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
		u32 len;
		if (state) {
			switch (state) {
			case BUTTON_UP:
				if (direction != down) {
					steer = up;
				}
				VDP_drawText("up", 0, 0);
				break;

			case BUTTON_DOWN:
				if (direction != up) {
					steer = down;
				}
				VDP_drawText("down", 0, 1);
				break;

			case BUTTON_LEFT:
				if (direction != right) {
					steer = left;
				}
				VDP_drawText("left", 0, 2);
				break;

			case BUTTON_RIGHT:
				if (direction != left) {
					steer = right;
				}
				VDP_drawText("right", 0, 3);
				break;
			}
		}
		
        if (changed) {
			switch (changed) {
			case BUTTON_UP:
				//direction = up;
				VDP_drawText("up", 0, 4);
				len = strlen("up");
				VDP_clearText(0, 0, len);
				break;

			case BUTTON_DOWN:
				//direction = down;
				VDP_drawText("down", 0, 5);
				len = strlen("down");
				VDP_clearText(0, 1, len);
				break;

			case BUTTON_LEFT:
				//direction = left;
				VDP_drawText("left", 0, 6);
				len = strlen("left");
				VDP_clearText(0, 2, len);
				break;

			case BUTTON_RIGHT:
				//direction = right;
				VDP_drawText("right", 0, 7);
				len = strlen("right");
				VDP_clearText(0, 3, len);
				break;
			}
		}

	}
}

void steerSnake(VDPSprite *snakeSprite) {
	JOY_update();
	//if (steer==direction)
	
	if (count > 5) {
		switch (steer) {
		case up:
			snakeSprite->y = snakeSprite->y - 8;
			direction = up;
			break;
		case down:
			snakeSprite->y = snakeSprite->y + 8;
			direction = down;
			break;
		case left:
			snakeSprite->x = snakeSprite->x - 8;
			direction = left;
			break;
		case right:
			snakeSprite->x = snakeSprite->x + 8;
			direction = right;
			break;
		}
		count = 0;
	}
	else count++;

}

int main()
{

	JOY_init();
	JOY_setEventHandler(&myJoyHandler);

	//load the tile in VRAM (check it using GensKMod CPU>Debug>Genesis>VDP)
	VDP_loadTileData((const u32 *)tile, TILE1, 1, 0);

	VDPSprite snakeSprite;
	snakeSprite.x = 128 + screenWidth / 2;
	snakeSprite.y = 128 + screenHeight / 2;
	snakeSprite.size = SPRITE_SIZE(1, 1);
	snakeSprite.attribut = TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE1);
	snakeSprite.link = 0;

	VDP_fillTileMapRect(PLAN_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, TILE1), 0, 0, 64, 64);

	while (1)
	{
	
		//read input
		//move sprite
		//update score
		//draw current screen (logo, start screen, settings, game, gameover, credits...)

		//snakeSprite.y--;
		steerSnake(&snakeSprite);
		VDP_setSprite(0, snakeSprite.x, snakeSprite.y, snakeSprite.size, snakeSprite.attribut);

        //VDP_setSpritePosition()

		//textofdirection = (char) direction;
		char textofdirection[1];
		//strcpy(textofdirection, "0");
		//strcpy(textofdirection, (char) direction);
		uintToStr(steer, textofdirection, 1);
		

		VDP_drawText("test000", 10, 9);
		VDP_drawText(textofdirection, 10, 10);

		VDP_updateSprites(1, TRUE);

		//wait for screen refresh
		VDP_waitVSync();
		
	}
	return (0);
}