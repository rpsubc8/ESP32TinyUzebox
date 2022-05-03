#include "gbConfig.h"
#include "uze.h"
#include "gbGlobals.h"

//unsigned char palette[256];
//unsigned char vram[0x4000];
//int SCREEN_WIDTH=256;
//int SCREEN_HEIGHT=240;

//void SetVideoMode(unsigned char aMode)
//{
// switch(aMode)
// {
//  case 8: 
//   SCREEN_WIDTH= 120;
//   SCREEN_HEIGHT= 96;
//   break;
// }
//}

//*************************************
//char jj_pgm_read_byte(const char *ptr)
//{
// return (*ptr);
//}

//void WaitVsync(int count)
//{
// int ini= gb_vsync_fps;
// int fin= ini + count;
// while (gb_vsync_fps<fin){}
//}

//**************************
//void ClearVram()
//{
// switch (gb_uze_video_cur_mode)
// {
//  case 8: memset(vram,0,2880);  
//   //sdl_fast_clear(0);
//   break;
// }
//}

//**************************
/*
void PutPixel(int x,int y,unsigned char aColor)
{
*/

/* int xOfs=40;
 int yOfs=10;
 switch (gb_uze_video_cur_mode)
 {
  case 8: 
   //auxOffset= (y*30)+((x*2)/8);
   //auxOffset += 4;//4 colores de la paleta
   //vram[auxOffset]= aColor;
   aColor = palette[(aColor & 0x03)];
   int id= (x >> 2);//X/4
   int modId= x%4;
   int auxOfs= (y*30)+id;
   unsigned char prevColor= vram[auxOfs]; //video anterior
   switch (modId)
   {
    case 0: vram[auxOfs] = (prevColor & 0x3F) | (aColor<<6); break; 
    case 1: vram[auxOfs] = (prevColor & 0xCF) | (aColor<<4); break;
    case 2: vram[auxOfs] = (prevColor & 0xF3)| (aColor<<2); break;
    case 3: vram[auxOfs] = (prevColor & 0xFC)| aColor; break;            
   }
   //sdl_fast_putpixel(x+xOfs,y+yOfs,aColor);
   break;
 }*/
 

 /*
    unsigned char color =aColor;
	if(x>=120 || y>=96 || x<0 || y<0) return;

	unsigned int addr=((SCREEN_WIDTH/4)*y)+(x>>2);
	unsigned char byte=vram[addr];
	color&=3;
	switch(x&3){
		case 3:						
			byte=(byte&~(3))|color;
			break;
		case 2:						
			byte=(byte&~(3<<2))|(color<<2);
			break;
		case 1:						
			byte=(byte&~(3<<4))|(color<<4);
			break;
		default:
			byte=(byte&~(3<<6))|(color<<6);
	}
	vram[addr]=byte;
}
*/