/*Program: 81866D superio user space driver
 *Descriptions: This is a superio user space driver for Fintek F81866D-I
 *Author: Ivan Chen(ivanchen@aaeon.com.tw)
 *Version: 1.0
 *Date: Jul.7 2014
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "aaeon_superio.h"
 
static const char *exe_name;

void display_help()
{
    printf( "Usage: %s [OPTION]\n"
            "\n"
            " -h, --help			display this help and exit\n"
            " -p, --pin[1,2,3,..]		GPIO pin number\n"
            " -s, --status[0,1]		GPIO status, 1:HIGH, 0:LOW\n"
            " -m, --mode[0,1]		GPIO mode, 0:INPUT, 1:OUTPUT\n"
            " -i, --display		Show all GPIO status and mode\n"
            , exe_name);
    exit(0);
}

void display_error()
{
	  printf("Please input valid parameters!\n");
	  display_help();
}

void display_gpio()
{
	int i, mode, val, superio_chip;
	superio_chip = aaeon_get_superio_chip();

	if(superio_chip==chip_f81866d)
        {
	  printf("SuperIO Chip: %s\n", "F81866d");

          printf("\n");
	  printf("\n");
	for(i=1; i<9; i++)
	  {
	    mode=aaeon_display_gpio_mode(i);
	    val=aaeon_display_gpio_value(i);
	    printf("Pin %d:\t",i);
	    if(mode==0)
	      printf("mode:input, ");
	    else 
	      printf("mode:output, ");
	    if(val==0)
	     printf("status:Low\n");
	   else 
   	     printf("status:High\n");
	  }
            }
         
 }        
void process_options( int argc, char *argv[] )
{
	 int pin=-1;
	 int mode=-1;
	 int status=-1;
	 int dio_base=0;

	exe_name=argv[0];
	 
    for(;;) {
        int option_index = 0;
        static const char* short_options="hm:p:s:i";
	static const struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"mode", 1, 0, 'm'},
		{"pin", 1, 0, 'p'},
		{"status", 1, 0, 's'},
		{"display", no_argument, 0, 'i'},
		{0, 0, 0, 0},
	};    

        int c=getopt_long( argc, argv, short_options, long_options, &option_index );
        
        if( c==EOF ) {
            break;
        }

        switch( c ) {
          case 'h':
        	  display_help();
          break;
          case 'i':
        	  display_gpio();
          break;
          case 'p':
            pin = atoi(optarg);
          break;
          case 'm':
            mode = atoi(optarg);
          break;
          case 's':
            status = atoi(optarg);
          break;    
 
        }
    }
      

    if(pin > -1)
    {
	if(mode>-1)
	  aaeon_set_gpio_mode(pin, mode);
	if(status>-1)
	  aaeon_set_gpio_value(pin, status);
    }
	  
    if( argc ==1 ) {
      fprintf( stderr, "Try `%s --help' for more information.\n", exe_name );
      exit( 1 );
    }
	  
}


int main(int argc, char *argv[])
{

      if(aaeon_open_superio()<0)
      {
	printf("Aaeon superio open error!\n");
        exit(1);
      }

      process_options( argc, argv );
 
      if(aaeon_close_superio()<0)
      {
	printf("Aaeon superio close error!\n");
        exit(1);
      }
	
      return 0;
}


