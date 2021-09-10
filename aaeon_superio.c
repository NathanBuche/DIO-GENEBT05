/*Program: 81866D superio user space driver
 *Descriptions: This is a superio user space driver for Fintek F81866D-I
 *Author: Ivan Chen(ivanchen@aaeon.com.tw)
 *Version: 1.0
 *Date: Jul.7 2014
 */

#include "stdio.h"
#include <stdlib.h>
#include "aaeon_f81866d.h"
#include "aaeon_superio.h"

int aaeon_check_flag=-1;	//This flag is for verification of aaeon boards
int superio_chip;

int check_chip_id_f81866d()
{
	if(f81866d_enter_conf()!=-1)
        {
	return 0;
	}
	return -1;
}
/*print error message*/
void aaeon_superio_not_found()
{
    printf("No superio chip found!\n");
}

/*print error message*/
void aaeon_superio_close_error(int chip)
{
    printf("Chip %d close error\n", chip);
}

/*set dio base*/
void aaeon_set_dio_base(int diobase)
{
    if(aaeon_check_flag==0)
    {
	switch (superio_chip)
    	{
		case chip_f81866d:
	//    	  f81866d_set_dio_base(diobase);
		break;

	}
    }
}


/*get the current using dio base*/
int aaeon_get_current_dio_base()
{
    int dioid;
  if(aaeon_check_flag==0)
  {
    switch (superio_chip)
    {
	case chip_f81866d:
//	      dioid = f81866d_get_current_dio_base();
 //             return dioid ;
	break;        

    }
  }else
    return -1;
}

/*return superio chip name*/
int aaeon_get_superio_chip()
{
	return superio_chip;
}

int aaeon_display_gpio_mode(int pin)
{
    if(aaeon_check_flag==0)
    {
	switch (superio_chip)
        {
	   case chip_f81866d:
	      return f81866d_display_gpio_mode(pin);
	   break;


	}
    }
    else
    {
      aaeon_superio_not_found();
      return -1;
    }
}

int aaeon_display_gpio_value(int pin)
{
    if(aaeon_check_flag==0)
    {
	switch (superio_chip)
        {
	   case chip_f81866d:
	      return f81866d_display_gpio_val(pin);
	   break;

	}
    }
    else
    {
      aaeon_superio_not_found();
      return -1;
    }
}

void aaeon_set_gpio_value(int pin, int status)
{
    if(aaeon_check_flag==0)
    {
	switch (superio_chip)
        {
	   case chip_f81866d:
	      f81866d_set_gpio_value(pin, status);
	   break;

	}
    }
    else
      aaeon_superio_not_found();
}

void aaeon_set_gpio_mode(int pin, int mode)
{
    if(aaeon_check_flag==0)
    {
	switch (superio_chip)
        {
	   case chip_f81866d:
		f81866d_set_gpio_mode(pin, mode);
	   break;

	}
      
    }
    else
      aaeon_superio_not_found();
}

/*This function is for detecting which chip is in use and then open it */
int aaeon_open_superio()
{

          if(check_chip_id_f81866d()==0)
          {
         	if(f81866d_open()==0)
		{
            	superio_chip = chip_f81866d;  //set superio_chip flag
	    	aaeon_check_flag=0;            //aaeon check verified
            	return 0;
		}
	      else   
          	{
	        aaeon_check_flag=-1;
	        return -1;
          	}
          }
	  else   
          {
	        return -1;
          }
       
}

/*close superio*/
int aaeon_close_superio()
{

    if(check_chip_id_f81866d()==0)
     {
      f81866d_close();
     }
     else
     {
      aaeon_superio_close_error(superio_chip);
      return -1;
     }
    return 0;
}




