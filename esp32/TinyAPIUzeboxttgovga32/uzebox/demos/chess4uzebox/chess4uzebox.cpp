#include "../../gbConfig.h" 

#ifdef use_rom_chess4uzebox



//JJ #include <uzebox.h>
#include "../../uzebox.h"
//JJ #include <avr/pgmspace.h>
//JJ #include <avr/interrupt.h>
#include <string.h>

#include "graphics.inc"
#include "patches.inc"
#include "music.inc"
#include "chess4uzebox.h"
#include "avr_umax.h"
#include <stdint.h>
#include <stdbool.h>



//#if TILE_WIDTH == 6 && TILE_HEIGHT == 8
	#include "data/font-6x8-full.inc"

//#elif TILE_WIDTH == 6 && TILE_HEIGHT == 12
//	#include "data/font-6x12-full.inc"
//
//#elif TILE_WIDTH == 8 && TILE_HEIGHT == 8
//	#include "data/font-8x8-full.inc"
//#else
//	#error Unsupported tile size defined in makefile! For this demo only 6x8, 6x12 and 8x8 tile sizes are supported.
//#endif



// position of cursor of each player
uint8_t p1cursor, p2cursor;
// position and status of blinking square
uint8_t blinking1, blinking2, blink_status;
// type of player on each side
uint8_t p1mode, p2mode;
// pieces count for detect capture, promotion
uint8_t pieces_count, nonpawn_pieces_count;
// stores, if last move was computer move
uint8_t was_computer_move;
// stores, if current game is demo
uint8_t is_demo;
// counts moves in game
uint16_t move_count;
// for testing wrong user move
char test_c[4];
// game settings
uint8_t settings;
// counter for 50 moves rule
int8_t rule50;
// flag to indicate that user wants to exit game
int8_t exit_game;
// or-ed status of joypad during thinking (to break game)
int break_joypad;

// avr-micro-max chess engine
//JJ revisar #include "avr_umax.c"







//short D(q,l,e,E,z,n)                          //recursive minimax search
short D(short q, short l,short e, unsigned char E,unsigned char z,unsigned char n)                          // recursive minimax search
{                       
//short q,l,e;                      // (q,l)=window, e=current eval. score,
//unsigned char E,z,n;     // E=e.p. sqr.z=prev.dest, n=depth; return score                        
 //short q,l,e;                        
 //unsigned char E,z,n;                 
 
                        
 short m,v,i,P,V,s;
 unsigned char t,p,u,x,y,X,Y,H,B,j,d,h,F,G,C;
 signed char r;

 break_joypad |= ReadJoypad(0) | ReadJoypad(1);// change flag to break     

 if (++Z>6) {                                  // stack underrun check     
  --Z;return e;                                    
 }
 
 q--;                                          // adj. window: delay bonus 
 k^=24;                                        // change sides             
 d=Y=0;                                        // start iter. from scratch 
 X=myrand()&~M;                                // start at random field    
 W(d++<n||d<3||                                // iterative deepening loop 
   z&K==I&&(N<T&d<98||                         // root: deepen upto time   
   (K=X,L=Y&~M,d=3)))                          // time's up: go do best    
 {x=B=X;                                       // start scan at prev. best 
  h=Y&S;                                       // request try noncastl. 1st
  P=d<3?I:D(-l,1-l,-e,S,0,d-3);                // Search null move         
  m=-P<l|R>35?d>2?-I:e:-P;                     // Prune or stand-pat       
  ++N;                                         // node count (for timing)  
  do{u=b[x];                                   // scan board looking for   
   if(u&k)                                     //  own piece (inefficient!)
   {r=p=u&7;                                   // p = piece type (set r>0) 
    j=o[p+16];                                 // first step vector f.piece
    W(r=p>2&r<0?-r:-o[++j])                    // loop over directions o[] 
    {A:                                        // resume normal after best 
     y=x;F=G=S;                                // (x,y)=move, (F,G)=castl.R
     do{                                       // y traverses ray, or:     
      H=y=h?Y^h:y+r;                           // sneak in prev. best move 
      if(y&M)break;                            // board edge hit           
      m=E-S&b[E]&&y-E<2&E-y<2?I:m;             // bad castling             
      if(p<3&y==E)H^=16;                       // shift capt.sqr. H if e.p.
      t=b[H];if(t&k|p<3&!(y-x&7)-!t)break;     // capt. own, bad pawn mode 
      i=37*w[t&7]+(t&192);                     // value of capt. piece t   
      m=i<0?I:m;                               // K capture                
      if(m>=l&d>1)goto C;                      // abort on fail high       
      v=d-1?e:i-p;                             // MVV/LVA scoring          
      if(d-!t>1)                               // remaining depth          
      {v=p<6?b[x+8]-b[y+8]:0;                  // center positional pts.   
       b[G]=b[H]=b[x]=0;b[y]=u|32;             // do move, set non-virgin  
       if(!(G&M))b[F]=k+6,v+=50;               // castling: put R & score  
       v-=p-4|R>29?0:20;                       // penalize mid-game K move 
       if(p<3)                                 // pawns:                   
       {v-=9*((x-2&M||b[x-2]-u)+               // structure, undefended    
              (x+2&M||b[x+2]-u)-1              //        squares plus bias 
             +(b[x^16]==k+36))                 // kling to non-virgin King 
             -(R>>2);                          // end-game Pawn-push bonus 
        V=y+r+1&S?647-p:2*(u&y+16&32);         // promotion or 6/7th bonus 
        b[y]+=V;i+=V;                          // change piece, add score  
        switch (c[4]) {						   // under-promotion (not so  
        case 'N':							   // minimalistic)            
        	b[y]-=4;
        	break;
        case 'R':
        	b[y]-=1;
        	break;
        case 'B':
        	b[y]-=2;
        	break;
        }
       }
       v+=e+i;V=m>q?m:q;                       // new eval and alpha       
       C=d-1-(d>5&p>2&!t&!h);
       C=R>29|d<3|P-I?C:d;                     // extend 1 ply if in check 
       do
        s=C>2|v>V?-D(-l,-V,-v,                 // recursive eval. of reply 
                              F,0,C):v;        // or fail low if futile    
       W(s>q&++C<d);v=s;
       if(z&&K-I&&v+I&&x==K&y==L)              // move pending & in root:  
       {Q=-e-i;O=F;                            //   exit if legal & found  
        R+=i>>7;--Z;return l;                  // captured non-P material  
       }
       if((z==9)&&(!was_computer_move))        // called as move-legality  
       {if(v!=-I&x==K&y==L)                    //   checker: if move found 
        {Q=-e-i;O=F;return l;}                 //    & not in check, signal 
        v=m;                                   // (prevent fail-lows on    
       }                                       //   K-capt. replies)       
       b[G]=k+6;b[F]=b[y]=0;b[x]=u;b[H]=t;     // undo move,G can be dummy 
      }
      if(v>m)                                  // new best, update max,best
       m=v,X=x,Y=y|S&F;                        // mark double move with S  
      if(h){h=0;goto A;}                       // redo after doing old best
      if(x+r-y|u&32|                           // not 1st step,moved before
         p>2&(p-4|j-7||                        // no P & no lateral K move,
         b[G=x+3^r>>1&7]-k-6                   // no virgin R in corner G, 
         ||b[G^1]|b[G^2])                      // no 2 empty sq. next to R 
        )t+=p<5;                               // fake capt. for nonsliding
      else F=y;                                // enable e.p.              
     }W(!t);                                   // if not capt. continue ray
  }}}W((x=x+9&~M)-B);                          // next sqr. of board, wrap 
C:if(m>I-M|m<M-I)d=98;                         // mate holds to any depth  
  m=m+I|P==I?m:0;                              // best loses K: (stale)mate
  if(z&&d>2)
   {*c='a'+(X&7);c[1]='8'-(X>>4);c[2]='a'+(Y&7);c[3]='8'-(Y>>4&7);c[4]=0;}
 }                                             //    encoded in X S,8 bits 
 k^=24;                                        // change sides back        
 --Z;return m+=m<e;                            // delayed-loss bonus       
}




/**
 * Plays sound for given event
 * @param uint8_t sound_event type of sound event
 */
void play_sound(uint8_t sound_event) {
	if (settings & SETTINGS_SOUND) {
		TriggerFx(sound_event, 0xFF, true);
	}
}

/**
 * Plays given song
 * @param char *song pointer to start of song
 */
void play_song(char *song) {
	if (settings & SETTINGS_MUSIC) {
		StartSong(song);
	}
}

/**
 * Shows main menu
 */
void show_menu() {
	// prepare screen
	SetTileTable(menu);
	SetFontTilesIndex(MENU_FONT_START);
	ClearVram();
	EnableSoundEngine();
	is_demo = 0;

	// menu interaction
	uint8_t menu_cursor = 0;
	uint8_t *p_mode;
	uint8_t editing_setting;
	uint16_t demo_countdown = DEMO_COUNTDOWN_TOP;
	unsigned int buttons = 0;
	do {
		// refresh screen
		MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_COUNTDOWN //Rompemos bucle para cargar otro juego
		
		WaitVsync(1);
		DrawMap2(0, 0, menu_screen);
		uze_Print(12, 20, PSTR("Black:"));
		uze_Print(12, 21, PSTR("White:"));
		uze_Print(12, 22, PSTR("Sound:"));
		uze_Print(12, 23, PSTR("Music:"));
		uze_Print(12, 24, PSTR("Turbo:"));
		uze_Print(12, 26, PSTR("press START to play"));
		uze_Print(19, 20, players[p2mode]);
		uze_Print(19, 21, players[p1mode]);
		uze_Print(19, 22, yesno[(settings & SETTINGS_SOUND) ? 1 : 0]);
		uze_Print(19, 23, yesno[(settings & SETTINGS_MUSIC) ? 1 : 0]);
		uze_Print(19, 24, yesno[(settings & SETTINGS_TURBO) ? 1 : 0]);
		uze_Print(10, 20 + menu_cursor, PSTR(">"));

		// wait until key release
		if (buttons) {
			demo_countdown = DEMO_COUNTDOWN_TOP;
			while (ReadJoypad(0) | ReadJoypad(1))
            {
             //uzeboxDelayCPU(); //Para que no se bloquee
             MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_COUNTDOWN //Rompemos bucle para cargar otro juego
            };
		}

		// interact
		buttons = ReadJoypad(0) | ReadJoypad(1);

		if (buttons & BTN_UP) {
			if (menu_cursor > 0) {
				menu_cursor--;
				play_sound(SOUND_MOVE_CURSOR);
			} else {
				play_sound(SOUND_CANT_MOVE_CURSOR);
			}
		}
		if (buttons & BTN_DOWN) {
			if (menu_cursor < 4) {
				menu_cursor++;
				play_sound(SOUND_MOVE_CURSOR);
			} else {
				play_sound(SOUND_CANT_MOVE_CURSOR);
			}
		}
		if (menu_cursor < 2) {
			p_mode = menu_cursor ? (&p1mode) : (&p2mode);
			if (buttons & BTN_LEFT) {
				if ((*p_mode) > 0) {
					(*p_mode)--;
					play_sound(SOUND_MOVE_CURSOR);
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
			if (buttons & BTN_RIGHT) {
				if ((*p_mode) < 5) {
					(*p_mode)++;
					play_sound(SOUND_MOVE_CURSOR);
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
		} else {
			editing_setting = 1 << (menu_cursor - 2);
			if (buttons & BTN_LEFT) {
				if (settings & editing_setting) {
					settings ^= editing_setting;
					play_sound(SOUND_MOVE_CURSOR);
					if (menu_cursor == 3) {
						StopSong();
					}
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
			if (buttons & BTN_RIGHT) {
				if (!(settings & editing_setting)) {
					settings ^= editing_setting;
					play_sound(SOUND_MOVE_CURSOR);
					if (menu_cursor == 3) {
						ResumeSong();
					}
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
		}

		demo_countdown--;

     
     //uzeboxDelayCPU(); //Para que no se bloquee
     MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_COUNTDOWN //Rompemos bucle para cargar otro juego
	} while (!(buttons & BTN_START) && demo_countdown);
	if (!demo_countdown) {
		is_demo = true;
		return;
	}
	while ((ReadJoypad(0) | ReadJoypad(1)) & BTN_START)
    {
     //uzeboxDelayCPU(); //Para que no se bloquee          
     MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_COUNTDOWN //Rompemos bucle para cargar otro juego
    };
    
	if ((settings & (SETTINGS_SOUND | SETTINGS_MUSIC))) {
		EnableSoundEngine();
	} else {
		DisableSoundEngine();
	}
	play_sound(SOUND_SELECT);
}

/**
 * Initializes game
 */
void init_game() 
{
 //printf("init_game\n");
 //fflush(stdout);
 
	SetTileTable(game);
	SetFontTilesIndex(GAME_FONT_START);
	ClearVram();
	DrawMap2(0, 0, game_board);	
	
	p1cursor = 0xFF;
	p2cursor = 0xFF;
	if (p1mode < 2) {
		p1cursor = 0x60;
	}
	if (p2mode < 2) {
		p2cursor = 0x10;
	}

	blinking1 = 0xFF;
	blinking2 = 0xFF;
	pieces_count = 32;
	nonpawn_pieces_count = 16;
	was_computer_move = 0;
	move_count = 0;
	rule50 = 100;
	exit_game = 0;

	// init avr_umax
	new_game();
}

/**
 * Lets user select square
 * @param *cursor pointer to cursor (stores selected square)
 * @param unsigned char joypadNo number of scanned joypad
 * @return uint8_t selected square
 */
uint8_t select_square(uint8_t *cursor, unsigned char joypadNo) {
	unsigned int buttons;

	for (;;) {
		// exit game ?
        MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_RETURN //Rompemos bucle para cargar otro juego		
        
		if ((ReadJoypad(0) | ReadJoypad(1)) & BTN_SELECT) 
        {
            //uzeboxDelayCPU(); //Para que no se bloquee                           
            
			exit_game = 1;
			return 0;
		}

		// read joypad status
		buttons = ReadJoypad(joypadNo);
        //uzeboxDelayCPU(); //Para que no se bloquee		

		// move cursor
		if (buttons & (BTN_LEFT | BTN_RIGHT | BTN_UP | BTN_DOWN)) {
			if (buttons & BTN_LEFT) {
				if (((*cursor) & 0x07) > 0) {
					(*cursor) -= 1;
					play_sound(SOUND_MOVE_CURSOR);
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
			if (buttons & BTN_RIGHT) {
				if (((*cursor) & 0x07) < 0x07) {
					(*cursor) += 1;
					play_sound(SOUND_MOVE_CURSOR);
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
			if (buttons & BTN_UP) {
				if (((*cursor) & 0x70) > 0) {
					(*cursor) -= 16;
					play_sound(SOUND_MOVE_CURSOR);
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
			if (buttons & BTN_DOWN) {
				if (((*cursor) & 0x70) < 0x70) {
					(*cursor) += 16;
					play_sound(SOUND_MOVE_CURSOR);
				} else {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
			}
		}
		blinking1 = *cursor;

		// select square
		if (buttons & (BTN_A | BTN_B | BTN_X | BTN_Y | BTN_SL | BTN_SR)) {
			// wait until release and return
			play_sound(SOUND_SELECT);
			while (ReadJoypad(joypadNo) & (BTN_A | BTN_B | BTN_X | BTN_Y | BTN_SL | BTN_SR)) 
            {
				draw_board();
				//uzeboxDelayCPU(); //Para que no se bloquee
                MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_RETURN //Rompemos bucle para cargar otro juego
			};
			return *cursor;
		}

		// wait until key release or return
		uint8_t i = 0;
		while ((buttons = ReadJoypad(joypadNo))) 
        {
            //uzeboxDelayCPU(); //Para que no se bloquee
              
			draw_board();
			i++;
			if (i > 10) {
				break;
			}
            MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_RETURN //Rompemos bucle para cargar otro juego
		};
		// refresh view
		draw_board();
	}
}

/**
 * Reads move from joypad and saves it into c global variable as string and into p saves end position
 */
void read_move() {
	uint8_t *cursor;
	uint8_t *mode;
	unsigned char joypadNo;
	uint8_t source, target;

	blinking1 = 0xFF;
	blinking2 = 0xFF;

	// which side to turn and which controller
	if ((k & 16)) {
		cursor = &p1cursor;
		mode = &p1mode;
	} else {
		cursor = &p2cursor;
		mode = &p2mode;
	}

	if (((*mode) > 1) || (is_demo)) { // computer play
		// set computer level
		if (((*mode) == 2) || (is_demo)) {
			T = 0x20;
			ZMAX = 3;
			w[0]=0; w[1]=1; w[2]=1; w[3]=1; w[4]=-1; w[5]=1; w[6]=1; w[7]=1;
		} else if ((*mode) == 3) {
			T = 0x40;
			ZMAX = 4;
			w[0]=0; w[1]=2; w[2]=2; w[3]=7; w[4]=-1; w[5]=8; w[6]=12; w[7]=23;
		} else if ((*mode) == 4) {
			T = 0x80;
			ZMAX = 5;
			w[0]=0; w[1]=2; w[2]=2; w[3]=7; w[4]=-1; w[5]=8; w[6]=12; w[7]=23;
		} else if ((*mode) == 5) {
			T = 0x400;
			ZMAX = 6;
			w[0]=0; w[1]=2; w[2]=2; w[3]=7; w[4]=-1; w[5]=8; w[6]=12; w[7]=23;
		}

		// prepare computer move animation
		draw_board();
		was_computer_move = 1;

		// empty input means auto-pilot
		c[0] = 10;
		c[1] = 0;
		p = c + 2;

	} else { // human play
		// detect which joypad
		joypadNo = (*mode);

		// do user interaction
		do {
			// select source square (only allow click to own piece)
			do {
				source = select_square(cursor, joypadNo);
				//JJ if (((b[source] & 0b00011000) != (k^24)) && (!exit_game)) 
				if (((b[source] & 0x18) != (k^24)) && (!exit_game))
                {
					play_sound(SOUND_CANT_MOVE_CURSOR);
				}
				
  			 //uzeboxDelayCPU(); //Para que no se bloquee
             MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
			} while (((b[source] & 0x18) != (k^24)) && (!exit_game));
            //JJ while (((b[source] & 0b00011000) != (k^24)) && (!exit_game));
			if (exit_game) {
				return;
			}
			blinking2 = source;
			draw_board();

			// select target square
			target = select_square(cursor, joypadNo);
			blinking1 = 0xFF;
			blinking2 = 0xFF;
			draw_board();

         //uzeboxDelayCPU(); //Para que no se bloquee
         MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
         
		} while ((target == source) && (!exit_game)); // repeat (cancel move)
		if (exit_game) {
			return;
		}

		// set stack level to be able to check move
		ZMAX = 3;

		// convert source and target to move string
		c[0] = 'a' + (source & 0x07);
		c[1] = '8' - ((source & 0x70) >> 4);
		c[2] = 'a' + (target & 0x07);
		c[3] = '8' - ((target & 0x70) >> 4);
		c[4] = 10;
		c[5] = 0;
		p = c + 6;

		// save old status to detect change by engine (wrong move)
		test_c[0] = c[0];
		test_c[1] = c[1];
		test_c[2] = c[2];
		test_c[3] = c[3];

		// promotion (source is pawn and target is top or bottom line)
		if ((((b[source] & 7) == 1) || ((b[source] & 7) == 2)) && (c[3] == '1' || c[3] == '8')) {

			// ask for under-promotion
			uint8_t selected_piece = 0;
			do {
				DrawMap2(BOARD_PROMOTION_X, BOARD_PROMOTION_Y, game_promotion);
				DrawMap2(BOARD_PROMOTION_X + 1, BOARD_PROMOTION_Y + 1, pieces_map[(c[3]=='1')][0][PIECE_TYPE_QUEEN]);
				DrawMap2(BOARD_PROMOTION_X + 1 + BOARD_SQUARE_SIZE_X, BOARD_PROMOTION_Y + 1, pieces_map[(c[3]=='1')][0][PIECE_TYPE_KNIGHT]);
				DrawMap2(BOARD_PROMOTION_X + 1 + BOARD_SQUARE_SIZE_X * 2, BOARD_PROMOTION_Y + 1, pieces_map[(c[3]=='1')][0][PIECE_TYPE_ROOK]);
				DrawMap2(BOARD_PROMOTION_X + 1 + BOARD_SQUARE_SIZE_X * 3, BOARD_PROMOTION_Y + 1, pieces_map[(c[3]=='1')][0][PIECE_TYPE_BISHOP]);
				if (blink_status & 8) {
					DrawMap2(BOARD_PROMOTION_X + 1 + BOARD_SQUARE_SIZE_X * selected_piece, BOARD_PROMOTION_Y + 1, blink_piece_map);
				}
				WaitVsync(1);
				blink_status++;
				if (ReadJoypad(joypadNo) & BTN_LEFT) {
					if (selected_piece > 0) {
						selected_piece--;
						play_sound(SOUND_MOVE_CURSOR);
						while (ReadJoypad(joypadNo) & BTN_LEFT) 
                        {
                         //uzeboxDelayCPU(); //Para que no se bloquee                              
                         MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
                        };
					} else {
						play_sound(SOUND_CANT_MOVE_CURSOR);
					}
				}
				if (ReadJoypad(joypadNo) & BTN_RIGHT) {
					if (selected_piece < 3) {
						selected_piece++;
						play_sound(SOUND_MOVE_CURSOR);
						while (ReadJoypad(joypadNo) & BTN_RIGHT) 
                        {
                         //uzeboxDelayCPU(); //Para que no se bloquee                              
                         MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
                        };
					} else {
						play_sound(SOUND_CANT_MOVE_CURSOR);
					}
				}
				
           	 //uzeboxDelayCPU(); //Para que no se bloquee		
             MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
			} while (!(ReadJoypad(joypadNo) & (BTN_A | BTN_B | BTN_X | BTN_Y | BTN_SL | BTN_SR)));
			play_sound(SOUND_SELECT);
			while(ReadJoypad(joypadNo)) 
            {
             //uzeboxDelayCPU(); //Para que no se bloquee                                        
             MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
            };

			// save selected promotion as part of command for engine
			switch (selected_piece) {
			case 1:
				c[4] = 'N';
				break;
			case 2:
				c[4] = 'R';
				break;
			case 3:
				c[4] = 'B';
				break;
			case 0:
			default:
				c[4] = 'Q';
				break;
			}
			c[5] = 10;
			c[6] = 0;
		}
	}
}

/**
 * Draws piece of specified type and color on specified position on board
 *
 * @param uint8_t x x-axis coordinate (a-f)
 * @param uint8_t y y-axis coordinate (8-1 !!)
 * @param uint8_t piece_type type of piece
 * @param uint8_t piece_color color of piece
 * @return void
 */
void draw_piece(uint8_t x, uint8_t y, uint8_t piece_type, uint8_t piece_color) {
	char *piece_map;
	uint8_t square_color = ((x - y) % 2) ? SQUARE_COLOR_BLACK : SQUARE_COLOR_WHITE;

	piece_map = (char*)pieces_map[piece_color][square_color][piece_type];
	if (((blinking1 == (y * 16 + x)) || (blinking2 == (y * 16 + x))) && (blink_status & 8)) {
		piece_map = (char*)blink_piece_map;
	}

	DrawMap2(
		(x * BOARD_SQUARE_SIZE_X + BOARD_OFFSET_X),
		(y * BOARD_SQUARE_SIZE_Y + BOARD_OFFSET_Y),
		piece_map
	);
}

/**
 * Draws whole board (from global variable b of avr-micro-max engine)
 * @return void
 */
void draw_board() {
	uint8_t type;
	uint8_t color;
	char data;
	uint8_t pieces_count_now = 0;
	uint8_t nonpawn_pieces_count_now = 0;

	blink_status++;
	WaitVsync(1);

	// for each square
	for (uint8_t y = 0; y < 8; y++) {
		for (uint8_t x = 0; x < 8; x++) {
			// read data in global variable b
			data = b[y * 16 + x];

			// convert data to piece type and color
			switch (data & 7) {
			case 0:
			default:
				type = PIECE_TYPE_NONE;
				break;
			case 1:
			case 2:
				type = PIECE_TYPE_PAWN;
				pieces_count_now++;
				break;
			case 3:
				type = PIECE_TYPE_KNIGHT;
				pieces_count_now++;
				nonpawn_pieces_count_now++;
				break;
			case 4:
				type = PIECE_TYPE_KING;
				pieces_count_now++;
				nonpawn_pieces_count_now++;
				break;
			case 5:
				type = PIECE_TYPE_BISHOP;
				pieces_count_now++;
				nonpawn_pieces_count_now++;
				break;
			case 6:
				type = PIECE_TYPE_ROOK;
				pieces_count_now++;
				nonpawn_pieces_count_now++;
				break;
			case 7:
				type = PIECE_TYPE_QUEEN;
				pieces_count_now++;
				nonpawn_pieces_count_now++;
				break;
			}

			color = (data & 8) ? PIECE_COLOR_WHITE : PIECE_COLOR_BLACK;

			// draw this piece
			draw_piece(x, y, type, color);
		}
	}

	// print "DEMO" info
	if (is_demo) {
		SetTile(32, 26, GAME_FONT_D);
		SetTile(33, 26, GAME_FONT_E);
		SetTile(34, 26, GAME_FONT_M);
		SetTile(35, 26, GAME_FONT_O);
	}

	// capture
	if (pieces_count_now < pieces_count) {
		play_sound(SOUND_CAPTURE);
		rule50 = 100;
	}

	// promotion
	if (nonpawn_pieces_count_now > nonpawn_pieces_count) {
		play_sound(SOUND_PROMOTION);
	}

	// save new counts
	pieces_count = pieces_count_now;
	nonpawn_pieces_count = nonpawn_pieces_count_now;
}

/**
 * Prints current move into game log
 *
 * @return 1 if game has ended, otherwise 0
 */
uint8_t print_log() {
	uint16_t y;

	// don't log zero move
	if (move_count == 0) {
		move_count = 1;
		return 0;
	}

	// user move test at first
	if (!was_computer_move) {
		if ((test_c[0] != c[0]) || (test_c[1] != c[1]) || (test_c[2] != c[2]) || (test_c[3] != c[3])) {
			// wrong move (was overwriten by engine) - play sound and don't write log
			play_sound(SOUND_MOVE_ERROR);
			return 0;
		}
	}

	// increment move number
	move_count++;

	y = move_count;
	if (y > 25) {
		y = 25;
		// scroll log up
		WaitVsync(1);
		//printf("vram directo\n");
		//fflush(stdout);
		for (uint8_t i = 28; i < 40; i++) {
			for (uint8_t j = 2; j < 26; j++) {
				vram[(i + j * 40)] = vram[(i + (j + 1) * 40)];
			}
		}
	}

	// moving pawn resets rule50 counter
	if (((b[(c[2] - 'a') + (('8' - c[3]) << 4)] & 7) == 1) || ((b[(c[2] - 'a') + (('8' - c[3]) << 4)] & 7) == 2)) {
		rule50 = 100;
	}
	rule50--;

	// print move number
	if (!(move_count % 2)) {
		int tmp = (move_count / 2) % 1000;
		SetTile(30, y, GAME_FONT_0 + tmp % 10);
		tmp /= 10;
		if (tmp > 0) {
			SetTile(29, y, GAME_FONT_0 + tmp % 10);
		}
		tmp /= 10;
		if (tmp > 0) {
			SetTile(28, y, GAME_FONT_0 + tmp % 10);
		}
	}

	// print source and destination
	 PrintChar(32, y, c[0] - 'a' + GAME_FONT_A);	
	 PrintChar(33, y, c[1] - '0' + GAME_FONT_0);
	 PrintChar(34, y, c[2] - 'a' + GAME_FONT_A);
	 PrintChar(35, y, c[3] - '0' + GAME_FONT_0);
	
    //PrintChar(32, y, c[0]-32); //convierto mayusculas
	//PrintChar(33, y, c[1]);
	//PrintChar(34, y, c[2]-32); //convierto mayusculas
	//PrintChar(35, y, c[3]);
   	//printf("c0:%c %d c1:%c %d c2:%c %d c3:%c %d\n",c[0],c[0],c[1],c[1],c[2],c[2],c[3],c[3]);
   	//fflush(stdout);

	// computer move - do "animation"
	if (was_computer_move) {
		blinking1 = (c[0] - 'a') + (('8' - c[1]) << 4);
		blinking2 = (c[2] - 'a') + (('8' - c[3]) << 4);
		play_sound(SOUND_COMPUTER_MOVE);
		for (uint8_t i = 0; i < 60; i++) {
			draw_board();
			break_joypad |= ReadJoypad(0) | ReadJoypad(1);
		}
		blinking1 = 0xFF;
		blinking2 = 0xFF;
		draw_board();

		// print draw (if any)
		if ((was_computer_move) && (rule50 < 0)) {
			SetTile(37, y, GAME_FONT_HALF);
			SetTile(38, y, GAME_FONT_HALF);
			play_sound(SOUND_DRAW);
			return 1;
		}

		was_computer_move = 0;
	}

	// print promotion (if any)
	if (c[4] == 'Q') {
		SetTile(36, y, GAME_FONT_QUEEN);
	}
	if (c[4] == 'R') {
		SetTile(36, y, GAME_FONT_ROOK);
	}
	if (c[4] == 'B') {
		SetTile(36, y, GAME_FONT_BISHOP);
	}
	if (c[4] == 'N') {
		SetTile(36, y, GAME_FONT_KNIGHT);
	}

	// print check/mate (if any)
	ZMAX = 4;
	k ^= 24;
	// check?
	if (D(-I,I,0,S,S,1) == I) {
		SetTile(37, y, GAME_FONT_CHECK);
		play_sound(SOUND_CHECK);
		k ^= 24;
		K = I;
		// mate?
		if (!(D(-I,I,0,S,S,3)>-I+1)) {
			SetTile(38, y, GAME_FONT_CHECK);
			play_sound(SOUND_MATE);
			return 1;
		}
		k ^= 24;
	} else {
		// stalemate?
		k ^= 24;
		D(-I,I,0,S,S,3);
		k ^= 24;
		if ((c[0] == c[2]) && (c[1] == c[3])) {
			SetTile(37, y, GAME_FONT_HALF);
			SetTile(38, y, GAME_FONT_HALF);
			play_sound(SOUND_DRAW);
			return 1;
		}
	}
	k ^= 24;

	return 0;
}

/**
 * Switches UI into thinking mode
 */
void start_thinking() {
	if ((settings & SETTINGS_TURBO) && (was_computer_move) && (!is_demo)) {
		WaitVsync(1);
		SetRenderingParameters(1, 1);
		WaitVsync(1);

		//WaitVsync(100);//Fix
		
		SetTileTable(thinking);
		DrawMap2(0, 0, thinking_map);
		SetRenderingParameters(110, 16);
		
		//WaitVsync(100); //Fix
	}
}

/**
 * Switches UI back from thinking mode
 */
void stop_thinking() {
	if (settings & SETTINGS_TURBO) {
		WaitVsync(1);
		SetRenderingParameters(1, 1);
		WaitVsync(1);
		
		//WaitVsync(100);//Fix
		
		SetTileTable(game);
		DrawMap2(0, 0, game_board_top);
		SetRenderingParameters(20, 224);
		
		//WaitVsync(100);//Fix
	}
}




void do_chess4uzebox_Setup()
{
 ClearScreen(); //Borro SDL surface
      
 //He cambiado el DEMO_COUNTDOWN_TOP chess4uzebox.h 600 a 50 para la espera del menu inicial
 gb_uze_cur_FontTilesIndex=0;
 gb_uze_cur_TileTable= NULL; //Para el fondo
 gb_uze_cur_SpritesTileTable= NULL;
 gb_uze_font= NULL;
 
 gb_uze_video_cur_mode=5; //240x224 resolution, 6x8 pixels tiles, 40x28 vram
 SetVideoMode(gb_uze_video_cur_mode); //Forzamos SCREEN_WIDTH y HEIGHT
 
 SetFontTable(font); //Fuerzo a usar
 
 //Clear the screen (fills the vram with tile zero)
 ClearVram();
// srand(0x365e);
 //#ifdef use_lib_log_serial
 // Serial.printf("do_chess4uzebox_Setup\n");
 //#endif  
 WaitVsync(50);//Esperamos por sincronizacion
}


/**
 * Main code
 */
//int main() 
void do_chess4uzebox_main()
{
	struct EepromBlockStruct eeprom_data;

	// todo original music
	InitMusicPlayer(patches);

	// default configuration
	p1mode = 0;
	p2mode = 2;
	settings = SETTINGS_SOUND | SETTINGS_MUSIC;
	mysrand(0);

	// read custom configuration from eeprom
	if (EepromReadBlock(EEPR_BLOCK_ID, &eeprom_data) == 0) {
		p1mode = (eeprom_data.data[EEPR_PMODE_POS] & 0xF0) >> 4;
		p2mode = (eeprom_data.data[EEPR_PMODE_POS] & 0x0F);
		settings = (eeprom_data.data[EEPR_SETTINGS_POS] & 0x0F);
		mysrand(eeprom_data.data[EEPR_RANDOM1_POS] << 8 | eeprom_data.data[EEPR_RANDOM2_POS]);
	}

	// main loop
	for (;;) {
		//play_song(music_menu);
		MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
		
		show_menu();
		// save changed settings to eeprom
		eeprom_data.id = EEPR_BLOCK_ID;
		eeprom_data.data[EEPR_PMODE_POS] = (p1mode << 4) | p2mode;
		eeprom_data.data[EEPR_SETTINGS_POS] = settings;
		eeprom_data.data[EEPR_RANDOM1_POS] = r & 0xFF00;
		eeprom_data.data[EEPR_RANDOM2_POS] = r & 0x00FF;
		EepromWriteBlock(&eeprom_data);

		if (!is_demo) {
			play_song((char*)music);
		}
		init_game();
		play_game();
		if (!exit_game && !is_demo) {
			while (ReadJoypad(0) | ReadJoypad(1)) 
            {
             //uzeboxDelayCPU(); //Para que no se bloquee                  
             MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
            };
			while (!(ReadJoypad(0) | ReadJoypad(1))) 
            {
             //uzeboxDelayCPU(); //Para que no se bloquee                  
             MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
            };
		}
		if (is_demo)
        {
         WaitVsync(240);
		}

		StopSong();
		
     MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX //Rompemos bucle para cargar otro juego
     		
	} //fin for
}

#endif

