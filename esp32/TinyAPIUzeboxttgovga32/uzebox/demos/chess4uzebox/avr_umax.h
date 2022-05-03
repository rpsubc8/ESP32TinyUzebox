#ifndef AVR_UMAX_H
#define AVR_UMAX_H

 #include "../../gbConfig.h"
 
 #ifdef use_rom_chess4uzebox
 
 #include "../../gbGlobals.h"
 #include "../../uzebox.h"
 #include<stdint.h>
 
 
 #define W while
 #define M 0x88
 #define S 128
 #define I 8000
 
 
 extern char ZMAX;  /* variable stack limit for tuning cpu moves quality */
 extern long N, T;                  /* N=evaluated positions+S, T=recursion limit */
 extern short Q,O,K,R,k;        /* k=moving side */
 extern char *p,c[7],Z;   /* p=pointer to c, c=user input, computer output, Z=recursion counter */

 extern char L,w[8],o[32],b[129];


 extern unsigned short r;
 
 
 extern short q,l,e;
 
 extern unsigned char E,z,n; 
 
 
 extern int break_joypad;
 
 extern uint8_t is_demo;
 extern int8_t exit_game;
 extern uint8_t was_computer_move;

 
 void new_game(void);
 unsigned short myrand(void);
 void mysrand(unsigned short r_);
 void play_game(void);
 
 #endif

#endif

 


 
 
