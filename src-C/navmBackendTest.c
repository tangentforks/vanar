#include "navmConfig.h"

int main (void)
  {pNavmBackend test = navmInitBackendHandler (0xFFFF);
   printf ("vD: %i\n", test->vD);
   navmFreeBackendHandler (test);
   return 0;}
