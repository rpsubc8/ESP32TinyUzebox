#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H
 
 #define use_rom_bitmapdemo
 #define use_rom_tutorial 
 #define use_rom_controllertester
 //#define use_rom_spritedemo
 //#define use_rom_arkanoid
 #define use_rom_chess4uzebox
 #define use_rom_megasokoban

 #define gb_uzeboxDelayCPU 1

 //Usa la libreria bitluni modificada en C tiny fast
 #define use_lib_tinybitluni_fast 

 #define use_lib_vga320x240

 //Usar sonido con libreria fabgl aunque se elija bitluni vga32 
 //#define use_lib_vga8colors
 #define use_lib_vga64colors
 //Modo vga sin doble buffer problemas con modos fast
 //#define use_lib_vga_low_memory 
 //#define use_lib_200x150
 //#define use_lib_320x175 

 //Opciones para bitluni vga32 con sonido
 //#define use_lib_fabgl_sound

 //modos solo para bitluni vga opcion lib_compile_vga32
 //#define use_lib_fast_vga
 //#define use_lib_fast_vgaBLUE8colors
 //#define use_lib_fast_vgaRED8colors
 
 //Modo hilos muy lento por ahora
 //#define use_lib_vga_thread
 
 //No usa font bitluni, ahorra memoria
 //#define use_lib_tinyFont

 //log serial usb
 #define use_lib_log_serial

 //skip frames fast mode
 //#define gb_frame_crt_skip


 //Bank switch optimice speed
 //#define usb_lib_optimice_checkchange_bankswitch


 #ifdef lib_compile_vga32
  //Pin keyboard PS2
  #define KEYBOARD_DATA 32
  #define KEYBOARD_CLK 33
 #endif 

 #ifdef use_lib_fast_vgaBLUE8colors
  #ifndef use_lib_fast_vga
   #define use_lib_fast_vga
  #endif 
 #endif

 #ifdef use_lib_fast_vgaRED8colors
  #ifndef use_lib_fast_vga
   #define use_lib_fast_vga
  #endif 
 #endif

 //Milliseconds read joystick 1000 div 60 fps
 #define gb_ms_joy 16
 //Read keyboard poll x millis
 #define gb_ms_keyboard 20
 //Read sound poll x millis
 #define gb_ms_sound 1
 //Skip x frames
// #define gb_frame_crt_skip 0
 //Delay millis iteration emulate
 #define gb_delay_emulate_ms 0




 #ifdef use_lib_vga320x200
  #define gb_topeY 200
  #define gb_topeX_div4 80
 #else
  #ifdef use_lib_vga320x240
   #define gb_topeY 240
   #define gb_topeX_div4 80
  #else
   #ifdef use_lib_vga360x200
    #define gb_topeY 200
    #define gb_topeX_div4 90
   #endif 
  #endif
 #endif 
  

#endif
