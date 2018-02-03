#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cpu_neon 0x01
#define cpu_vfp3 0x02
#define cpu_armv7 0x04
typedef unsigned char bool;
#define true 1
#define false 0

int main(int argc, char **argv)
{
   FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
   char *arg = 0;
   size_t size = 0;
   bool flagsinfo = false;
   bool featuresinfo = false;
   bool modelinfo = false;
   size_t cpu_flags = 0;
   while(getline(&arg, &size, cpuinfo) != -1)
   {
      	puts(arg);
	      char * pch;
	      printf ("Splitting string \"%s\" into tokens:\n",arg);
  	    pch = strtok (arg," :");
        if (pch != NULL) {
          if (strcmp(pch,"flags")== 0)
             flagsinfo = true;
          if (strcmp(pch, "Features") == 0)
             featuresinfo = true;
          if (strcmp(pch, "model") == 0)
             modelinfo = true;  
        }
        
      	while (pch != NULL)
      	{
      		printf ("%s\n",pch);
      		
          if (featuresinfo == true) {
            if (strcmp(pch, "neon") == 0) {
              cpu_flags |= cpu_neon; 
            }

            if (strcmp(pch, "vfpv3") == 0) {
              cpu_flags |= cpu_vfp3; 
            }
          }
          if (modelinfo == true) {
            if (strcmp(pch, "ARMv7") == 0) {
              cpu_flags != cpu_armv7; 
            }               
          }
          pch = strtok (NULL, " ,.-");
      	}
        flagsinfo = false;
        featuresinfo = false;
        modelinfo = false;
   }
   free(arg);
   fclose(cpuinfo);

   return cpu_flags;
}
