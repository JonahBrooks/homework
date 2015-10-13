// Jonah Brooks
// CS 472
// Homework 1

#include <stdio.h>
#include <string.h>


const char* tlb(unsigned int i)
{
  switch (i)
  {
		case 0x00 : return " General: Null descriptor, this byte contains no information";
		case 0x01 : return " TLB: Instruction TLB: 4 KByte pages, 4-way set associative, 32 entries";
		case 0x02 : return " TLB: Instruction TLB: 4 MByte pages, fully associative, 2 entries";
		case 0x03 : return " TLB: Data TLB: 4 KByte pages, 4-way set associative, 64 entries";
		case 0x04 : return " TLB: Data TLB: 4 MByte pages, 4-way set associative, 8 entries";
		case 0x05 : return " TLB: Data TLB1: 4 MByte pages, 4-way set associative, 32 entries";
		case 0x06 : return " Cache: 1st-level instruction cache: 8 KBytes, 4-way set associative, 32 byte line size";
		case 0x08 : return " Cache: 1st-level instruction cache: 16 KBytes, 4-way set associative, 32 byte line size";
		case 0x09 : return " Cache: 1st-level instruction cache: 32KBytes, 4-way set associative, 64 byte line size";
		case 0x0A : return " Cache: 1st-level data cache: 8 KBytes, 2-way set associative, 32 byte line size";
		case 0x0B : return " TLB: Instruction TLB: 4 MByte pages, 4-way set associative, 4 entries";
		case 0x0C : return " Cache: 1st-level data cache: 16 KBytes, 4-way set associative, 32 byte line size";
		case 0x0D : return " Cache: 1st-level data cache: 16 KBytes, 4-way set associative, 64 byte line size";
		case 0x0E : return " Cache: 1st-level data cache: 24 KBytes, 6-way set associative, 64 byte line size";
		case 0x1D : return " Cache: 2nd-level cache: 128 KBytes, 2-way set associative, 64 byte line size";
		case 0x21 : return " Cache: 2nd-level cache: 256 KBytes, 8-way set associative, 64 byte line size";
		case 0x22 : return " Cache: 3rd-level cache: 512 KBytes, 4-way set associative, 64 byte line size, 2 lines per sector";
		case 0x23 : return " Cache: 3rd-level cache: 1 MBytes, 8-way set associative, 64 byte line size, 2 lines per sector";
		case 0x24 : return " Cache: 2nd-level cache: 1 MBytes, 16-way set associative, 64 byte line size";
		case 0x25 : return " Cache: 3rd-level cache: 2 MBytes, 8-way set associative, 64 byte line size, 2 lines per sector";
		case 0x29 : return " Cache: 3rd-level cache: 4 MBytes, 8-way set associative, 64 byte line size, 2 lines per sector";
		case 0x2C : return " Cache: 1st-level data cache: 32 KBytes, 8-way set associative, 64 byte line size";
		case 0x30 : return " Cache: 1st-level instruction cache: 32 KBytes, 8-way set associative, 64 byte line size";
		case 0x40 : return " Cache: No 2nd-level cache or, if processor contains a valid 2nd-level cache, no 3rd-level cache";
		case 0x41 : return " Cache: 2nd-level cache: 128 KBytes, 4-way set associative, 32 byte line size";
		case 0x42 : return " Cache: 2nd-level cache: 256 KBytes, 4-way set associative, 32 byte line size";
		case 0x43 : return " Cache: 2nd-level cache: 512 KBytes, 4-way set associative, 32 byte line size";
		case 0x44 : return " Cache: 2nd-level cache: 1 MByte, 4-way set associative, 32 byte line size";
		case 0x45 : return " Cache: 2nd-level cache: 2 MByte, 4-way set associative, 32 byte line size";
		case 0x46 : return " Cache: 3rd-level cache: 4 MByte, 4-way set associative, 64 byte line size";
		case 0x47 : return " Cache: 3rd-level cache: 8 MByte, 8-way set associative, 64 byte line size";
		case 0x48 : return " Cache: 2nd-level cache: 3MByte, 12-way set associative, 64 byte line size";
		case 0x49 : return " Cache: 3rd-level cache: 4MB, 16-way set associative, 64-byte line size (Intel Xeon processor MP, Family 0FH, Model 06H);\n 2nd-level cache: 4 MByte, 16-way set associative, 64 byte line size";
		case 0x4A : return " Cache: 3rd-level cache: 6MByte, 12-way set associative, 64 byte line size";
		case 0x4B : return " Cache: 3rd-level cache: 8MByte, 16-way set associative, 64 byte line size";
		case 0x4C : return " Cache: 3rd-level cache: 12MByte, 12-way set associative, 64 byte line size";
		case 0x4D : return " Cache: 3rd-level cache: 16MByte, 16-way set associative, 64 byte line size";
		case 0x4E : return " Cache: 2nd-level cache: 6MByte, 24-way set associative, 64 byte line size";
		case 0x4F : return " TLB: Instruction TLB: 4 KByte pages, 32 entries";
		case 0x50 : return " TLB: Instruction TLB: 4 KByte and 2-MByte or 4-MByte pages, 64 entries";
		case 0x51 : return " TLB: Instruction TLB: 4 KByte and 2-MByte or 4-MByte pages, 128 entries";
		case 0x52 : return " TLB: Instruction TLB: 4 KByte and 2-MByte or 4-MByte pages, 256 entries";
		case 0x55 : return " TLB: Instruction TLB: 2-MByte or 4-MByte pages, fully associative, 7 entries";
		case 0x56 : return " TLB: Data TLB0: 4 MByte pages, 4-way set associative, 16 entries";
		case 0x57 : return " TLB: Data TLB0: 4 KByte pages, 4-way associative, 16 entries";
		case 0x59 : return " TLB: Data TLB0: 4 KByte pages, fully associative, 16 entries";
		case 0x5A : return " TLB: Data TLB0: 2-MByte or 4 MByte pages, 4-way set associative, 32 entries";
		case 0x5B : return " TLB: Data TLB: 4 KByte and 4 MByte pages, 64 entries";
		case 0x5C : return " TLB: Data TLB: 4 KByte and 4 MByte pages,128 entries";
		case 0x5D : return " TLB: Data TLB: 4 KByte and 4 MByte pages,256 entries";
		case 0x60 : return " Cache: 1st-level data cache: 16 KByte, 8-way set associative, 64 byte line size";
		case 0x61 : return " TLB: Instruction TLB: 4 KByte pages, fully associative, 48 entries";
		case 0x63 : return " TLB: Data TLB: 1 GByte pages, 4-way set associative, 4 entries";
		case 0x66 : return " Cache: 1st-level data cache: 8 KByte, 4-way set associative, 64 byte line size";
		case 0x67 : return " Cache: 1st-level data cache: 16 KByte, 4-way set associative, 64 byte line size";
		case 0x68 : return " Cache: 1st-level data cache: 32 KByte, 4-way set associative, 64 byte line size";
		case 0x70 : return " Cache: Trace cache: 12 K-μop, 8-way set associative";
		case 0x71 : return " Cache: Trace cache: 16 K-μop, 8-way set associative";
		case 0x72 : return " Cache: Trace cache: 32 K-μop, 8-way set associative";
		case 0x76 : return " TLB: Instruction TLB: 2M/4M pages, fully associative, 8 entries";
		case 0x78 : return " Cache: 2nd-level cache: 1 MByte, 4-way set associative, 64byte line size";
		case 0x79 : return " Cache: 2nd-level cache: 128 KByte, 8-way set associative, 64 byte line size, 2 lines per sector";
		case 0x7A : return " Cache: 2nd-level cache: 256 KByte, 8-way set associative, 64 byte line size, 2 lines per sector";
		case 0x7B : return " Cache: 2nd-level cache: 512 KByte, 8-way set associative, 64 byte line size, 2 lines per sector";
		case 0x7C : return " Cache: 2nd-level cache: 1 MByte, 8-way set associative, 64 byte line size, 2 lines per sector";
		case 0x7D : return " Cache: 2nd-level cache: 2 MByte, 8-way set associative, 64byte line size";
		case 0x7F : return " Cache: 2nd-level cache: 512 KByte, 2-way set associative, 64-byte line size";
		case 0x80 : return " Cache: 2nd-level cache: 512 KByte, 8-way set associative, 64-byte line size";
		case 0x82 : return " Cache: 2nd-level cache: 256 KByte, 8-way set associative, 32 byte line size";
		case 0x83 : return " Cache: 2nd-level cache: 512 KByte, 8-way set associative, 32 byte line size";
		case 0x84 : return " Cache: 2nd-level cache: 1 MByte, 8-way set associative, 32 byte line size";
		case 0x85 : return " Cache: 2nd-level cache: 2 MByte, 8-way set associative, 32 byte line size";
		case 0x86 : return " Cache: 2nd-level cache: 512 KByte, 4-way set associative, 64 byte line size";
		case 0x87 : return " Cache: 2nd-level cache: 1 MByte, 8-way set associative, 64 byte line size";
		case 0xA0 : return " DTLB: DTLB: 4k pages, fully associative, 32 entries";
		case 0xB0 : return " TLB: Instruction TLB: 4 KByte pages, 4-way set associative, 128 entries";
		case 0xB1 : return " TLB: Instruction TLB: 2M pages, 4-way, 8 entries or 4M pages, 4-way, 4 entries";
		case 0xB2 : return " TLB: Instruction TLB: 4KByte pages, 4-way set associative, 64 entries";
		case 0xB3 : return " TLB: Data TLB: 4 KByte pages, 4-way set associative, 128 entries";
		case 0xB4 : return " TLB: Data TLB1: 4 KByte pages, 4-way associative, 256 entries";
		case 0xB5 : return " TLB: Instruction TLB: 4KByte pages, 8-way set associative, 64 entries";
		case 0xB6 : return " TLB: Instruction TLB: 4KByte pages, 8-way set associative, 128 entries";
		case 0xBA : return " TLB: Data TLB1: 4 KByte pages, 4-way associative, 64 entries";
		case 0xC0 : return " TLB: Data TLB: 4 KByte and 4 MByte pages, 4-way associative, 8 entries";
		case 0xC1 : return " STLB: Shared 2nd-Level TLB: 4 KByte/2MByte pages, 8-way associative, 1024 entries";
		case 0xC2 : return " DTLB: DTLB: 4 KByte/2 MByte pages, 4-way associative, 16 entries";
		case 0xC3 : return " STLB: Shared 2nd-Level TLB: 4 KByte /2 MByte pages, 6-way associative, 1536 entries. Also 1GBbyte pages, 4-way, 16 entries.";
		case 0xCA : return " STLB: Shared 2nd-Level TLB: 4 KByte pages, 4-way associative, 512 entries";
		case 0xD0 : return " Cache: 3rd-level cache: 512 KByte, 4-way set associative, 64 byte line size";
		case 0xD1 : return " Cache: 3rd-level cache: 1 MByte, 4-way set associative, 64 byte line size";
		case 0xD2 : return " Cache: 3rd-level cache: 2 MByte, 4-way set associative, 64 byte line size";
		case 0xD6 : return " Cache: 3rd-level cache: 1 MByte, 8-way set associative, 64 byte line size";
		case 0xD7 : return " Cache: 3rd-level cache: 2 MByte, 8-way set associative, 64 byte line size";
		case 0xD8 : return " Cache: 3rd-level cache: 4 MByte, 8-way set associative, 64 byte line size";
		case 0xDC : return " Cache: 3rd-level cache: 1.5 MByte, 12-way set associative, 64 byte line size";
		case 0xDD : return " Cache: 3rd-level cache: 3 MByte, 12-way set associative, 64 byte line size";
		case 0xDE : return " Cache: 3rd-level cache: 6 MByte, 12-way set associative, 64 byte line size";
		case 0xE2 : return " Cache: 3rd-level cache: 2 MByte, 16-way set associative, 64 byte line size";
		case 0xE3 : return " Cache: 3rd-level cache: 4 MByte, 16-way set associative, 64 byte line size";
		case 0xE4 : return " Cache: 3rd-level cache: 8 MByte, 16-way set associative, 64 byte line size";
		case 0xEA : return " Cache: 3rd-level cache: 12MByte, 24-way set associative, 64 byte line size";
		case 0xEB : return " Cache: 3rd-level cache: 18MByte, 24-way set associative, 64 byte line size";
		case 0xEC : return " Cache: 3rd-level cache: 24MByte, 24-way set associative, 64 byte line size";
		case 0xF0 : return " Prefetch: 64-Byte prefetching";
		case 0xF1 : return " Prefetch: 128-Byte prefetching";
		case 0xFF : return " General: CPUID leaf 2 does not report cache descriptor information, use CPUID leaf 4 to query cache parameters";
    default : return " Unknown information";
 }
}

int main(int argc, char **argv)
{

  // I need to get each of the following:
  // L1 data cache size (with line info)
  //    EAX:04H, ECX:01H ?
  //    Results in: ??? 
  // L1 instruction cache size (with line info)
  // TLB information (any and all)
  //    EAX:02H
  //    Results in: everything
  // Memory hierarchy information
  // physical address width
  // logical address width
  // number of logical CPUs
  // CPU family
  //    EAX:01, ?
  // CPU model (number and name)
  //    EAX:01, ?
  // CPU frequency
  // CPU features available
  enum QUESTION
  { 
    VENDOR = 0,
    L1DATA,
    L1INSTRUCTION,
    TLBINFO,
    MEMHIERARCHY,
    PHYSADDRWIDTH,
    LOGICALADDRWIDTH,
    NUMLOGICALCPUS,
    CPUFAMILY,
    CPUMODEL,
    CPUFREQ,
    CPUFEATURES,  
    MAXINPUT
  };


  const char *cache_type[4];
  const char *ecxf[31];
  const char *edxf[31];
  int i;

  unsigned int eax;
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;

  char vendor[13];
  
  unsigned int tmp;
  unsigned int val;
  unsigned int msk;
  int nobreak;
  enum QUESTION q;

cache_type[0] = "Null - No more caches";
cache_type[1] = "Data Cache";
cache_type[2] = "Instruction Cache";
cache_type[3] = "Unified Cache";


ecxf[0] = " SSE3 Streaming SIMD Extensions 3 ";
ecxf[1] = " PCLMULQDQ";
ecxf[2] = " DTES64 64-bit DS Area. ";
ecxf[3] = " MONITOR MONITOR/MWAIT. ";
ecxf[4] = " DS-CPL CPL Qualified Debug Store. ";
ecxf[5] = " VMX Virtual Machine Extensions. ";
ecxf[6] = " SMX Safer Mode Extensions. ";
ecxf[7] = " EIST Enhanced Intel SpeedStep® technology. ";
ecxf[8] = " TM2 Thermal Monitor 2. ";
ecxf[9] = " SSSE3 ";
ecxf[10] = " CNXT-ID L1 Context ID. ";
ecxf[11] = " SDBG ";
ecxf[12] = " FMA ";
ecxf[13] = " CMPXCHG16B ";
ecxf[14] = " xTPR Update Control ";
ecxf[15] = " PDCM Perfmon and Debug Capability: ";
ecxf[16] = " Reserved Reserved";
ecxf[17] = " PCID Process-context identifiers. ";
ecxf[18] = " DCA ";
ecxf[19] = " SSE4.1 ";
ecxf[20] = " SSE4.2 ";
ecxf[21] = " x2APIC ";
ecxf[22] = " MOVBE ";
ecxf[23] = " POPCNT";
ecxf[24] = " TSC-Deadline ";
ecxf[25] = " AESNI ";
ecxf[26] = " XSAVE ";
ecxf[27] = " OSXSAVE ";
ecxf[28] = " AVX ";
ecxf[29] = " F16C";
ecxf[30] = " RDRAND ";
ecxf[31] = " Not Used";

edxf[0] = " FPU Floating Point Unit On-Chip. The processor contains an x87 FPU.";
edxf[1] = " VME Virtual 8086 Mode Enhancements. ";
edxf[2] = " DE Debugging Extensions. ";
edxf[3] = " PSE Page Size Extension. ";
edxf[4] = " TSC Time Stamp Counter. ";
edxf[5] = " MSR Model Specific Registers ";
edxf[6] = " PAE Physical Address Extension.";
edxf[7] = " MCE Machine Check Exception. ";
edxf[8] = " CX8 CMPXCHG8B Instruction. ";
edxf[9] = " APIC APIC On-Chip. ";
edxf[10] = " Reserved Reserved";
edxf[11] = " SEP SYSENTER and SYSEXIT Instructions.";
edxf[12] = " MTRR Memory Type Range Registers. ";
edxf[13] = " PGE Page Global Bit.";
edxf[14] = " MCA Machine Check Architecture. ";
edxf[15] = " CMOV Conditional Move Instructions. ";
edxf[16] = " PAT Page Attribute Table. ";
edxf[17] = " PSE-36 36-Bit Page Size Extension. ";
edxf[18] = " PSN Processor Serial Number. ";
edxf[19] = " CLFSH CLFLUSH Instruction. ";
edxf[20] = " Reserved Reserved";
edxf[21] = " DS Debug Store. ";
edxf[22] = " ACPI Thermal Monitor and Software Controlled Clock Facilities.";
edxf[23] = " MMX Intel MMX Technology. ";
edxf[24] = " FXSR FXSAVE and FXRSTOR Instructions. ";
edxf[25] = " SSE ";
edxf[26] = " SSE2";
edxf[27] = " SS Self Snoop. ";
edxf[28] = " HTT Max APIC IDs reserved field is Valid. ";
edxf[29] = " TM Thermal Monitor. ";
edxf[30] = " Reserved Reserved";
edxf[31] = " PBE Pending Break Enable. ";

  q = 0;
  nobreak = 1; // Set to 1 to remove breaks from the switch statement

  switch (q)
  {
    case VENDOR: eax = 0x00; // Get vendor
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            memcpy(vendor, (char*)&ebx, 4);
            memcpy(vendor + 4, (char*)&edx, 4);
            memcpy(vendor + 8, (char*)&ecx, 4);
            vendor[12] = 0;
            printf("Vendor: %s\n", vendor);
            if (!nobreak) break;
    case L1DATA: eax = 0x04; ecx = 0x00; // Get L1 Data Cache size
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax), "c"(ecx)
                       );
            msk = 0b1111111111;
            val = (((ebx>>22)&msk) + 1) * (((ebx>>12)&msk) + 1) * ((ebx&msk) + 1) * (ecx + 1);
            printf("L1 Data Cache Stats: %u Bytes, %u Ways, %u Partitions, "\
                     "%u Line_Size, %u Sets, %x EAX\n", val, 
                  (((ebx>>22)&msk) + 1) , (((ebx>>12)&msk) + 1) , ((ebx&msk) + 1) , (ecx + 1), eax);
            if (!nobreak) break;
    case L1INSTRUCTION: eax = 0x04; ecx = 0x01; // Get L1 Instruction Cache size
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax), "c"(ecx)
                       );
            msk = 0b1111111111;
            val = (((ebx>>22)&msk) + 1) * (((ebx>>12)&msk) + 1) * ((ebx&msk) + 1) * (ecx + 1);
            printf("L1 Instruction Cache Stats: %u Bytes, %u Ways, %u Partitions, "\
                     "%u Line_Size, %u Sets, %x EAX\n", val, 
                  (((ebx>>22)&msk) + 1) , (((ebx>>12)&msk) + 1) , ((ebx&msk) + 1) , (ecx + 1), eax);
            if (!nobreak) break;
    case TLBINFO: eax = 0x02; // Get TLB info
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            
            printf("TLB Info:\n");
            eax = eax>>8; // Get rid of the 01H at the end of EAX
            while (eax > 0)
            {
              printf("%s\n",tlb((eax&0xff))); // Look up and print the meaning of this byte
              eax = eax>>8; // Then remove that byte
            }
            while (ebx > 0)
            {
              printf("%s\n",tlb(ebx&0xff)); // Look up and print the meaning of this byte
              ebx = ebx>>8; // Then remove that byte
            }
            while (ecx > 0)
            {
              printf("%s\n",tlb(ecx&0xff)); // Look up and print the meaning of this byte
              ecx = ecx>>8; // Then remove that byte
            }
            while (edx > 0)
            {
              printf("%s\n",tlb(edx&0xff)); // Look up and print the meaning of this byte
              edx = edx>>8; // Then remove that byte
            }
            if (!nobreak) break;
    case MEMHIERARCHY: eax = 0x04; ecx = 0x00; // Output... memory hierarchy?
            tmp = 0;
            val = 0;
            printf("Memory Hierarchy:\n");
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax), "c"(ecx)
                       );
            while ((eax&0x1f) != 0) // If the next ECX value is invalid, EAX 4-0 will be 0
            { 
              msk = 0b1111111111;
              val = (((ebx>>22)&msk) + 1) * (((ebx>>12)&msk) + 1) * ((ebx&msk) + 1) * (ecx + 1);
              printf(" Level %u %s, size: %u bytes.\n", ((eax>>5)&0x03), cache_type[(eax&0x1f)], val);
              tmp++;
              ecx = tmp; 
              eax = 0x04;
              __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(eax), "c"(ecx)
                         );
            }
            if (!nobreak) break;
    case PHYSADDRWIDTH: eax = 0x80000008; // Get physical address width
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            msk = 0xff;
            if((eax&msk) == 0)
            {
              printf("Physical Address Width: Not Supported\n");
            }
            else
            {
              printf("Physical Address Width: %u (bits)\n", (eax&msk));
            }
            if (!nobreak) break;
    case LOGICALADDRWIDTH: eax = 0x80000008; // Get virtual address width
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            msk = 0xff;
            // If 0 output not supported
            if((ebx&msk) == 0)
            {
              printf("Logical Address Width: Not Supported\n");
            }
            else
            {
              printf("Logical Address Width: %u (bits)\n", (ebx&msk));
            }
            if (!nobreak) break;
    case NUMLOGICALCPUS: eax = 0x0B; ecx = 0x00; // Get Number of Logical CPUs
            tmp = 0;
            val = 0;
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax), "c"(ecx)
                       );
            while ( ((ecx>>8)&0xff) != 0 && ((ecx>>8)&0xff) < 3) // For every valid level type
            { 
              printf("Logical CPUs at level type %u: %u\n", ((ecx>>8)&0xff),(ebx&0xffff));
              val += (ebx&0xffff);
              tmp++;
              ecx = tmp; 
              __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(eax), "c"(ecx)
                         );
            } 
            printf("Logal CPUs total: %u\n",val);
            if (!nobreak) break;
    case CPUFAMILY: eax = 0x00; // Get CPU family
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       ); 
            // If this has 0 for (eax>>8)%15 or (eax>>12)%3, report "Not Supported"
            if(((eax>>8)%15) == 0 || ((eax>>12)%3) == 0)
            {
              printf("CPU Family: Not Supported\n");
            }
            else
            {
              // TODO: Translate these into something meaningful
              printf("CPU Family: Processor Type: %x, Family: %x (extended: %x)\n", 
                  (eax>>12)%3, (eax>>8)%15, (eax>>20)%127);
            }
            if (!nobreak) break;
    case CPUMODEL: eax = 0x00; // Get CPU model
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            // If this has 0 for (eax>>8)%15 or (eax>>12)%3, report "Not Supported"
            if(((eax>>8)%15) == 0 || ((eax>>12)%3) == 0)
            {
              printf("CPU Model: Not Supported\n");
            }
            else
            {
              // TODO: Translate these into something meaningful
              printf("CPU Model: %x (extended: %x)\n", (eax>>4)%15, (eax>>16)%15);
            }
            if (!nobreak) break;
    case CPUFREQ: eax = 0x16; // Get CPU frequencies
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            msk = 0xffff;
            // If any 0s, report that this is not supported.
            printf("Processor Frequency: Base: "); 
            if((eax&msk) == 0)
            {
              printf("Not Supported");
            }
            else
            {
               printf("%u MHz",(eax&msk));
            }
            printf(" Max: ");
            if((ebx&msk) == 0)
            {
              printf("Not Supported");
            }
            else
            {
               printf("%u MHz",(ebx&msk));
            }
            printf(" Bus: ");
            if((ecx&msk) == 0)
            {
              printf("Not Supported");
            }
            else
            {
               printf("%u MHz",(ecx&msk));
            }
            printf("\n");
            if (!nobreak) break;
    case CPUFEATURES: eax = 0x01; // Get features
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            printf("Supported features:\n");
            for(i = 0; i < 32; i++)
            {
              if(((ecx>>i)&1) == 1)
              {
                printf("%s\n",ecxf[i]);
              } 
            } 
            for(i = 0; i < 32; i++)
            {
              if(((edx>>i)&1) == 1)
              {
                printf("%s\n",edxf[i]);
              } 
            } 
            if (!nobreak) break;
    case MAXINPUT: eax = 0x00; // Get features
            __asm__ __volatile__(
                       "cpuid;"
                       : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                       : "a"(eax)
                       );
            printf("Max CPUID input: 0x%02x\n",eax);
            break;
    default: printf("Invalid question number selected");
  }  

  // Page 3-178 Vol.2A
  //
  // I need to get each of the following:
  // L1 data cache size (with line info)
  //    EAX:04H, ECX:00H ?
  //    Results in: ??? 
  // L1 instruction cache size (with line info)
  //    EAX:04H, ECX:00H ?
  //    Results in: ??? 
  // TLB information (any and all)
  //    EAX:02H
  //    Results in: everything
  // Memory hierarchy information // What? Does this just mean how many layers of cache there are?
  // physical address width
  //      EAX:80000008H
  //      results in: EAX: 07-00
  // logical address width
  //      EAX:80000008H
  //      Results in: EBX: 07-00
  // number of logical CPUs
  //      EAX:0BH ECX: 0,1,2...
  //      Results in: EBX: 15-00
  // CPU family // Not Suported
  //    EAX:01H
  //    Results in: EAX: 11-8 (and extended in 27-20) (processor type in 13-12)
  // CPU model (number and name) // Not supported
  //    EAX:01H 
  //    Results in: EAX: 7-4 (and extended in 19-16)
  // CPU frequency  // Not Supported
  //    EAX:16H
  //    Results in: Base; EAX: 15-0, Max; EBX: 15-0, Bus; ECX: 50-0
  // CPU features available
  //    EAX:01H
  //    Results in: ECX and EDX, look up in table on page 3-190 (702)

  return 0;
}
