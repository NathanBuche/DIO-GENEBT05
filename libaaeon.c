#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

int aaeon_check_protection()
{
   unsigned int base, basepage, baseoff, pagesize, i;
   unsigned char* mem;
   int fd;

   if ((fd = open("/dev/mem",O_RDWR)) == 0) {
     printf("Error while opening /dev/mem\n");
     return -1;
   }
    
   pagesize = getpagesize();
   base = 0x000F0000;
   baseoff = base % pagesize;
   basepage = base - baseoff;
   
   if ((mem = (char*) mmap(0,0xffff,PROT_READ, MAP_SHARED, fd, basepage)) == (caddr_t) -1) {
     printf("Error while maping memory\n");
     return -2;
   }
   
   i=0;
   while(i < 0xffff)   
    {
	if(mem[i] == 0x22)
        {
	   if(mem[i+1] == 0x34)
	     {
		if(mem[i+2] == 0x03)
                {
			if(mem[i+3] == 0x98)
			{
			   //printf("Aaeon board check verified!\n");
			    return 0;
			}
		}
	     }
	}   
     i++;
    }
   printf("Sorry! This program can only be used on Aaeon boards!\n");
   return -1;

}

int axtoi(char *hexStg) {
    int n = 0;         // position in string
    int m = 0;         // position in digit[] to shift
    int count;         // loop index
    int intValue = 0;  // integer value of hex string
    int digit[5];      // hold values to convert
    while (n < 4) {
       if (hexStg[n]=='\0')
          break;
       if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
	  digit[n] = hexStg[n] & 0x0f;            //convert to int
       else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
          digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
       else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
          digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
       else return (-1);					//Wrong Data
      n++;
    }
    count = n;
    m = n - 1;
    n = 0;
    while(n < count) {
       // digit[n] is value of hex digit at position n
       // (m << 2) is the number of positions to shift
       // OR the bits into return value
       intValue = intValue | (digit[n] << (m << 2));
       m--;   // adjust the position to set
       n++;   // next digit to process
    }
    return (intValue);
}


