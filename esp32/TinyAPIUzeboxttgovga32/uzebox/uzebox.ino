//Author: ackerman
//Port API Uzebox emulator to TTGO VGA32 by ackerman:
// VGA 320x240
// Mode 64 colors
// VGA Tiny library bitluni 0.3.3 (include) Ricardo Massaro
// Visual Studio 1.66.1 PLATFORMIO 2.4.3 Espressif32 v3.5.0 (python 3.6)
// TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)
// No PSRAM
// Arduino IDE (not compile). Only Visual Studio.
// Unstable proof of concept
// gbConfig options configuration compile
#include <Ticker.h>
#include "PS2Kbd.h"
//#include <Arduino.h>
//Para ahorrar memoria
//JJ #include <esp_bt.h>

#include "gbConfig.h"
#include "gbGlobals.h"
#include <time.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>


#include "uzebox.h"
//#include "MartianVGA.h"
#include "hardware.h"
#include "driver/timer.h"
#include "soc/timer_group_struct.h"

//#ifndef use_lib_tinyFont
// #include "ESP32Lib/Ressources/Font6x8.h"
//#endif 

#include "gbGlobals.h"
#include "osd.h"
//#include "BitmapDemo.h"

#ifdef use_rom_tutorial
 #include "demos/tutorial/tutorial.h" 
#endif 

#ifdef use_rom_controllertester
 #include "demos/ControllerTester/ControllerTester.h"
#endif 

#ifdef use_rom_bitmapdemo
 #include "demos/BitmapDemo/BitmapDemo.h"
#endif 

#ifdef use_rom_chess4uzebox
 #include "demos/chess4uzebox/chess4uzebox.h"
#endif

#ifdef use_rom_megasokoban
 #include "demos/MegaSokoban/MegaSokoban.h"
#endif

#ifdef use_rom_spritedemo
 #include "demos/SpriteDemo/SpriteDemo.h"
#endif

#ifdef use_rom_arkanoid
 #include "demos/Arkanoid/Arkanoid.h"
#endif


#include "vga_6bit.h"

#ifdef use_lib_vga8colors
  //DAC 3 bits 8 colores
  // 3 bit pins  
 static const unsigned char pin_config[] = {  
  PIN_RED_HIGH,
  PIN_GREEN_HIGH,  
  PIN_BLUE_HIGH,
  255,
  255,
  255,
  PIN_HSYNC,
  PIN_VSYNC
 };
#else
 //DAC 6 bits 64 colores
 static const unsigned char pin_config[] = {
  PIN_RED_LOW,
  PIN_RED_HIGH,
  PIN_GREEN_LOW,
  PIN_GREEN_HIGH,
  PIN_BLUE_LOW,
  PIN_BLUE_HIGH,
  PIN_HSYNC,
  PIN_VSYNC
 };
#endif


volatile unsigned char gb_speed_turbo=0;
volatile unsigned char gb_draw_timer=1;
volatile unsigned int gb_cont_function_timer=0;


unsigned char gb_sync_bits;
unsigned char ** gb_buffer_vga;
unsigned int **gb_buffer_vga32;
unsigned char gb_cache_color[256];


volatile unsigned char gb_load_new_rom=1;
//unsigned char gb_id_cur_rom=4;
volatile unsigned char gb_id_cur_rom=0;

Ticker gb_ticker_callback;

unsigned char gb_keepalive=0;
unsigned char gb_timer_ms=20; //20 ms
volatile unsigned int gb_vsync_fps=0;
const char *gb_uze_font;

//int gb_done = 0;
int gb_currentTime,gb_vgaTime,gb_keyboardTime; //Tiempo ms vga
int gb_jj_cur_cycle=0;
int gb_jj_ini_cycle=0;
int gb_jj_cur_fps=0;
int gb_jj_ini_fps=0;
int gb_jj_time_ini_fps=0;
unsigned char gb_uze_video_cur_mode=0; //Modo de video actual


volatile int gb_done = 0;
//int gb_currentTime,gb_vgaTime,gb_keyboardTime; //Tiempo ms vga

int gb_left, gb_now,gb_vga_time_prev;

int inicio_fps,fin_fps;
int ini_cont_fps,fin_cont_fps;

//#ifdef COLOR_3B
// #ifdef use_lib_vga_low_memory
//  VGA3BitI vga; 
// #else
//  VGA3Bit vga;
// #endif 
//#else
// #ifdef use_lib_vga_low_memory
//  VGA6BitI vga;
// #else
//  VGA6Bit vga;
// #endif
//#endif



unsigned char gb_current_ms_poll_keyboard = gb_ms_keyboard;
static unsigned long gb_time_ini_espera;

//volatile unsigned char keymap[256];
//volatile unsigned char oldKeymap[256];

//#ifdef use_lib_ultrafast_vga
// unsigned char ** ptrVGA;
//#endif

//unsigned char ** ptrVGA;

//JJ unsigned char gb_sdl_blit=0;//solo necesito en pc

void SDL_keys_poll()
{
}

void PrepareColorsUltraFastVGA()
{
 //(color & RGBAXMask) | SBits;
 unsigned char b,g,r;
 #ifdef use_lib_vga8colors
  for (unsigned char i=0;i<256;i++){   
   gb_cache_color[i]= (i & 0x07) | gb_sync_bits;
  }
 #else
  for (unsigned int i=0;i<256;i++)
  {//DAC 6 bits 64 colores   
   //gb_cache_color[cont]= (i & 0x3F) | gb_sync_bits;

   b= ((i>>6)&0x03);
   g= (((((i>>3)&0x07))>>1 ) & 0x03 );
   r= (((i & 0x07)>>1 ) & 0x03);
   gb_cache_color[i]= ((b<<4)|(g<<2)|r) | gb_sync_bits;      
   //cont4++;
  // if (cont4 >= 4)
  // {
  //   cont4=0;
  //   cont++;
  // }
   //gb_colors[cont].b = ((i>>6)&0x03)*85; //0..3
   //gb_colors[cont].g = (((((i>>3)&0x07))>>1 ) & 0x03 )*85; //0..3
   //gb_colors[cont].r = (((i & 0x07)>>1 ) & 0x03) * 85 ; //0..3

  }
 #endif  
}


void setup()
{
 //DO NOT turn off peripherals to recover some memory
 //esp_bt_controller_deinit(); //Reduzco consumo RAM
 //esp_bt_controller_mem_release(ESP_BT_MODE_BTDM); //Reduzco consumo RAM

 #ifdef use_lib_log_serial
  Serial.begin(115200);         
  Serial.printf("HEAP BEGIN %d\n", ESP.getFreeHeap()); 
#endif

 #ifdef use_lib_log_serial
  Serial.printf("RAM %d\n", ESP.getFreeHeap()); 
 #endif

 #ifdef use_lib_vga8colors
  #ifdef use_lib_400x300
   //vga.init(vga.MODE400x300, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);       
  #else
   //vga.init(vga.MODE320x200, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);    
  #endif
 #else
  //const int redPins[] = {RED_PINS_6B};
  //const int grePins[] = {GRE_PINS_6B};
  //const int bluPins[] = {BLU_PINS_6B}; 
  /*#ifdef use_lib_400x300
   vga.init(vga.MODE400x300, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);   
  #else
   vga.init(vga.MODE320x200, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
  #endif 
  */
  //#ifdef usb_lib_video_mode_8
  // vga.init(vga.MODE200x150, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
  //#else 
   //Funciona vga.init(vga.MODE320x240, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);   
  //#endif 
 #endif

 vga_init(pin_config,VgaMode_vga_mode_320x240,false); //Llamada en C

 gb_sync_bits= vga_get_sync_bits();
 gb_buffer_vga = vga_get_framebuffer();
 gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
 PrepareColorsUltraFastVGA(); //Llamar despues de tener gb_sync_bits
 SDLClear();
 vTaskDelay(20);
 //ptrVGA = vga.backBuffer; //Asignamos  puntero buffer
 //Serial.printf("RGBAXMask:%d SBits:%d\n",vga.RGBAXMask,vga.SBits);
 
 //#ifndef use_lib_tinyFont
 // vga.setFont(Font6x8);
 //#endif 
 //vga.clear(BLACK); 
 //vga.fillRect(10,10,100,50,WHITE);
 /*#ifdef use_lib_ultrafast_vga
  #ifdef use_lib_400x300
   vga.fillRect(0,0,400,300,BLACK);
   vga.fillRect(0,0,400,300,BLACK);//fix mode fast video
  #else
   vga.fillRect(0,0,320,200,BLACK);
   vga.fillRect(0,0,320,200,BLACK);//fix mode fast video
  #endif 
  ptrVGA = vga.backBuffer; //Asignamos  puntero buffer
 #endif
 */

 #ifdef use_lib_log_serial
  Serial.printf("VGA %d\n", ESP.getFreeHeap()); 
 #endif

 kb_begin();

 gb_keyboardTime = gb_currentTime = millis();

 #ifdef use_lib_log_serial  
  Serial.printf("END SETUP %d\n", ESP.getFreeHeap()); 
 #endif
 inicio_fps = millis();
 //doSetupBitmapDemo();
 gb_ticker_callback.attach(0.02,my_callbackfunc);
}


//void sdl_fast_putpixel(int x,int y, unsigned char aColor)
//{
// //vga.dotFast(x,y,aColor);
// gb_buffer_vga[y][x^2]= gb_cache_color[aColor];
//}


//************************************
void ProcessScanLineVideo5(unsigned char dest_x, unsigned char dest_y, unsigned char aColor)
{
 //printf("Y:%d X:%d ini:%d Lines:%d\n",dest_y,dest_x,gb_uze_firstScanlineToRender,gb_uze_verticalLinesToRender);
 if ((gb_uze_firstScanlineToRender<=20)&&(gb_uze_verticalLinesToRender>=224))
 {
  //gb_buffer_vga[dest_y][dest_x]= aColor; //Dibuja normalmente
  sdl_fast_putpixel(dest_x,dest_y,aColor);
 }
 else
 {
  if (dest_y < gb_uze_verticalLinesToRender)
  {
//   gb_buffer_vga[(dest_y+gb_uze_firstScanlineToRender)][dest_x]= aColor;
   sdl_fast_putpixel(dest_x,(dest_y+gb_uze_firstScanlineToRender),aColor);
  }
  else
  {
   if ((dest_y < gb_uze_firstScanlineToRender) || (dest_y > (gb_uze_firstScanlineToRender+gb_uze_verticalLinesToRender)))
   {
//    gb_buffer_vga[dest_y][dest_x]= 0;
sdl_fast_putpixel(dest_x,dest_y,0);
    //gb_buffer_vga[dest_y][dest_x+240]= aColor;
   }
  }
 }
}

//************************************
void SDLDumpVideo5()
{
 unsigned char dato,colorDato;
 int contVram=0;
 int auxX=0;
 int finalX,finalY;
 int xOfs=40;
 int yOfs=10;     
 char *ptr = (char *)vram;
 int offsTile=0;
 unsigned char idTile;
 unsigned char datoRGB;
 int dest_y,dest_x;
 unsigned int calc_y,calc_x;
 //240x224 resolution, 6x8 pixels tiles, 40x28 vram
 
 if (gb_uze_cur_TileTable == NULL)
 {
  return;
 }
 
 calc_y= 0;
 calc_x= 0;
 for (int j=0;j<28;j++)
 {     
  calc_y= calc_y+8; //j*8
  calc_x=0;
  for (int i=0;i<40;i++)
  {
   calc_x= calc_x+6; //i*6
      
   idTile= vram[(j*40)+i];

   offsTile= (int)(idTile)*48;
      
   for (int jTile=0;jTile<8;jTile++)
   {
    for (int iTile=0;iTile<6;iTile++)
    {
     datoRGB= gb_uze_cur_TileTable[offsTile];
     offsTile++;
     
     //dest_y = (j*8) + jTile;
     //dest_x = (i*6) + iTile;
     dest_y = calc_y + jTile;
     dest_x = calc_x + iTile;
     
     ProcessScanLineVideo5(dest_x, dest_y, datoRGB);
     //gb_buffer_vga[dest_y][dest_x]= datoRGB;     
    }
   }   
  }
 }
}


void SDLDumpVideo10()
{
 //192x192 resolution, 12x16 pixels tiles, 16x12 vram with black border. 192 bytes per tile
 unsigned char dato,colorDato;
 int contVram=0;
 int auxX=0;
 int finalX,finalY;     
 char *ptr = (char *)vram;
 int offsTile=0;
 unsigned char idTile;
 unsigned char datoRGB;
 int dest_y,dest_x;
 unsigned int calc_y,calc_x;
 
 if (gb_uze_cur_TileTable == NULL)
 {
  return;
 }
 
 calc_y= 0;
 calc_x= 0;
 for (int j=0;j<12;j++)
 {     
  calc_y= calc_y+16; //j*16
  calc_x=0;
  for (int i=0;i<16;i++)
  {
   calc_x= calc_x+12; //i*12
      
   idTile= vram[(j*16)+i];

   offsTile= (int)(idTile)*192;
      
   for (int jTile=0;jTile<16;jTile++)
   {
    for (int iTile=0;iTile<12;iTile++)
    {
     datoRGB= gb_uze_cur_TileTable[offsTile];
     offsTile++;
     
     //dest_y = (j*8) + jTile;
     //dest_x = (i*6) + iTile;
     dest_y = calc_y + jTile;
     dest_x = calc_x + iTile;
     
     ProcessScanLineVideo5(dest_x, dest_y, datoRGB);
     //gb_buffer_vga[dest_y][dest_x]= datoRGB;     
    }
   }   
  }
 }
}




//************************************
void SDLvideoTask()
{
 unsigned char dato,colorDato;
 int contVram=0;
 int auxX=0;
 int finalX,finalY;
 int xOfs=40;
 int yOfs=10;
 
 //char **ptr = (char **)vram;
 char *ptr = (char *)vram;
 int contTile=0;
 
 switch (gb_uze_video_cur_mode)
 {
  case 1: break;
   
  case 5: SDLDumpVideo5(); break;
  case 10: SDLDumpVideo10(); break;
  
  case 8:
   unsigned char auxPalette[4];
   for (int i=0;i<4;i++)
   {
    unsigned char aRed= (palette[i]&0x07); //No lo necesitamos
    unsigned char aGreen= ((palette[i]>>3)&0x07); //No lo necesitamos
    unsigned char aBlue= ((palette[i]>>6)&0x03); //No lo necesitamos
    //printf("%d 0x%02X R:%d G:%d B:%d\n",i,palette[i],aRed,aGreen,aBlue);
//    aRed = aRed>>1;
//    aGreen = aGreen>>1;
//    auxPalette[i]= (aBlue<<4)|(aGreen<<2)|aRed; //64 colores lo hace mal
    //auxPalette[i]= (aBlue<<6)|(aGreen<<3)|aRed; //Formato VGA 256 colores bien
    auxPalette[i]= palette[i]; //BBGGGRRR
   }
   //fflush(stdout); 
   for (int y=0;y<96;y++)
   {
    auxX=0;
    for(int x=0;x<30;x++)
    {
     dato = vram[contVram];
     for(int i=6;i>=0;i-=2)
     {
      colorDato = ((dato>>i) & 0x03);
      colorDato = auxPalette[colorDato];
      //finalX = (auxX+xOfs)<<1;
      finalX = (auxX+0)<<1;
      finalY= (y+yOfs)<<1;
      sdl_fast_putpixel(finalX,finalY,colorDato);
      sdl_fast_putpixel(finalX+1,finalY,colorDato);
      sdl_fast_putpixel(finalX,finalY+1,colorDato);
      sdl_fast_putpixel(finalX+1,finalY+1,colorDato);

      //gb_buffer_vga[finalY][finalX]= colorDato;
      //gb_buffer_vga[finalY][finalX+1]= colorDato;
      //gb_buffer_vga[finalY+1][finalX]= colorDato;
      //gb_buffer_vga[finalY+1][finalX+1]= colorDato;
      
      auxX++;
     }
     contVram++; //11001100
    }
   }
   break;
 }

/*  
 volatile unsigned char *ptr;
 unsigned char dato,colorDato;
 int contVram=0;
 int auxX=0;
 int finalX,finalY;
 int xOfs=0;
 int yOfs=0;
 switch (gb_uze_video_cur_mode)
 {
  case 8:
   unsigned char auxPalette[4];
   for (int i=0;i<4;i++)
   {
    unsigned char aRed= (palette[i]&0x07);
    unsigned char aGreen= ((palette[i]>>3)&0x07);
    unsigned char aBlue= ((palette[i]>>6)&0x03);
    //printf("%d 0x%02X R:%d G:%d B:%d\n",i,palette[i],aRed,aGreen,aBlue);
    aRed = aRed>>1;
    aGreen = aGreen>>1;
    auxPalette[i]= (aBlue<<4)|(aGreen<<2)|aRed;
   }
   //fflush(stdout); 
   ptr= vram;   
   for (int y=0;y<96;y++)
   {
    auxX=0;
    for(int x=0;x<30;x++)
    {
     dato = ptr[contVram];
     for(int i=6;i>=0;i-=2)
     {
      colorDato = ((dato>>i) & 0x03);
      colorDato = auxPalette[colorDato];
      finalX = (auxX+xOfs)<<1;
      finalY= (y+yOfs)<<1;
      sdl_fast_putpixel(finalX,finalY,colorDato);
      sdl_fast_putpixel(finalX+1,finalY,colorDato);
      sdl_fast_putpixel(finalX,finalY+1,colorDato);
      sdl_fast_putpixel(finalX+1,finalY+1,colorDato);
      auxX++;
     }
     contVram++; //11001100
    }
   }
   break;
 }
 */
}

//************************
void my_callbackfunc()
{
 gb_cont_function_timer++;
 gb_vsync_fps++;
 
 if (gb_cont_function_timer>1)
 {//Entre mas de 1 vez
  gb_cont_function_timer--;
  return; 
 }

 if (gb_draw_timer == 0){
  gb_cont_function_timer--;   
  return;
 }

 SDLvideoTask();

// SDLvideoTask();  
// gb_vsync_fps++;
 //Serial.printf("%d\n", gb_vsync_fps);
 gb_cont_function_timer--;
}

// +-------------+
// | LOOP core 1 |
// +-------------+
//
void loop()
{
 //doBitmapDemoMain();
 //my_callbackfunc();

 if (gb_load_new_rom==1)
 {
  gb_load_new_rom= 0;
  #ifdef use_lib_log_serial
   Serial.printf("Load new rom id:%d\n",gb_id_cur_rom);
  #endif
  gb_draw_timer=0;
  switch(gb_id_cur_rom)
  {
   case 0: do_Tutorial_Setup(); break;
   case 1: do_ControllerTester_Setup(); break;
   case 2: do_BitmapDemo_Setup(); break;
   case 3: do_chess4uzebox_Setup(); break;
   case 4: do_megasokoban_Setup(); break;
   default: break;
  }  
  //delay(1000);//Esperamos por sincronizacion    
  WaitVsync(50);
  gb_draw_timer=1;
 }

 if (gb_load_new_rom == 0)
 {
  #ifdef use_rom_tutorial
   if (gb_id_cur_rom==0){
    do_Tutorial_main();
   }
  #endif
  #ifdef use_rom_controllertester 
   if (gb_id_cur_rom==1){
    do_ControllerTester_main();
   }
  #endif 
  #ifdef use_rom_bitmapdemo        
   if (gb_id_cur_rom==2){
    do_BitmapDemo_main();
   }
  #endif 
  #ifdef use_rom_chess4uzebox
   if (gb_id_cur_rom==3){
    do_chess4uzebox_main();
   }
  #endif 
  #ifdef use_rom_megasokoban
   if (gb_id_cur_rom==4){
    do_megasokoban_main();
   }
  #endif 
 }

 //uze_pgm_read_byte uze_Print
}
