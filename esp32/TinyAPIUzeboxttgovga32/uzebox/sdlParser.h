#ifndef _GB_SDLPARSER_H
 #define _GB_SDLPARSER_H
 #include "gbConfig.h"
 #include "gbGlobals.h"
 
 void LiberarRecursos(void); 
 void sdl_fast_putpixel(unsigned int x, unsigned int y, unsigned int c);
 //void sdl_fast_clear(unsigned int c);
 void sdl_do_key_poll(void);
 void sdl_dump_bitluni(void);
 
 void SDL_print_char(int x,int y,int w,int h,int byteBlock,unsigned char idLetra);
#endif
