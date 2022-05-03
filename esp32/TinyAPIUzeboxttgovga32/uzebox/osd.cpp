#include "gbConfig.h"
#include "osd.h"
#include "gbGlobals.h"

#include "PS2Kbd.h"

#include "keys.h"
#include "hardware.h"
#include "gb_sdl_font8x8.h"
#include <string.h>


#ifdef use_lib_vga8colors
 #define ID_COLOR_BLACK 0 //Negro
 #define ID_COLOR_WHITE 7 //Blanco
 //#define ID_COLOR_MAGENTA 6 //Magenta
 #define ID_COLOR_MAGENTA 3 //Magenta
#else
// #define ID_COLOR_BLACK 0 //Negro
// #define ID_COLOR_WHITE 7 //Blanco
// //#define ID_COLOR_MAGENTA 6 //Magenta
// #define ID_COLOR_MAGENTA 3 //Magenta

 #define ID_COLOR_BLACK 0 //Negro
 #define ID_COLOR_WHITE 255 //Blanco
 #define ID_COLOR_MAGENTA 6 //Magenta 
#endif 

volatile unsigned char gb_show_osd_main_menu=0;

//********************************************************************
void SDLClear()
{
 unsigned int a32= gb_cache_color[0];
 a32= a32|(a32<<8)|(a32<<16)|(a32<<24);
 for (int y=0; y<gb_topeY; y++){
  for (int x=0; x<gb_topeX_div4; x++){
   gb_buffer_vga32[y][x]= a32;
  }
 }
}


#define max_gb_main_menu 5
const char * gb_main_menu[max_gb_main_menu]={
 "Load or Run ROM",
 "Speed",
 "Reset",
 "Exit",
 "Return"
};

#define max_gb_speed_uzebox 2
const char * gb_speed_uzebox[max_gb_speed_uzebox]={
 "Normal",
 "Turbo"
};

#define max_list_romextra 5
const char * gb_list_romextra_title[max_list_romextra]={
 "Tutorial",
 "ControllerTester",
 "BitmapDemo",
 "Chess4uzebox",
 "MegaSokoban"
};


#define gb_pos_x_menu 60
#define gb_pos_y_menu 50
#define gb_osd_max_rows 10

void jj_fast_putpixel(int x,int y,unsigned char c)
{
 gb_buffer_vga[y][x^2]= gb_cache_color[c];
}


//*************************************************************************************
void SDLprintCharOSD(char car,int x,int y,unsigned char color,unsigned char backcolor)
{ 
// unsigned char aux = gb_sdl_font_6x8[(car-64)];
 int auxId = car << 3; //*8
 unsigned char aux;
 unsigned char auxBit,auxColor;
 for (unsigned char j=0;j<8;j++)
 {
  aux = gb_sdl_font_8x8[auxId + j];
  for (int i=0;i<8;i++)
  {
   auxColor= ((aux>>i) & 0x01);
   //SDLputpixel(surface,x+(6-i),y+j,(auxColor==1)?color:backcolor);
   jj_fast_putpixel(x+(6-i),y+j,(auxColor==1)?color:backcolor);
  }
 }
}

void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor)
{
//SDL_Surface *surface,
// gb_sdl_font_6x8
 int auxLen= strlen(cad);
 if (auxLen>50)
  auxLen=50;
 for (int i=0;i<auxLen;i++)
 {
  SDLprintCharOSD(cad[i],x,y,color,backcolor);
  x+=7;
 }
}

void OSDMenuRowsDisplayScroll(const char **ptrValue,unsigned char currentId,unsigned char aMax)
{//Dibuja varias lineas
 //jjvga vga.setTextColor(WHITE,BLACK);
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  //jjvga vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
  //jjvga vga.print("                    ");
  SDLprintText("                    ",gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),ID_COLOR_BLACK,ID_COLOR_BLACK);
 }
 
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  if (currentId >= aMax)
   break;
//jjvga   if (i == 0)
//jjvga    vga.setTextColor(CYAN,BLUE);
//jjvga   else
//jjvga    vga.setTextColor(WHITE,BLACK);
//jjvga   vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
//jjvga   vga.print(ptrValue[currentId]);
  SDLprintText(ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_WHITE:ID_COLOR_WHITE),((i==0)?ID_COLOR_MAGENTA:ID_COLOR_BLACK));
  currentId++;
 }     
}



//Maximo 256 elementos
unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax)
{
 unsigned char aReturn=0;
 unsigned char salir=0; 
//jjvga  #ifdef use_lib_vga320x240
//jjvga   vga.clear(BLACK);
//jjvga   vga.fillRect(0,0,320,240,BLACK);
//jjvga   vga.fillRect(0,0,320,240,BLACK);//Repeat Fix visual defect   
//jjvga  #else
//jjvga   #ifdef use_lib_vga320x200
//jjvga    vga.clear(BLACK);
//jjvga    vga.fillRect(0,0,320,200,BLACK);
//jjvga    vga.fillRect(0,0,320,200,BLACK);//Repeat Fix visual defect 
//jjvga   #else
//jjvga    #ifdef use_lib_vga360x200
//jjvga     vga.clear(BLACK);
//jjvga     vga.fillRect(0,0,360,200,BLACK);
//jjvga     vga.fillRect(0,0,360,200,BLACK);//Repeat Fix visual defect 
//jjvga    #endif
//jjvga   #endif  
//jjvga  #endif
 //jjvga vTaskDelay(2);
 //jjvga vga.setTextColor(WHITE,BLACK);
 //jjvga vga.setCursor((gb_pos_x_menu-(32)), gb_pos_y_menu-16); 
 //jjvga vga.print("Mod ZXESPectrum by Ackerman");
 //jjvga vga.setTextColor(BLACK,WHITE);
 //SDLClear(gb_osd_sdl_surface);   

 SDLClear(); 
 SDLprintText("ESP32 UZEBOX by Ackerman",(gb_pos_x_menu-(32)),(gb_pos_y_menu-16),ID_COLOR_WHITE,ID_COLOR_BLACK);
 //for (int i=0;i<20;i++)
 for (int i=0;i<14;i++)
 { 
  SDLprintCharOSD(' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
  //SDLprintChar(gb_osd_sdl_surface,' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,BLACK,WHITE,1);
  //jjvga vga.setCursor((gb_pos_x_menu+(i*6)), gb_pos_y_menu);
  //jjvga vga.print(" ");
 } 
 //SDLprintText(gb_osd_sdl_surface,cadTitle,gb_pos_x_menu,gb_pos_y_menu,BLACK,WHITE,1);
 //jjvga vga.setCursor(gb_pos_x_menu,gb_pos_y_menu);
 //jjvga vga.print(cadTitle);  
 //for (int i=0;i<aMax;i++)
 //{     
 // //SDLprintText(gb_osd_sdl_surface,ptrValue[i],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK),1);  
 // vga.setTextColor(((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK));
 // vga.setCursor(gb_pos_x_menu,(gb_pos_y_menu+8+(i<<3)));
 // vga.print(ptrValue[i]);
 //}
 SDLprintText(cadTitle,gb_pos_x_menu,gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 OSDMenuRowsDisplayScroll(ptrValue,0,aMax);
 //SDL_Flip(gb_osd_sdl_surface); 
 while (salir == 0)
 {             
  //case SDLK_UP:  
  if (checkAndCleanKey(KEY_CURSOR_LEFT))
  {
   if (aReturn>10) aReturn-=10;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
  }
  if (checkAndCleanKey(KEY_CURSOR_RIGHT))
  {
   if (aReturn<(aMax-10)) aReturn+=10;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
  }  
  if (checkAndCleanKey(KEY_CURSOR_UP))
  {
   if (aReturn>0) aReturn--;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
  }
  if (checkAndCleanKey(KEY_CURSOR_DOWN))
  {
   if (aReturn < (aMax-1)) aReturn++;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
  }
  if (checkAndCleanKey(KEY_ENTER))
  {
   salir= 1;
  }
  //case SDLK_KP_ENTER: case SDLK_RETURN: salir= 1;break;
  if (checkAndCleanKey(KEY_ESC))
  {
   salir=1; aReturn= 255;    
  }
  //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
  //default: break;    
 }
 gb_show_osd_main_menu= 0;
 return aReturn;
}


//Menu velocidad emulador
void ShowTinySpeedMenu()
{
 unsigned char aSelNum,aSelNumSpeed;
 
 #ifdef use_lib_log_serial
  Serial.printf("ShowTinySpeedMenu\n");
 #endif
 
 aSelNum = ShowTinyMenu("SPEED CPU",gb_speed_uzebox,max_gb_speed_uzebox);
 if (aSelNum == 255)
  return;
 gb_speed_turbo = aSelNum;
}

void ShowTinyLoadROMMenu()
{

 unsigned char aSelLoadROM;
 aSelLoadROM= ShowTinyMenu("Load ROM",gb_list_romextra_title,max_list_romextra);
 if (aSelLoadROM==255){
  return; 
 }
 gb_id_cur_rom= aSelLoadROM;
 gb_load_new_rom= 1;
 
 #ifdef use_lib_log_serial
  Serial.printf("ShowTinyLoadROMMenu id:%d\n",gb_id_cur_rom);
 #endif  
}



void do_tinyOSD() 
{
 unsigned char aSelNum;
// SDLprintText("Ackerman Mod ZXESP32 Win32 Emulator",10,250,BLACK,WHITE);
 if (gb_show_osd_main_menu == 1)
 {
  delay(50);
  aSelNum = ShowTinyMenu("MAIN MENU",gb_main_menu,max_gb_main_menu);
  switch (aSelNum)
  {
   //case 0: ShowTinyMachineMenu(); break;
   case 0: ShowTinyLoadROMMenu(); break;
   case 1: ShowTinySpeedMenu(); break;

   case 2: break;
   case 3: gb_done=1; break;
   default: break;
  }
 }
}