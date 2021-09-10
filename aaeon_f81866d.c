/*Program: 81866D superio user space driver
 *Descriptions: This is a superio user space driver for Fintek F81866D-I
 *Author: Ivan Chen(ivanchen@aaeon.com.tw)
 *Version: 1.0
 *Date: Jul.7 2014
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include "libaaeon.h"
#include <unistd.h>
#include <fcntl.h>

/* The base address is 0x2e or 0x4e, depending on config bytes. */

#define SIO_BASE		0x2E
#define SIO_DATA		0x2F

/* Configuration registers. */

#define UNLOCK_DATA			0x87
#define LOCK_DATA			0xAA
#define F81866D_GPIO			0x07
#define F81866D_GPIO_Enable_Register	0x30
#define F81866D_GPIO8_MODE	        0x88   /* GPIO8 Mode(Input, output) */
#define F81866D_GPIO8_Output_Data       0x89
#define F81866D_GPIO8_Data_Status       0x8A
#define F81866D_GPIO7_MODE	        0x80   /* GPIO7 Mode(Input, output) */
#define F81866D_GPIO7_Output_Data       0x81
#define F81866D_GPIO7_Data_Status       0x82
#define F81866D_GPIO5_MODE	        0xA0   /* GPIO5 Mode(Input, output) */
#define F81866D_GPIO5_Output_Data       0xA1
#define F81866D_GPIO5_Data_Status       0xA2
#define F81866D_IO_BASE_MSB		0x60
#define F81866D_IO_BASE_LSB		0x61



int DIO_BASE;
int gpio_id;


//select device and register
static void superio_select(int ldn)
{
	outb(F81866D_GPIO, SIO_BASE);
	outb(ldn, SIO_DATA);
}

static void superio_outb(int val, int reg)
{
	outb(reg, SIO_BASE);
	outb(val, SIO_DATA);
}

static int superio_inb(int reg)
{
	outb(reg, SIO_BASE);
	return inb(SIO_DATA);
}

static int superio_inw(int reg)
{
	outb(reg, SIO_BASE);
	return inw(SIO_DATA);
}

/*
int f81866d_get_gpio_group()
{
        int gpio_available_group;
	int groupid;
	int num=0;

	//F81866d has 5 sets of GPIO from 0x25 to 0x29
	for(groupid=0x25; groupid<0x30; groupid++)
	{
	  if(superio_inb(groupid)==0xff)
	  {
   	     if(groupid == 0x25)
               gpio_available_group=1;
	     else if(groupid == 0x26)
               gpio_available_group=2;
	     else if(groupid == 0x27)
               gpio_available_group=3;
	     else if(groupid == 0x28)
               gpio_available_group=4;
	     else if(groupid == 0x29)
               gpio_available_group=5;

	  }
          
	}

	return gpio_available_group;
}
*/
/*

int f81866d_get_current_dio_base()
{
         DIO_BASE = (superio_inb(F81866D_IO_BASE_MSB) << 8) | superio_inb(F81866D_IO_BASE_LSB);  //get default dio base 
         DIO_BASE =DIO_BASE+gpio_id;
	 return DIO_BASE;
}

*/

int f81866d_enter_conf(void)
{
	/*  Enter the configuration state (MB PnP mode). */

	if(iopl(3)<0)
	{
	  printf("IO port permission failed! You must have root privilege to execute this program!\n");
	  return -1;
	}

	/* enter config mode */
	outb(UNLOCK_DATA, SIO_BASE);
	outb(UNLOCK_DATA, SIO_BASE);
	
	return 0;
}

int f81866d_open()
{
	int c;
	//Here we check whether the program is running on Aaeon boards
        //if(w83697ug_enter_conf()<0 || aaeon_check_protection()<0)
	if(f81866d_enter_conf()<0 || aaeon_check_protection()<0)
	  return -1;
	superio_select(0x06);      //set device ldn 6
	/* set device active */
    	outb(0x30, SIO_BASE);
	c = inb(SIO_DATA);
	outb(c || 0x01, SIO_DATA); /* set bit 0 to activate GPIO */
/*	DIO_BASE = (superio_inb(F81866D_IO_BASE_MSB) << 8) | superio_inb(F81866D_IO_BASE_LSB);  //get default dio base 
        DIO_BASE =DIO_BASE+gpio_id;  */ 
	return 0;
}

void f81866d_close()
{
	outb(LOCK_DATA, SIO_BASE);
}

/*

void f81866d_set_dio_base(int diobase)
{

	if(diobase!=0)
	{
//	  superio_outb((diobase >> 8), F81866D_IO_BASE_MSB); //MSB
//	  superio_outb((diobase & 0xFF), F81866D_IO_BASE_LSB); //LSB
	  DIO_BASE = diobase;
	}
}

*/
void f81866d_set_gpio_mode(int port, int mode)
{
	 int gpio_direc;


	 
	 gpio_direc = superio_inb(F81866D_GPIO5_MODE);       //GPIO 0-7


	 if(mode ==1)
	   gpio_direc = gpio_direc | (1<<(port-1));
	 else if(mode ==0)
	   gpio_direc = gpio_direc & ~(1<<(port-1));

	 superio_outb(gpio_direc, F81866D_GPIO5_MODE);
}


void f81866d_set_gpio_value(int port, int status)
{
	 //int gpio_direc;
	 int dio_val;

	 dio_val = superio_inb(F81866D_GPIO5_Output_Data);


	 if(status ==1)
	   dio_val = dio_val | (1<<(port-1));
	 else if(status ==0)
	   dio_val = dio_val & ~(1<<(port-1));
	 
	 
	 superio_outb(dio_val, F81866D_GPIO5_Output_Data);


}


int f81866d_display_gpio_mode(int port)
{
	 int gpio_direc;


	 
	 gpio_direc = superio_inb(F81866D_GPIO5_MODE);


	 if(((gpio_direc >> (port-1)) & 0x1)==0)
	   return 0;  //input
	 else 
	   return 1;  //output	

}

int f81866d_display_gpio_val(int port)
{
	 int mask;
	 int val,val1,val2,val3;


	 int dio_val;

         val = superio_inb(F81866D_GPIO5_MODE);	//0x88
	 val1 = superio_inb(F81866D_GPIO5_Data_Status);	//0x8A
	 val2 = superio_inb(F81866D_GPIO5_Output_Data);	//0x89
	 val3 =((val & 0xff) & (val2 & 0xff))|(~(val & 0xff) & (val1 & 0xff)); 
		 
	 mask = val3 & (0x1 << (port-1));
	 if( mask==0)
           return 0;  //low
	 else 
   	   return 1;  //high
}


