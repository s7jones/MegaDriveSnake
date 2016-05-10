#include <genesis.h>

//moon.h is generated from moon.bmp
//just be sure to put your 16colors bitmap on 'res' folder
// file logo-blue-moon-25024.jpg is here for reference : it's the original picture I used

#include "moon.h"

#define TILE1	1

void myJoyHandler(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		if (state & BUTTON_START)
		{
			//player1 press start
			VDP_drawText("Start", 0, 0);
		}
		else if (changed & BUTTON_START)
		{
			//player1 release start
		}
	}
}

const u32 tile[8] =
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

const u32 spriteTiles[4 * 8] =
{
	0x00001111, //Tile Top Left
	0x00001111,
	// ...

	0x11112222, //Tile Bottom Left
	0x11112222,
	// ...

	0x11110000, //Tile Top Right
	0x11110000,
	// ...

	0x22221111, //Tile Bottom Right
	0x22221111,
	// ...

};

int main()
{

	JOY_init();
	JOY_setEventHandler(&myJoyHandler);

	VDP_drawText("Hello Genny World!", 10, 13);

	//This will fill the background with this tile
	//VDP_loadTileData( (const u32 *)tile, 0, 1, 0);

	//load the tile in VRAM (check it using GensKMod CPU>Debug>Genesis>VDP)
	VDP_loadTileData((const u32 *)tile, TILE1, 1, 0);

	//draw this tile at (5,5) using PAL0
	VDP_setTileMapXY(APLAN, TILE1, 5, 5);


	//draw this tile flipped on B plane at (6,5) using PAL2 (default : green gradient)
	//PAL2 = green pal
	// 0 = low priority
	// 1 = vflip
	// 0 = no hflip
	// 1 = tile 1
	VDP_setTileMapXY(BPLAN, TILE_ATTR_FULL(PAL2, 0, 1, 0, TILE1), 6, 5);

	//tile at same position with 2 pal...which one is in front ? the one with field priority 1
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE1), 7, 7);
	VDP_setTileMapXY(BPLAN, TILE_ATTR_FULL(PAL2, 0, 0, 0, TILE1), 7, 7);
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL1, 0, 0, 0, TILE1), 8, 7);
	VDP_setTileMapXY(BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, TILE1), 8, 7);

	//same priority, APLAN first
	//write order doesn't define the priority
	VDP_setTileMapXY(BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, TILE1), 9, 7);
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE1), 9, 7);
	VDP_setTileMapXY(BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, TILE1), 9, 7);

	//fill a square filled of tile 1 in blue => 1 tile in memory, 64 on screen
	VDP_fillTileMapRect(BPLAN, TILE_ATTR_FULL(PAL3, 0, 0, 0, TILE1), 12, 12, 8, 8);

	// get the image width (in pixel) ==> should be 8pix aligned
	//u16 w = moon[0];
	u16 w = moon.w;
	// get the image height (in pixel)  ==> should be 8px aligned
	//u16 h = moon[1];
	u16 h = moon.h;

	// get the palette at moon[2 to 17]
	//VDP_setPalette(PAL1, (u16 *)&moon[2]);
	//const Palette pal1 = *(moon.palette);
	//const u16 *palpointer = pal1.data;

	//VDP_setPalette(PAL1, pal1.data);
	VDP_setPalette(PAL3, moon.palette->data);

	// load bitmap data at moon[18....] in VRAM
	// w/8 = width in tiles we want to load
	// h/8 = height in tile we want to load
	// w/8 = width in tiles of the bitamp
	// the 3rd arg is needed because you could load only a part of the bitmap if you want but SGDK needs the width as reference
	//VDP_loadBMPTileData((u32*)&moon[18], TILE1, w / 8, h / 8, w / 8);
	VDP_loadBMPTileData(moon.image, 2, w / 8, h / 8, w / 8);

	// draw the moon at (12,12)
	VDP_fillTileMapRectInc(BPLAN, TILE_ATTR_FULL(PAL3, 0, 0, 0, 2), 12, 12, w / 8, h / 8);

	//load the tiles on VRAM
	VDP_loadTileData((const u32 *)spriteTiles, 10, 4, 0);

	//load its pal (if not already loaded)
	//we'll use one of the pre-loaded pal for now

	//define it
	// arg0 : sprite idx (from 0 to 79)
	// arg1 : x
	// arg2 : y
	// arg3 : size (from 1x1 to 4x4 tiles)
	// arg4 : tile(s) attributes
	// arg5 : link property (more on this later)
	VDP_setSprite(0, 40, 40, SPRITE_SIZE(2, 2), TILE_ATTR_FULL(PAL0, 1, 0, 0, 10), 0);

	// ask for draw
	VDP_updateSprites();

	SpriteDef mySprite;

	mySprite.posx = 0;
	mySprite.posy = 0;
	mySprite.size = SPRITE_SIZE(2, 2);
	mySprite.tile_attr = TILE_ATTR_FULL(PAL0, 1, 0, 0, TILE1);
	mySprite.link = 0;
	VDP_setSpriteP(0, &mySprite);

	while (1)
	{
		mySprite.posx++;
		mySprite.posy++;
		VDP_setSpriteP(0, &mySprite);

		VDP_updateSprites();

		VDP_waitVSync();

		//psycho fx! (pause/continue with Esc key on Gens KMod to see it step by step)
		VDP_fillTileMapRect(BPLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE1), 12, 1, 4, 4);
		VDP_waitVSync(); //50Hz is too fast ;)
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_fillTileMapRect(BPLAN, TILE_ATTR_FULL(PAL1, 0, 1, 0, TILE1), 12, 1, 4, 4);
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_fillTileMapRect(BPLAN, TILE_ATTR_FULL(PAL2, 0, 0, 0, TILE1), 12, 1, 4, 4);
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_fillTileMapRect(BPLAN, TILE_ATTR_FULL(PAL3, 0, 1, 0, TILE1), 12, 1, 4, 4);
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_waitVSync();
		VDP_waitVSync();

		//read input
		//move sprite
		//update score
		//draw current screen (logo, start screen, settings, game, gameover, credits...)

		//wait for screen refresh
		VDP_waitVSync();
	}
	return (0);
}