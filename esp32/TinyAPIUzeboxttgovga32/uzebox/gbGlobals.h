#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H
 #include "gbConfig.h"
 #include "uzebox.h"
 #include <stdio.h>
 
 extern volatile unsigned char keymap[256];
 extern volatile unsigned char oldKeymap[256];

 extern volatile unsigned char gb_show_osd_main_menu;
 extern volatile unsigned char gb_speed_turbo;
 extern volatile unsigned char gb_draw_timer;
 extern volatile unsigned int gb_cont_function_timer;

 extern unsigned char ** gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32;
 extern unsigned char gb_sync_bits;

 extern volatile int gb_done;
 extern int gb_currentTime,gb_vgaTime,gb_keyboardTime; //Tiempo ms vga

 extern unsigned char gb_cache_color[256];


 //Global UZEBOX
 extern unsigned char palette[256];
 extern volatile int SCREEN_WIDTH;
 extern volatile int SCREEN_HEIGHT;
 extern volatile unsigned char vram[0x4000]; //16 KB de RAM
 //extern volatile unsigned char vram[0x1000]; //4 KB RAM
 extern unsigned char gb_uze_video_cur_mode; //Modo de video actual
 extern unsigned char gb_timer_ms;
 extern volatile unsigned int gb_vsync_fps;

 extern int X_SCROLL_WRAP;
 extern int VRAM_TILES_H;
 extern int VRAM_TILES_V;
 extern int TILE_WIDTH; //X_SCROLL_WRAP= VRAM_TILES_H*TILE_WIDTH
 extern int TILE_HEIGHT;
 extern int MAX_SPRITES;
 extern int SCREEN_TILES_V;
 extern int SCREEN_TILES_H;
 extern int RAM_TILES_COUNT;
 extern int FIRST_RENDER_LINE;
 extern int FRAME_LINES;
 
 extern unsigned int gb_uze_cur_gamepad[2]; //Botones del gamepad 0 y 1
 
 extern const char * gb_uze_font;
 
 extern const unsigned char *gb_uze_cur_TileTable; //Tabla render fondo SetTileTable()
 extern const unsigned char *gb_uze_cur_SpritesTileTable; //Tabla SetSpritesTileTable (modos 2,3)
 
 extern unsigned char gb_uze_cur_FontTilesIndex;

 extern unsigned char gb_uze_firstScanlineToRender;
 extern unsigned char gb_uze_verticalLinesToRender;
 
 extern const struct PatchStruct *gb_uze_patchPointersParam;
 



 extern struct TrackStruct tracks[4];
 
 
 
 extern volatile unsigned char gbVolMixer_now[3];
 extern volatile unsigned short int gbFrecMixer_now[3];
 
 extern unsigned int gb_uze_ini_triggerfx;
 extern unsigned char gb_uze_ini_play_triggerfx;
 
// extern int TILE_WIDTH;
// extern int TILE_HEIGHT;


 extern volatile unsigned char gb_load_new_rom;
 extern volatile unsigned char gb_id_cur_rom; 

#endif

 
