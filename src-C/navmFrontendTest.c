#include "navmConfig.h"

int main (void)
  {pNavmBackend test = navmInitBackendHandler (0xFFFF);
     test->fDisAsm = true;

     printf ("start\n");
     
     uOpcode testOpcode;
       testOpcode.aOpcodes[0] = 0;   // liLI
       testOpcode.aOpcodes[1] = 214; // ADD
       testOpcode.aOpcodes[2] = 80;  // liMUL
       testOpcode.aOpcodes[3] = 96;  // liSHL       
       testOpcode.aOpcodes[4] = 209; // BR
     
     sWord testImm[4]; pImm = (pWord) testImm;
       pImm[0] = 0x101; pImm[1] = 0x202; pImm[2] = 0x309; pImm[3] = 0x04;
            
     navmFrontendDynScalRegShed (test, testOpcode);
     //navmFrontendDynScal        (test, testOpcode);
       
   navmFreeBackendHandler (test);
   return 0;}
