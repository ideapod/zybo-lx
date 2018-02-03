

#if defined(ARM7)
#include <arm_neon.h>
#define NEON
#endif

#if defined(SCRYPT_TEST_SPEED)
size_t cpu_detect_mask = (size_t)-1;
#endif

#if defined(OS_LINUX)


/* this is what a /proc/cpuinfo looks like from a zynq
root@zplx:~# cat /proc/cpuinfo
processor	: 0
model name	: ARMv7 Processor rev 0 (v7l)
BogoMIPS	: 650.00
Features	: half thumb fastmult vfp edsp neon vfpv3 tls vfpd32 
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x3
CPU part	: 0xc09
CPU revision	: 0

processor	: 1
model name	: ARMv7 Processor rev 0 (v7l)
BogoMIPS	: 650.00
Features	: half thumb fastmult vfp edsp neon vfpv3 tls vfpd32 
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x3
CPU part	: 0xc09
CPU revision	: 0

Hardware	: Xilinx Zynq Platform
Revision	: 0003
Serial		: 0000000000000000

*/

/*
mark@mark-VirtualBox:~$ cat /proc/cpuinfo
processor	: 0
vendor_id	: GenuineIntel
cpu family	: 6
model		: 37
model name	: Intel(R) Core(TM) i5 CPU       M 560  @ 2.67GHz
stepping	: 5
microcode	: 0x616
cpu MHz		: 2660.006
cache size	: 3072 KB
physical id	: 0
siblings	: 1
core id		: 0
cpu cores	: 1
apicid		: 0
initial apicid	: 0
fpu		: yes
fpu_exception	: yes
cpuid level	: 11
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 syscall nx rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid pni pclmulqdq monitor ssse3 cx16 sse4_1 sse4_2 x2apic popcnt aes hypervisor lahf_lm pti
bugs		: cpu_insecure
bogomips	: 5320.01
clflush size	: 64
cache_alignment	: 64
address sizes	: 36 bits physical, 48 bits virtual
power management:
*/


static size_t
detect_cpu(void) {
   
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
#if defined(SCRYPT_TEST_SPEED)
	cpu_flags |= cpu_detect_mask;
#endif

   return cpu_flags;
}
#endif // OS_LINUX
//struct cpu;
