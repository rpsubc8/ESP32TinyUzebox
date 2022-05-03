#include "gbConfig.h"
#include "uzebox.h"
#include "gbGlobals.h"
#include "sdlParser.h"
#include "Arduino.h"
#include <string.h>
#include "osd.h"

//BEGIN Video mode 2
 struct SpriteStruct sprites[128]; //128 sprites pruebas
 struct ScreenSectionStruct screenSections[128];
//END Video mode 2 

unsigned char palette[256];
//volatile unsigned char vram[0x1000]; //4 KB RAM
volatile unsigned char vram[0x4000]; //16 KB RAM
volatile int SCREEN_WIDTH=256;
volatile int SCREEN_HEIGHT=240;

int X_SCROLL_WRAP;
int VRAM_TILES_H;
int VRAM_TILES_V;
int TILE_WIDTH; //X_SCROLL_WRAP= VRAM_TILES_H*TILE_WIDTH
int TILE_HEIGHT;
int MAX_SPRITES;
int SCREEN_TILES_V;
int SCREEN_TILES_H;
int RAM_TILES_COUNT;
int FIRST_RENDER_LINE;
int FRAME_LINES;


const unsigned char *gb_uze_cur_TileTable; //SetTileTable render fondo
const unsigned char *gb_uze_cur_SpritesTileTable; //Tabla SetSpritesTileTable (modos 2,3)

unsigned char gb_uze_cur_FontTilesIndex=0; //Id del espacio blanco

unsigned int gb_uze_cur_gamepad[2]={0,0}; //Botones del gamepad 0 y 1

unsigned char gb_uze_firstScanlineToRender=0;
unsigned char gb_uze_verticalLinesToRender=224;

const struct PatchStruct *gb_uze_patchPointersParam=NULL;

//Video mode 8
//#define VMODE_ASM_SOURCE "videoMode8/videoMode8.s"
//#define VMODE_C_PROTOTYPES "videoMode8/videoMode8.h"
//#define VMODE_FUNC sub_video_mode8
//#define SCREEN_WIDTH 120
//#define SCREEN_HEIGHT 96
//#define VRAM_SIZE (SCREEN_WIDTH*SCREEN_HEIGHT)/4
//#define VRAM_ADDR_SIZE 1
//#define VRAM_PTR_TYPE char
//#define VRAM_TILES_H 15	
//#define FIRST_RENDER_LINE 36
//#define FRAME_LINES SCREEN_HEIGHT


//**************************************************
void ClearScreen()
{//Revisar para cambiarlo Borro pantalla
 //for (int y=0;y<400;y++){
 // memset(gb_buffer_vga[y],0,320);
 //}
 //for(int y=0;y<200;y++)
 //{
 // for(int x=0;x<256;x++)   
 // {
 //  sdl_fast_putpixel(x,y,0);
 // }
 //}
 SDLClear();
}


void sdl_fast_putpixel(int x,int y, unsigned char aColor)
{
 //vga.dotFast(x,y,aColor);
 gb_buffer_vga[y][x^2]= gb_cache_color[aColor];
}

//**************************************************
void uzeboxDelayCPU()
{
 if (gb_uzeboxDelayCPU != 0)
 {
  if (gb_speed_turbo == 0)
  {
   unsigned int initTime= millis();
   while ((millis() - initTime) < gb_uzeboxDelayCPU)
   {
     sdl_do_key_poll();
     MACRO_UZEBOX_BREAK_MAIN //Rompemos bucle para cargar otro juego
   }
   //SDL_Delay(gb_uzeboxDelayCPU);
  }
 }

 gb_currentTime= millis();
 if ((gb_currentTime - gb_keyboardTime) >= 20)
 {
  gb_keyboardTime = gb_currentTime;
  sdl_do_key_poll();
 }
 if (gb_done == 1)
 {
  #ifdef use_lib_log_serial 
   Serial.printf("EXIT from uzeboxDelayCPU\n");  
  #endif 
  do_exit();
 }
}

//**************************************************
void SetVideoMode(unsigned char aMode)
{
 switch(aMode)
 {
  case 1:
   SCREEN_WIDTH= 240;
   SCREEN_HEIGHT= 224;
   break;
  case 2:
   SCREEN_WIDTH= 144; //144x224 resolution, 6x8 pixels tiles, 32x32 vram
   SCREEN_HEIGHT= 224;       
   break; 
  case 3:
   SCREEN_WIDTH= 240;
   SCREEN_HEIGHT= 224;
   break;
  case 8: 
   SCREEN_WIDTH= 120;
   SCREEN_HEIGHT= 96;
   break;
  default:
   #ifdef use_lib_log_serial
    Serial.printf("Modo de video no soportado:%d\n",aMode);
   #endif 
   break;
 }
 #ifdef use_lib_log_serial
  Serial.printf("SetVideoMode:%d SCREEN_WIDTH:%d SCREEN_HEIGHT:%d\n",aMode,SCREEN_WIDTH,SCREEN_HEIGHT);
 #endif 
}

//*************************************
//JJ char pgm_read_byte(const char *ptr)
//JJ {
//JJ  return (*ptr);
//JJ }

//*************************************
//unsigned char pgm_read_byte(const unsigned char *ptr)
//{
// return (*ptr);         
//}

//*************************************
char uze_pgm_read_byte(const char *ptr)
{
  return (*ptr);
}

//*************************************
//unsigned char uze_pgm_read_byte_unsigned_char(const unsigned char *ptr)
//{
// return (*ptr);         
//}

//*************************************
//unsigned short int pgm_read_word(const char *ptr)
//{
// unsigned short int * auxPtr= (unsigned short int *)ptr;
// return (*auxPtr);
//}


//***************************************************
void WaitVsync(int count)
{
 int ini= gb_vsync_fps;
 int fin= ini + count;
 while (gb_vsync_fps<fin)
 {
//  sdl_do_key_poll(); //poll teclado
  gb_currentTime= millis();
  if ((gb_currentTime - gb_keyboardTime) >= 20)
  {
   gb_keyboardTime = gb_currentTime;
   sdl_do_key_poll();
  }
  if (gb_done == 1)
  {
   #ifdef use_lib_log_serial
    Serial.printf("EXIT from WaitVsync\n");
   #endif 
   //fflush(stdout);
   do_exit();
  }
  
  if (gb_speed_turbo == 1){
   break;
  }

  MACRO_UZEBOX_BREAK_MAIN //Rompemos bucle para cargar otro juego
  
 } //fin while
}

//BEGIN Video Functions

void DibujaFondo()
{
 int idTile,dest_x,dest_y;
 unsigned int offsTile;
 unsigned char aColor;     
 int i,j;
 int anchoTile,altoTile,divX,divY,ByteBlockTile;
 
 if (gb_uze_cur_TileTable == NULL){
  return;
 }
 
 switch(gb_uze_video_cur_mode)
 {
  case 2: //144x224
   anchoTile= 6;
   altoTile= 8;
   divX = 144/6;
   divY= 224/8;
   ByteBlockTile= 48;//6x8
   break;
  case 3: //240x224
   anchoTile= 8;
   altoTile= 8;
   divX = 240/8;
   divY= 224/8;
   ByteBlockTile= 64; //8x8
   break;
  default:
   anchoTile= 8;
   altoTile= 8;
   divX = 240/8;
   divY= 224/8;
   ByteBlockTile= 64;
   break;
 }
 
 #ifdef use_lib_log_serial
  Serial.printf("DibujaFondo:%d\n",gb_uze_video_cur_mode);
 #endif 
 
 //for (j=0;j<28;j++) //224 DIV 8 = 28
 for (j=0;j<divY;j++) //224 DIV 8 = 28
 {
  //for (i=0;i<30;i++) //240 DIV 8 = 30
  for (i=0;i<divX;i++) //240 DIV 8 = 30
  {
    //idTile=vram[(j*30)+i]; //240 DIV 8
    idTile=vram[(j*divX)+i]; //240 DIV 8
    //offsTile= (idTile*64);
    offsTile= (idTile*ByteBlockTile);
    //for (int yTile=0; yTile<8;yTile++)
    for (int yTile=0; yTile < altoTile; yTile++)
    {
     //for (int xTile=0; xTile<8;xTile++)
     for (int xTile=0; xTile < anchoTile; xTile++)
     {
      aColor= gb_uze_cur_TileTable[offsTile];
      //dest_y = yTile + (j*8);
      //dest_x = xTile + (i*8);
      
      dest_y = yTile + (j*altoTile);
      dest_x = xTile + (i*anchoTile);
      
//      gb_buffer_vga[dest_y][dest_x]= aColor;
      sdl_fast_putpixel(dest_x,dest_y,aColor);
      //dest_x++;
      offsTile++;
     }
    }      
  }
 }
}

//**************************
void ClearVram()
{
 switch (gb_uze_video_cur_mode)
 {
  case 1: memset((void *)vram,0,2880); break;
  case 2: DibujaFondo(); break;
  case 3: DibujaFondo(); break;
  case 8: memset((void *)vram,0,2880);  
   //sdl_fast_clear(0);
   break;
 }
}

//*************************************************************
void DrawMap2(unsigned char x,unsigned char y,const char *map)
{
 unsigned char ancho,alto;
 int dest_x,dest_y;
 unsigned char idTile;
 unsigned int offsTile;
 unsigned char aColor;
 int anchoTile,altoTile,divX,divY,ByteBlockTile;
 unsigned char *ptr_TileTable;
 
 switch(gb_uze_video_cur_mode)
 {
  case 2: //144x224
   anchoTile= 6;
   altoTile= 8;
   divX = 144/6;
   divY= 224/8;
   ByteBlockTile= 48;//6x8
   break;
  case 3: //240x224
   anchoTile= 8;
   altoTile= 8;
   divX = 240/8;
   divY= 224/8;
   ByteBlockTile= 64; //8x8
   break;
  case 5:
   anchoTile= 6; //240x224 resolution, 6x8 pixels tiles, 40x28 vram
   altoTile= 8;
   divX = 240/6;
   divY= 224/8;
   ByteBlockTile= 48; //6x8       
   break; 
  default:
   anchoTile= 8;
   altoTile= 8;
   divX = 240/8;
   divY= 224/8;
   ByteBlockTile= 64;
   break;
 } 
 
 if ((gb_uze_video_cur_mode==2)||(gb_uze_video_cur_mode==3)||(gb_uze_video_cur_mode==5))
 {
  ancho= map[0];
  alto= map[1];
  #ifdef use_lib_log_DrawMap2
   #ifdef use_lib_log_serial
    Serial.printf("DrawMap2 x:%d y:%d map:0x%08X w:%d h:%d\n",x,y,map,ancho,alto);   
   #endif 
  #endif 
  
  if ((gb_uze_cur_TileTable== NULL)&&(gb_uze_cur_SpritesTileTable==NULL)){
   #ifdef use_lib_log_DrawMap2
    #ifdef use_lib_log_serial
     Serial.printf("gb_uze_cur_TileTable y gb_uze_cur_TileTable NULL\n");
    #endif    
   #endif 
   return;
  }  
  
  #ifdef use_lib_log_DrawMap2
   #ifdef use_lib_log_serial
    Serial.printf("anchoTile:%d altoTile:%d divX:%d divY:%d block:%d\n",anchoTile,altoTile,divX,divY,ByteBlockTile);
   #endif   
  #endif 
  
  if (gb_uze_cur_TileTable != NULL){
   ptr_TileTable= (unsigned char*)gb_uze_cur_TileTable;
  }
  else{
   ptr_TileTable= (unsigned char*)gb_uze_cur_SpritesTileTable;
  }
  
  for (int j=0;j<alto;j++)
  {
   for (int i=0;i<ancho;i++)
   {
    idTile= map[(j*ancho)+i +2]; //Nos saltamos ancho y alto

    switch (gb_uze_video_cur_mode)
    {//Para simular uzebox vram
     case 5: vram[(y*40)+(j*40)+x+i] = idTile; break;
    }
    #ifdef use_lib_log_DrawMap2
     #ifdef use_lib_log_serial
      Serial.printf("%02X ",idTile);          
     #endif 
    #endif 
    //fflush(stdout);

    //offsTile= (idTile*64);
    offsTile= (idTile*ByteBlockTile); //64 o 48    
    //printf("Ofs:%d\n",offsTile);
    for (int yTile=0; yTile<altoTile;yTile++)
    {
     for (int xTile=0; xTile<anchoTile;xTile++)
     {
      //aColor= gb_uze_cur_TileTable[offsTile];      
      //printf("Antes\n");
      //fflush(stdout);
      aColor = ptr_TileTable[offsTile]; //Tabla fondo o sprites      
      //printf("Despues\n");
      //fflush(stdout);
      //printf("%02X\n",aColor);
      //fflush(stdout);
      
      dest_y = yTile + (y*altoTile) + (j*altoTile);
      dest_x = xTile + (x*anchoTile) + (i*anchoTile);
      
      if (gb_uze_video_cur_mode != 5){
       //gb_buffer_vga[dest_y][dest_x]= aColor;
       sdl_fast_putpixel(dest_x,dest_y,aColor);
      }
      //dest_x++;
      offsTile++;
     }
    }
   }
   #ifdef use_lib_log_DrawMap2
    #ifdef use_lib_log_serial
     Serial.printf("\n");
    #endif 
   #endif 
  }
  //gb_uze_cur_TileTable
  //gb_uze_cur_SpritesTileTable
  
  //if (gb_uze_video_cur_mode == 5)
  //{
  // uzeboxDelayCPU(); //Fix cambio TileMap Timer Draw Screen
  //}
 }
 else
 {
  #ifdef use_lib_log_DrawMap2
   #ifdef use_lib_log_serial
    Serial.printf("DrawMap2 no soporta modo de video:%d\n",gb_uze_video_cur_mode);
   #endif 
  #endif 
 }
}

//*************************************************************
//JJ void FadeIn(unsigned char speed,bool blocking)
void FadeIn(unsigned char speed,unsigned char blocking)
{
 #ifdef use_lib_log_serial  
  Serial.printf("FadeIn no implementada\n");
 #endif
 for (int i=0;i<(50*4);i++){
  WaitVsync(1);
  MACRO_UZEBOX_BREAK_MAIN //Rompemos bucle para cargar otro juego
 }
}

//*************************************************************
//JJ void FadeOut(unsigned char speed,bool blocking)
void FadeOut(unsigned char speed,unsigned char blocking)
{
 #ifdef use_lib_log_serial
  Serial.printf("FadeOut no implementada\n");
 #endif
 for (int i=0;i<(50*4);i++){
  WaitVsync(1);
  MACRO_UZEBOX_BREAK_MAIN //Rompemos bucle para cargar otro juego
 } 
}

//*************************************************************
void Fill(int x,int y,int width,int height,int tile)
{//Fill an arbitrary sized, rectangular region using the specified tile index
 int idTile,dest_x,dest_y;
 unsigned int offsTile;
 unsigned char aColor;
 int i,j;
 int anchoTile,altoTile,divX,divY,ByteBlockTile;
 
 switch(gb_uze_video_cur_mode)
 {
  case 2: //144x224
   anchoTile= 6;
   altoTile= 8;
   divX = 144/6;
   divY= 224/8;
   ByteBlockTile= 48;//6x8
   break;
  case 3: //240x224
   anchoTile= 8;
   altoTile= 8;
   divX = 240/8;
   divY= 224/8;
   ByteBlockTile= 64; //8x8
   break;
  default:
   anchoTile= 8;
   altoTile= 8;
   divX = 240/8;
   divY= 224/8;
   ByteBlockTile= 64;
   break;
 } 

 #ifdef use_lib_log_serial
  Serial.printf("Funcion Fill\n");
 #endif
 
 if (gb_uze_cur_TileTable== NULL)
 {
  return;
 }

 for (j=0;j<height;j++)
 {
  for (i=0;i<width;i++)
  { 
    //offsTile= (tile*64);
    offsTile= (tile*ByteBlockTile); //48 o 64
    for (int yTile=0; yTile<altoTile;yTile++)
    {
     for (int xTile=0; xTile<anchoTile;xTile++)
     {
      aColor= gb_uze_cur_TileTable[offsTile];
      dest_y = yTile + (j*altoTile) + (y*altoTile);
      dest_x = xTile + (i*anchoTile) + (x*anchoTile);
      
//      gb_buffer_vga[dest_y][dest_x]= aColor;
      sdl_fast_putpixel(dest_x,dest_y,aColor);
      //dest_x++;
      offsTile++;
     }
    }       
  }
 }
}

//*************************************************************
void MapSprite(unsigned char startSprite,const char *map)
{
 #ifdef use_lib_log_serial
  Serial.printf("MoveSprite no implementada\n");
 #endif
}

//*************************************************************
void MoveSprite(unsigned char startSprite,unsigned char x,unsigned char y,unsigned char width,unsigned char height)
{
 #ifdef use_lib_log_serial
  Serial.printf("MoveSprite no implementada\n");
 #endif
}

//*************************************************************
//JJ void SetSpriteVisibility(bool visible)
void SetSpriteVisibility(unsigned char visible)
{
 if ((gb_uze_video_cur_mode == 2) || (gb_uze_video_cur_mode == 3))
 {
  #ifdef use_lib_log_serial
   Serial.printf("Funcion SetSpriteVisibility sin implementar\n");
  #endif 
 }
 else
 {
  #ifdef use_lib_log_serial
   Serial.printf("Funcion SetSpriteVisibility modo de video no soportado:%d\n",gb_uze_video_cur_mode);
  #endif 
 }
}

//***************************************************
void SetSpritesOptions(unsigned char params)
{
 #ifdef use_lib_log_serial
  Serial.printf("SetSpritesOptions no implementada\n");
 #endif
}

//END Video Functions

//**************************
void PutPixel(int x,int y,unsigned char aColor)
{
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

//**************************
void SetFontTable(const char *data)
{
 gb_uze_font = data;
}

//**************************
void PrintChar(int x, int y, char c)
{
 //char **ptr = (char **)vram;
 //int auxOfsScreen = y*x;
 //int idLetra= (c-65)*64;
 //char * ptrOfsFont = (char *)&gb_uze_font[idLetra];
 //ptr[auxOfsScreen] = ptrOfsFont;
 
 switch (gb_uze_video_cur_mode)
 {
  case 1: SDL_print_char(x,y,8,8,64,c); break;        
  case 2: SDL_print_char(x,y,6,8,48,c); break;
  case 3: SDL_print_char(x,y,8,8,64,c); break;
  case 5: SDL_print_char(x,y,6,8,48,c); break;
 }
  
}

//**************************************************
void uze_Print(int x, int y, const char *str)
{
  int len= strlen(str);
  int destX= x;
  int destY= y;
  for (int i=0;i<len;i++)
  {
   PrintChar(destX, destY, str[i]);
   destX++;
  }
 }

//**********************************************
void SetFontTilesIndex(unsigned char index)
{//Tile index of the space character. El id del caracter blanco
 gb_uze_cur_FontTilesIndex = index; 
 gb_uze_font = (char *)gb_uze_cur_TileTable; //Fuerzo a usar la Tabla actual
 #ifdef use_lib_log_serial
  Serial.printf("SetFontTilesIndex Id:%d\n",index);
 #endif 
}

//**********************************************
void SetFont(char x,char y, unsigned char tileId)
{
 #ifdef use_lib_log_serial  
  Serial.printf("SetFont no implementada\n");
 #endif
}



//************************************
void SetTileTable(const char *data)
{//render background
 gb_uze_cur_TileTable = (const unsigned char *)data;
 #ifdef use_lib_log_serial
  Serial.printf("SetTileTable:0x%0x08\n",data); 
 #endif  
}

//**************************
void SetSpritesTileTable(const char *data)
{
 if ((gb_uze_video_cur_mode == 2) || (gb_uze_video_cur_mode == 3))
 {        
  gb_uze_cur_SpritesTileTable= (const unsigned char *)data;
  #ifdef use_lib_log_serial
   Serial.printf("SetSpritesTileTable:0x%0x08\n",data);
  #endif 
 }
 else
 {
  #ifdef use_lib_log_serial
   Serial.printf("SetSpritesTileTable no soporta modeo de video:%d\n",gb_uze_video_cur_mode);
  #endif 
 } 
}

//****************************************************
void SetTile(char x,char y, unsigned int tileId)
{
 int ancho,alto,idTile,dest_x,dest_y;
 unsigned int offsTile;
 unsigned char aColor;
 int anchoTile,altoTile,divX,divY,ByteBlockTile;
 unsigned char *ptr_TileTable;
 
 switch(gb_uze_video_cur_mode)
 {
  case 5:
   anchoTile= 6;
   altoTile= 8;
   divX = 240/6;
   divY= 224/8;
   ByteBlockTile= 48; //6x8       
   break; 
  default:
   #ifdef use_lib_log_SetTile
    #ifdef use_lib_log_serial
     Serial.printf("SetTile video no soportado %d\n",gb_uze_video_cur_mode);
    #endif     
   #endif 
   return;
   break;
 }      
 
 
 if (gb_uze_cur_TileTable != NULL){
  ptr_TileTable= (unsigned char*)gb_uze_cur_TileTable;
 }
 else{
  ptr_TileTable= (unsigned char*)gb_uze_cur_SpritesTileTable;
 }
  
 vram[(y*divX)+x]=tileId; //Simula vram Uzebox
 
 offsTile= (tileId*ByteBlockTile); //64 o 48   
 for (int yTile=0; yTile<altoTile;yTile++)
 {
  for (int xTile=0; xTile<anchoTile;xTile++)
  { 
   aColor = ptr_TileTable[offsTile]; //Tabla fondo o sprites
   dest_y = yTile + (y*altoTile);
   dest_x = xTile + (x*anchoTile);
      
//   gb_buffer_vga[dest_y][dest_x]= aColor;   
   offsTile++;
  }
 }
 #ifdef use_lib_log_SetTile
  #ifdef use_lib_log_serial
   Serial.printf("SetTile x:%d y:%d tileId:%d video:%d\n",x,y,tileId,gb_uze_video_cur_mode);
  #endif  
 #endif 
}

void SetRenderingParameters(unsigned char firstScanlineToRender, unsigned char verticalLinesToRender)
{
 gb_uze_firstScanlineToRender= firstScanlineToRender;
 gb_uze_verticalLinesToRender= verticalLinesToRender;
 
 #ifdef use_lib_log_serial
  Serial.printf("SetRenderingParameters first:%d Lines:%d\n",firstScanlineToRender,verticalLinesToRender); 
  Serial.printf("SetRenderingParameters no implementada\n");
 #endif  
}





//BEGIN Controllers Functions
void EnableSnesMouse(unsigned char spriteIndex,const char *spriteMap)
{
}

//**************************************
unsigned int ReadJoypad(unsigned char joypadNo)
{

 sdl_do_key_poll();
         
 gb_uze_cur_gamepad[1]= gb_uze_cur_gamepad[0];
 
 #ifdef use_lib_log_ReadJoypad
  #ifdef use_lib_log_serial
   Serial.printf("ReadJoypad id:%d %08X\n",joypadNo,gb_uze_cur_gamepad[joypadNo]);
  #endif   
 #endif
 
 if (joypadNo < 2)
 {
  return gb_uze_cur_gamepad[joypadNo];
 }
 return 0;
}

//**************************************
unsigned char DetectControllers()
{//Detects what devices are connected to the game ports
 //b1:b0 = 00 -> No controller connected in P1 port
 //        01 -> Gamepad connected in P1 port
 //        10 -> Mouse connected in P1 port
 //        11 -> Reserved
 //b3:b2 = 00 -> No controller connected in P2 port
 //        01 -> Gamepad connected in P2 port
 //        10 -> Mouse connected in P2 port
 //        11 -> Reserved
 return 1;
}
//END Controllers Functions



//BEGIN SOUND
//************************************************************
void InitMusicPlayer(const struct PatchStruct *patchPointersParam)
{
 #ifdef use_lib_log_serial
  Serial.printf("InitMusicPlayer\n");
 #endif  
 gb_uze_patchPointersParam = patchPointersParam;
}

//************************************************************
void ResumeSong()
{
}

//************************************************************
void SetMasterVolume(unsigned char vol)
{
}

//************************************************************
void StartSong(const char *midiSong)
{
}

//************************************************************
void StopSong()
{
}

//************************************************************
//JJ void TriggerFx(unsigned char patch,unsigned char volume, bool retrig)
void TriggerFx(unsigned char patch,unsigned char volume, unsigned char retrig)
{ 
 unsigned char data_type;
 const char * data_pcmData;
 const char * data_cmdStream;  
 unsigned int data_loopStart;
 unsigned int data_loopEnd;    
 
 unsigned char dataPitch;
 
// printf("TriggerFx path:%d vol:%d ret:%d\n",patch,volume,retrig);
 if (gb_uze_patchPointersParam != NULL)
 {
  data_type = gb_uze_patchPointersParam[patch].type;
  data_pcmData = gb_uze_patchPointersParam[patch].pcmData;
  data_cmdStream = gb_uze_patchPointersParam[patch].cmdStream;
  data_loopStart = gb_uze_patchPointersParam[patch].loopStart;
  data_loopEnd = gb_uze_patchPointersParam[patch].loopEnd;

  dataPitch= data_cmdStream[8];
//  printf(" type:%d star:%d end:%d pitch:%d\n",data_type,data_loopStart,data_loopEnd, dataPitch);
  
  if (dataPitch>0)
  {
//   gbVolMixer_now[0]= 15;
//   gbFrecMixer_now[0]= dataPitch;
   //gb_uze_ini_triggerfx= SDL_GetTicks();   
//   gb_uze_ini_triggerfx = gb_vsync_fps;
//   gb_uze_ini_play_triggerfx=1;
  }
  else
  {
//   gbVolMixer_now[0]= 0;      
//   gbFrecMixer_now[0]= 0;
//   gb_uze_ini_play_triggerfx=0;
  }
  
 } 
}

//************************************************************
void TriggerNote(unsigned char channel,unsigned char patch,unsigned char note,unsigned char volume)
{
}

//************************************************************
void EnableSoundEngine()
{
}

//************************************************************
void DisableSoundEngine()
{
}
//END SOUND


//BEGIN EEPROM
char EepromReadBlock(unsigned int blockId,struct EepromBlockStruct *block)
{
 #ifdef use_lib_log_serial
  Serial.printf("EepromReadBlock no implementada\n");
 #endif  
 block->id= blockId; //Hay que modificarlo
 memset(block->data,0,30);
 return 0;
}

//************************************************************
char EepromWriteBlock(struct EepromBlockStruct *block)
{
 #ifdef use_lib_log_serial
  Serial.printf("EepromWriteBlock no implementada\n");
 #endif 
 return 0;
}
//END EEPROM


//**************************
void do_exit()
{
 //SDL_PauseAudio(1);
 //SDL_CloseAudio(); 
         
 //printf("EXIT\n");
 //fflush(stdout);
 //SDL_RemoveTimer(my_timer_id);
 LiberarRecursos();
 //atexit (SDL_Quit);
 //exit(1);
}
