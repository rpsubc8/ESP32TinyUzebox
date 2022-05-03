/*
 *  Uzebox quick and dirty tutorial
 *  Copyright (C) 2008  Alec Bourque
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../gbConfig.h"


#ifdef use_rom_tutorial



#include <stdbool.h>
//JJ #include <avr/io.h>
#include <stdlib.h>
//JJ #include <avr/pgmspace.h>
#include "../../uzebox.h"
#include "../../gbGlobals.h"


//JJ #define TILE_WIDTH 8
//JJ #define TILE_HEIGHT 8
//JJ #define SCREEN_TILES_H 40
//JJ #define SCREEN_TILES_V 18



//TILE_WIDTH= 8;
//TILE_HEIGHT= 8;

//Import the appropriate font file
//based on the video mode's tile height
//and width. These values can be overriden
//in the makefile.
//JJ #if TILE_WIDTH == 6 && TILE_HEIGHT == 8
//JJ 	#include "data/font-6x8-full.inc"

//JJ #elif TILE_WIDTH == 6 && TILE_HEIGHT == 12
//JJ 	#include "data/font-6x12-full.inc"

//JJ #elif TILE_WIDTH == 8 && TILE_HEIGHT == 8
	#include "data/font-8x8-full.inc"
//JJ #else
//JJ 	#error Unsupported tile size defined in makefile! For this demo only 6x8, 6x12 and 8x8 tile sizes are supported.
//JJ #endif

void do_Tutorial_Setup()
{
 ClearScreen(); //Borro SDL surface
 
 gb_uze_cur_FontTilesIndex=0;
 gb_uze_cur_TileTable= NULL; //Para el fondo
 gb_uze_cur_SpritesTileTable= NULL;
 gb_uze_font= NULL; 

 TILE_WIDTH = 8;
 TILE_HEIGHT = 8;
 SCREEN_TILES_H = 40;
 SCREEN_TILES_V = 18;
 
 gb_uze_video_cur_mode=1; //Modo de video 8 forzamos 
 SetVideoMode(gb_uze_video_cur_mode); //Forzamos SCREEN_WIDTH y HEIGHT
   
 //Clear the screen (fills the vram with tile zero)
 ClearVram();
 
 //printf("do_Tutorial_Setup\n");
 //fflush(stdout);
 WaitVsync(50);//Esperamos por sincronizacion    
}


int do_Tutorial_main(){

	//Set the font and tiles to use.
	//Always invoke before any ClearVram()
	SetFontTable(font);

	//Clear the screen (fills the vram with tile zero)
	ClearVram();

	//Print column and rows on screen border
	for(u8 x=0;x<SCREEN_TILES_H;x++){
		PrintChar(x,0,'0'+(x%10));
	}
	for(u8 y=0;y<SCREEN_TILES_V;y++){
		PrintChar(0,y,'0'+(y%10));
	}

	//Prints a string on the screen. Note that PSTR() is a macro 
	//that tells the compiler to store the string in flash.
	//14 is half the lenght of the "Hello world" string
	uze_Print((SCREEN_TILES_H/2)-14, (SCREEN_TILES_V/2)-1, PSTR("Hello World From The Uzebox!"));

    //JJ while(1);
	
	while(1)
	{
	 WaitVsync(1); //Se lo meto yo
	 
     MACRO_UZEBOX_BREAK_MAIN_RETURN //Rompemos bucle para cargar otro juego
    }//fin while

} 

#endif

