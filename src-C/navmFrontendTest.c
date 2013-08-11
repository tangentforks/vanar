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
       testOpcode.aOpcodes[4] = 0;   // liLI
       testOpcode.aOpcodes[5] = 214; // ADD
       testOpcode.aOpcodes[6] = 80;  // liMUL
       testOpcode.aOpcodes[7] = 96;  // liSHL

     sWord testImm[30]; pImm = (pWord) testImm;
       pImm[0] = (sWord) testOpcode.bundle;
       pImm[1] = 0x101; pImm[2] = 0x202; pImm[3] = 0x303; pImm[4] = 0x04;
       pImm[5] = 0x404; pImm[6] = 0x505; pImm[7] = 0x606; pImm[8] = 0x04;

       testOpcode.aOpcodes[0] = 0;   // liLI
       testOpcode.aOpcodes[1] = 214; // ADD
       testOpcode.aOpcodes[2] = 80;  // liMUL
       testOpcode.aOpcodes[3] = 96;  // liSHL
       testOpcode.aOpcodes[4] = 209; // BR

       pImm[9]  = (sWord) testOpcode.bundle;
       pImm[10] = 0x707; pImm[11] = 0x808; pImm[12] = 0x909; pImm[13] = 0x04;

     navmFrontendCompile (test);
     //navmFrontendDynScalRegShed (test, testOpcode);
     //navmFrontendDynScal        (test, testOpcode);

     navmBackendExecute (test, 0);

   navmFreeBackendHandler (test);
   return 0;}
