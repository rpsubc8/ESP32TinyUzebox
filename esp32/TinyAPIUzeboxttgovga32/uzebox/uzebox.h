//#pragma once
#ifndef _UZEBOX_H
 #define _UZEBOX_H
 #include "gbConfig.h"
 #include "hardware.h"
 //#include "MartianVGA.h"

 #define PROGMEM
 #define PSTR
 
 //Rompemos bucle para cargar otro juego 
 #define MACRO_UZEBOX_BREAK_MAIN  if (gb_load_new_rom == 1){ return; }
 #define MACRO_UZEBOX_BREAK_MAIN_RETURN  if (gb_load_new_rom == 1){ return 0; }
 #define MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX  if (gb_load_new_rom == 1){ exit_game=1; is_demo=0; return; }
 #define MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_RETURN  if (gb_load_new_rom == 1){ exit_game=1; is_demo=0; return 0; }
 #define MACRO_UZEBOX_BREAK_MAIN_CHESS4UZEBOX_COUNTDOWN if (gb_load_new_rom == 1){demo_countdown=0; exit_game=1; is_demo=0; return; }
 
 #define u8 unsigned char
 

 //BEGIN SOUND
	//Patch commands
	#define PC_ENV_SPEED	 0
	#define PC_NOISE_PARAMS	 1
	#define PC_WAVE			 2
	#define PC_NOTE_UP		 3
	#define PC_NOTE_DOWN	 4
	#define PC_NOTE_CUT		 5
	#define PC_NOTE_HOLD 	 6
	#define PC_ENV_VOL		 7
	#define PC_PITCH		 8
	#define PC_TREMOLO_LEVEL 9
	#define PC_TREMOLO_RATE	10
	#define PC_SLIDE		11
	#define PC_SLIDE_SPEED	12
	#define PC_LOOP_START	13
	#define PC_LOOP_END		14
	#define PATCH_END		0xff
    
    
   	#define TRACK_FLAGS_SLIDING		8
	#define TRACK_FLAGS_ALLOCATED	16
	#define TRACK_FLAGS_PLAYING		32
	#define TRACK_FLAGS_HOLD_ENV	64
	#define TRACK_FLAGS_PRIORITY	128
    
	struct  PatchStruct{   
   		unsigned char type;
		const char *pcmData;
		const char *cmdStream;  
		unsigned int loopStart;
		unsigned int loopEnd;   		       
	}; 
	typedef struct PatchStruct Patch;    
    
    
	struct TrackStruct
	{
		unsigned char flags;		//b0-b2: reserved
									//b3: pitch slide		: 1=sliding to note, 0=slide off
									//b4: allocated 		: 1=used by music player, 0=voice can be controlled by main program
									//b5: patch playing 	: 1=playing, 0=stopped
									//b6: hold envelope		: 1=hold volume envelope, i.e: don't increae/decrease, 0=don't hold
									//b7: priority			: 1=Hi/Fx, 0=low/regular note

		unsigned char note;
		unsigned char channel;

		#if MUSIC_ENGINE == MOD
			const char *patternPos;
		#else
			unsigned char expressionVol;
		#endif

		unsigned char loopCount; 

		short int slideStep;		//used to slide to note
		unsigned char slideNote;		//target note
		unsigned char slideSpeed;		//fixed point 4:4, 1:0= 1/16 half note per frame

		unsigned char tremoloPos;
		unsigned char tremoloLevel;
		unsigned char tremoloRate;

		unsigned char trackVol;
		unsigned char noteVol;
		unsigned char envelopeVol;		//(0-255)
		char envelopeStep;				//signed, amount of envelope change each frame +127/-128
		
		unsigned char patchNo;
		unsigned char fxPatchNo;
		unsigned char patchNextDeltaTime;
		unsigned char patchCurrDeltaTime;
		unsigned char patchPlayingTime;	//used by fx to steal oldest voice
		const char *patchCommandStreamPos;
		
	};
	typedef struct TrackStruct Track;     
	
	
	 	

  void InitMusicPlayer(const struct PatchStruct *patchPointersParam);   	
  void ResumeSong(void);
  void SetMasterVolume(unsigned char vol);
  void StartSong(const char *midiSong);
  void StopSong(void);
  void TriggerFx(unsigned char patch,unsigned char volume, unsigned char retrig);
  void EnableSoundEngine(void);
  void DisableSoundEngine(void);
  void TriggerNote(unsigned char channel,unsigned char patch,unsigned char note,unsigned char volume);
 //END SOUND
 
 
 	//Joypad standard buttons mappings.
	//Applies to both NES & SNES gamepads.
	#define TYPE_SNES 0
	#define TYPE_NES 1

	#if JOYSTICK == TYPE_SNES
		#define BTN_SR	   2048
		#define BTN_SL	   1024
		#define BTN_X	   512
		#define BTN_A	   256
		#define BTN_RIGHT  128
		#define BTN_LEFT   64
		#define BTN_DOWN   32
		#define BTN_UP     16
		#define BTN_START  8
		#define BTN_SELECT 4
		#define BTN_Y      2
		#define BTN_B      1
	#elif JOYSTICK == TYPE_NES
		#define BTN_SR	   2048 //unused
		#define BTN_SL	   1024 //unused		
		#define BTN_X	   512 //unused
		#define BTN_Y      256 //unused

		#define BTN_RIGHT  128
		#define BTN_LEFT   64
		#define BTN_DOWN   32
		#define BTN_UP     16
		#define BTN_START  8
		#define BTN_SELECT 4
		#define BTN_B      2
		#define BTN_A      1
	#endif 

	#define BTN_MOUSE_LEFT 512
	#define BTN_MOUSE_RIGHT 256

	#define MOUSE_SENSITIVITY_LOW    0b00
	#define MOUSE_SENSITIVITY_MEDIUM 0b10
	#define MOUSE_SENSITIVITY_HIGH   0b01
 
 
 //BEGIN SPRITES
	//Screen sections flags
	#define SCT_PRIORITY_BG  0
	#define SCT_PRIORITY_SPR 1
     
	//Sprites Options
	#define SPR_OVERFLOW_CLIP   0
	#define SPR_OVERFLOW_ROTATE 1
	#define SPR_FLIP_X 1
	#define SPR_FLIP_Y 2 
 //END SPRITES
 
 
 void SetVideoMode(unsigned char aMode);
 
 //char pgm_read_byte(const char *ptr);
 //unsigned char pgm_read_byte(const unsigned char *ptr);
 //unsigned short int pgm_read_word(const char *ptr);

 char uze_pgm_read_byte(const char *ptr);
 //unsigned char uze_pgm_read_byte_unsigned_char(const unsigned char *ptr);
 
 void WaitVsync(int count);
 //BEGIN Video Functions
 void ClearVram(void);
 void DibujaFondo(void);
 void DrawMap2(unsigned char x,unsigned char y,const char *map);
 void FadeIn(unsigned char speed,unsigned char blocking);
 void FadeOut(unsigned char speed,unsigned char blocking);
 void Fill(int x,int y,int width,int height,int tile);
 void MapSprite(unsigned char startSprite,const char *map);
 void MoveSprite(unsigned char startSprite,unsigned char x,unsigned char y,unsigned char width,unsigned char height);
 void SetSpriteVisibility(unsigned char visible);
 void SetSpritesOptions(unsigned char params);
 //END Video Functions

 void PutPixel(int x,int y,unsigned char aColor);
 
 void SetFontTable(const char *data);
 void PrintChar(int x, int y, char c);
 void uze_Print(int x, int y, const char *str);
 void SetFontTilesIndex(unsigned char index);
 void SetFont(char x,char y, unsigned char tileId);


 void SetTileTable(const char *data);
 void SetSpritesTileTable(const char *data);
 void SetTile(char x,char y, unsigned int tileId);
 void SetRenderingParameters(unsigned char firstScanlineToRender, unsigned char verticalLinesToRender);
 
 //BEGIN Controllers Functions
 void EnableSnesMouse(unsigned char spriteIndex,const char *spriteMap);
 unsigned int ReadJoypad(unsigned char joypadNo);
 unsigned char DetectControllers(void);
 //END Controllers Functions

 
 void do_exit(void);
 
 void uzeboxDelayCPU(void);










//BEGIN Video Mode 2
 struct SpriteStruct
 {
  unsigned char x;
  unsigned char y;
  unsigned char tileIndex;
 };

 struct ScreenSectionStruct
 {
  //user defined
  unsigned char scrollX;
  unsigned char scrollY;
  unsigned char height;
  unsigned char *vramBaseAdress;
  const char *tileTableAdress;
  unsigned char wrapLine;
  unsigned char flags;
  //calculated (don't write to)
  unsigned char scrollXcoarse;
  unsigned char scrollXfine;		
  unsigned char *vramRenderAdress;
  unsigned char *vramWrapAdress;
 };

 //extern struct SpriteStruct sprites[];
 //extern struct ScreenSectionStruct screenSections[];
 extern struct SpriteStruct sprites[128];
 extern struct ScreenSectionStruct screenSections[128];
//END Video Mode 2	



//BEGIN EEPROM
 struct EepromBlockStruct {
  unsigned int id;         // Unique block id. 0xffff if block is free (assuming formatted eeprom)
  unsigned char data[30];  // Application specific data (cast to your own types)
 };
 
 char EepromReadBlock(unsigned int blockId,struct EepromBlockStruct *block);
 char EepromWriteBlock(struct EepromBlockStruct *block);
 
//END EEPROM





void ClearScreen(void); //Revisar para cambiarlo


























 // Declared vars
 //#ifdef COLOR_3B
 // #ifdef use_lib_vga_low_memory
 //  extern VGA3BitI vga;
 // #else
 //  extern VGA3Bit vga;
 // #endif
 //#else   
 // #ifdef use_lib_vga_low_memory
 //  extern VGA6BitI vga;
 // #else
 //  extern VGA6Bit vga;
 // #endif 
 //#endif

 //extern unsigned char ** ptrVGA;

 // Declared methods
 void do_keyboard();

 void sdl_fast_putpixel(int x,int y, unsigned char aColor);
#endif
