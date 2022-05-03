#ifndef OSDFILE_H
 #define OSDFILE_H

 void SDLClear(void);
 
 void do_tinyOSD(void);
 void SDLActivarOSDMainMenu();
 void SDLOSDClear(void);
 //void SDLEsperaTeclado(void); 
 void jj_fast_putpixel(int x,int y,unsigned char c);
 //void ResetSound(void);

 unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax);
 
#endif
