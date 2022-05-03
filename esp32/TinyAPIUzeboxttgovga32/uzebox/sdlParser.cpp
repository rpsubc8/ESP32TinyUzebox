#include "gbConfig.h"
#include "sdlParser.h"
#include "gbGlobals.h"
#include "uzebox.h"
#include "osd.h"
#include "PS2Kbd.h"
#include "PS2KeyCode.h"
#include "keys.h"


//***************************************
void LiberarRecursos()
{
 //SDL_RemoveTimer(my_timer_id);
}

//**********************************************
void SDL_print_char(int x,int y,int w,int h,int byteBlock,unsigned char idLetra)
{
 //char **ptr = (char **)vram;
 //int auxOfsScreen = y*x;
 //int idLetra= (c-65)*64;
 //char * ptrOfsFont = (char *)&gb_uze_font[idLetra];
 //ptr[auxOfsScreen] = ptrOfsFont;     
 //unsigned char auxId= (idLetra-65)*64;;
 if (gb_uze_font == NULL)
 {
  //printf("gb_uze_font NULL\n");
  //fflush(stdout);
  return;
 }
 
 unsigned char antesIdLetra= idLetra;
 
 idLetra= idLetra-32;
 if (idLetra>90)
 {
  idLetra= 0;
 }
 
 idLetra= antesIdLetra-32+gb_uze_cur_FontTilesIndex;
 
 //if (antesIdLetra == 32)
 // idLetra= gb_uze_cur_FontTilesIndex;
 //idLetra = idLetra-gb_uze_cur_FontTilesIndex;
 
 unsigned int offsetId= (unsigned int)(idLetra) * byteBlock; //48 o 64
 //if (id>=32)&&(id<96)
 unsigned char aux;
 int dest_x,dest_y;
 #ifdef use_lib_log_PrintChar
  #ifdef use_lib_log_serial
   Serial.printf("idLetra:%d charLetra:%c Letra:%d char:%c Offs:%d\n",antesIdLetra,antesIdLetra,idLetra,idLetra,offsetId);
  #endif
 #endif 
 
 switch (gb_uze_video_cur_mode)
 {
  case 5: vram[(y*40)+x] = idLetra; break;
 }
 
    
 for (int j=0;j<h;j++)
 {
  for (int i=0;i<w;i++)
  {
   aux= gb_uze_font[offsetId];
   #ifdef use_lib_log_PrintChar
    #ifdef use_lib_log_serial
     Serial.printf("%02X",aux);
    #endif 
   #endif
   dest_y = j + (y*h);
   dest_x = i + (x*w);
      
//   gb_buffer_vga[dest_y][dest_x]= aux;
   sdl_fast_putpixel(dest_x,dest_y,aux);
   offsetId++;
  }
  #ifdef use_lib_log_PrintChar
   #ifdef use_lib_log_serial
    Serial.printf("\n");
   #endif 
  #endif 
 }
}


//void sdl_fast_putpixel(unsigned int x, unsigned int y, unsigned int c)
//{
// if (x >= 400 || y >= 300)
//  return;
// Uint8* p = (Uint8*)gb_screen->pixels + (y * gb_screen->pitch) + x;
// *p= c;
//}

//******************************************
//void sdl_dump_bitluni()
//{
 //unsigned int auxLine = gb_screen->pitch;
 //Uint8* p = (Uint8*)gb_screen->pixels;
 //for (int y=0;y<240;y++)
 //{
 // p = (Uint8*)gb_screen->pixels + (y * auxLine);
 // for (int x=0;x<320;x++)
 // { 
 //  *p= gb_buffer_vga[y][x];
 //  p++;
 // }  
 //}
//}

//******************************************
void sdl_fast_clear(unsigned int c)
{
 for (int y=0;y<240;y++)
 {
  for (int x=0;x<320;x++)
  {
   //sdl_fast_putpixel(x,y,c);
   gb_buffer_vga[y][x]= c;
  }
 }
}


//**************************************
void sdl_do_key_poll()
{ 

 if (checkAndCleanKey(KEY_F1)) 
 {
  gb_show_osd_main_menu= 1;
  gb_draw_timer= 0;
  do_tinyOSD();
  gb_show_osd_main_menu= 0;
  gb_draw_timer= 1;
 }

//BOTON A
 if (keymap[PS2_KC_X] == 0) gb_uze_cur_gamepad[0] |= BTN_A; //1 A
 else gb_uze_cur_gamepad[0] &= (~BTN_A)&0xFF; //1 A     
 //BOTON B
 if (keymap[PS2_KC_Z] == 0) gb_uze_cur_gamepad[0] |= BTN_B; //2 B
 else gb_uze_cur_gamepad[0] &= (~BTN_B)&0xFF; //2 B
 //BOTON SELECT
 if (keymap[PS2_KC_C] == 0) gb_uze_cur_gamepad[0] |= BTN_SELECT; //4 SELECT
 else gb_uze_cur_gamepad[0] &= (~BTN_SELECT)&0xFF; //4 SELECT
 //BOTON START
 if (keymap[PS2_KC_V] == 0) gb_uze_cur_gamepad[0] |= BTN_START; //8 START
 else gb_uze_cur_gamepad[0] &= (~BTN_START)&0xFF; //8 START
 //BOTON ARRIBA
 if (keymap[KEY_CURSOR_UP] == 0) gb_uze_cur_gamepad[0] |= BTN_UP; //16
 else gb_uze_cur_gamepad[0] &= (~BTN_UP)&0xFF;
 //BOTON ABAJO
 if (keymap[KEY_CURSOR_DOWN] == 0) gb_uze_cur_gamepad[0] |= BTN_DOWN; //32
 else gb_uze_cur_gamepad[0] &= (~BTN_DOWN)&0xFF;
 //BOTON IZQUIERDA
 if (keymap[KEY_CURSOR_LEFT] == 0) gb_uze_cur_gamepad[0] |= BTN_LEFT; //64
 else gb_uze_cur_gamepad[0] &= (~BTN_LEFT)&0xFF;
 //BOTON DERECHA
 if (keymap[KEY_CURSOR_RIGHT] == 0) gb_uze_cur_gamepad[0] |= BTN_RIGHT; //128
 else gb_uze_cur_gamepad[0] &= (~BTN_RIGHT)&0xFF;

  /*
 while (SDL_PollEvent (&gb_event))
 {//Check for events
  switch (gb_event.type)
  {
   case SDL_KEYDOWN:
    if (gb_event.key.keysym.sym == SDLK_F10)
    {                         
     printf("Tecla F10 JJ\n");
     fflush(stdout);
     //SDL_RemoveTimer(my_timer_id);     
     gb_done = 1;
    }
    
    switch (gb_event.key.keysym.sym)
    {
     case SDLK_F1:
      gb_show_osd_main_menu=1;
      gb_draw_timer=0;
      do_tinyOSD();
      gb_draw_timer=1;
      gb_show_osd_main_menu=0;
      break;
     case SDLK_RIGHT: gb_uze_cur_gamepad[0] |= BTN_RIGHT; break; //128
     case SDLK_LEFT: gb_uze_cur_gamepad[0] |= BTN_LEFT; break; //64
     case SDLK_DOWN: gb_uze_cur_gamepad[0] |= BTN_DOWN; break; //32
     case SDLK_UP: gb_uze_cur_gamepad[0] |= BTN_UP; break; //16
     
     case SDLK_SPACE: gb_uze_cur_gamepad[0] |= BTN_START; break; //8 START
     case SDLK_KP_ENTER: gb_uze_cur_gamepad[0] |= BTN_SELECT; break; //4 SELECT
     case SDLK_RETURN: gb_uze_cur_gamepad[0] |= BTN_SELECT; break; //4 SELECT
     case SDLK_z: gb_uze_cur_gamepad[0] |= BTN_B; break; //2 B
     case SDLK_a: gb_uze_cur_gamepad[0] |= BTN_A; break; //1 A
    }                    
    break; //Fin break pulsar
         
             
   case SDL_KEYUP: 
    switch (gb_event.key.keysym.sym)
    {
     case SDLK_RIGHT: gb_uze_cur_gamepad[0] &= (~BTN_RIGHT)&0xFF; break;
     case SDLK_LEFT: gb_uze_cur_gamepad[0] &= (~BTN_LEFT)&0xFF; break;
     case SDLK_DOWN: gb_uze_cur_gamepad[0] &= (~BTN_DOWN)&0xFF; break;
     case SDLK_UP: gb_uze_cur_gamepad[0] &= (~BTN_UP)&0xFF; break;
     
     case SDLK_SPACE: gb_uze_cur_gamepad[0] &= (~BTN_START)&0xFF; break; //8 START
     case SDLK_KP_ENTER: gb_uze_cur_gamepad[0] &= (~BTN_SELECT)&0xFF; break; //4 SELECT
     case SDLK_RETURN: gb_uze_cur_gamepad[0] &= (~BTN_SELECT)&0xFF; break; //4 SELECT
     case SDLK_z: gb_uze_cur_gamepad[0] &= (~BTN_B)&0xFF; break; //2 B
     case SDLK_a: gb_uze_cur_gamepad[0] &= (~BTN_A)&0xFF; break; //1 A     
     
    }        
    break;//fin break liberar
    
   case SDL_QUIT:
    gb_done = 1;
    break;
   default:
    break;
  }
 } 
 
 
 if (gb_done == 1)
 {
  printf("EXIT from sdl_do_key_poll\n");
  fflush(stdout);
  do_exit();
 }
 */
}
