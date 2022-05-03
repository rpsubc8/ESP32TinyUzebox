#ifndef _GB_BITMAPDEMO_H
 #define _GB_BITMAPDEMO_H
 #include "../../gbConfig.h"
 #include "../../gbGlobals.h"
 
 //No existia
 //#ifdef __cplusplus
 // extern "C"
 // {
 //#endif

  //void doSetupBitmapDemo(void);
  //void doBitmapDemoMain(void);

 #ifdef use_rom_bitmapdemo
  void do_BitmapDemo_Setup(void);
  int do_BitmapDemo_main(void);
 #endif   
 
 //#ifdef __cplusplus
 //}
 //#endif

#endif
