//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller-Dual Brush Motor Card (IFC-BH02)
//	Project description	: Sample1 for IFC-BH02
//  Date				: 30 December 2008
//  Edited				: 28 February 2013
//
//=====================================================================================

//=====================================================================================
//	include h file for cards
//	User has to include h file for all the cards used 
//=====================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_bh.h"


//=====================================================================================
//	configuration bit, User is advised not to change the setting here
//=====================================================================================
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//=====================================================================================
//	define input or ouput of board here
//	For example: 
//				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//=====================================================================================

#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00

#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00
#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards

//=====================================================================================
//	card address
//	User can define the address of slave card here
//=====================================================================================
#define add_bh1 		0b000010	// Please make sure the communication address on 
									// IFC-BH02 is compatible with the address 

//=====================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//=====================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);
	
//=====================================================================================
//	Main Function
//	This is the main function where program start to execute 
//=====================================================================================
void main(void)
{
	//--------------------------------------------------------------------------------
	//	Program start
	//	User can start to write/modify the program here
	//--------------------------------------------------------------------------------

	unsigned short i,temp1;				// define 16-bits local variable
	unsigned char flag1,flag2,dir1,dir2,speed1,speed2;	// define 8-bits local variable

	//	User is advised not to change or remove the initialization function called 
	init();						// call initialization function of the setting in program

	while(1)
	{

	
		//--------------------------------------------------------------------------------
		// This program is mainly for slave card,BH02. It will test the 3 push buttons
		// on MB00, the push buttons are active low, hence, when any of the push button 
		// is pressed, value '0' will be sent to microcontroller on master card, MB00.
		// Hence, the brush motor will run base on program
		//--------------------------------------------------------------------------------

	
		if(sw1==0)							// Test whether SW1 on IFC-MB00 is pressed
		{
			led1 = 1;						// turn ON LED1 on MB00 by sending value '1'

			//buzzer beep once
			buzzer = 1;						// turn ON buzzer on MB00 by sending value '1'
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// turn OFF buzzer on MB00 by sending value '0'

			while(1)
			{
				// this is an infinity loop after SW1 on IFC-MB00 is pressed, 
				// user need to press reset button on MB00 to exit from the mode		

				bh_1_cw(add_bh1);			// Run motor 1 in clockwise
				bh_1_speed(add_bh1,250);	// motor 1 speed = 250
				bh_2_brake(add_bh1);		// brake motor 2
				delay(40000);                   // BH02 need 40ms for relay direction changing
								// so some delay is needed to get the correct runstat value 
				led4=bh_1_runstat(add_bh1);	// turn ON led4 when motor 1 run
				led5=bh_2_runstat(add_bh1);	// turn OFF led5 when motor 2 brake
				for(i=0;i<10;i++)			// delay for motor run/brake time
				{
					delay(50000);
				}
				bh_1_stop(add_bh1);			// stop motor 1
				bh_2_cw(add_bh1);			// Run motor 2 in clockwise
				bh_2_speed(add_bh1,250);	// motor 2 speed = 250
				delay(40000);                   // BH02 need 40ms for relay direction changing
								// so some delay is needed to get the correct runstat value 
				led4=bh_1_runstat(add_bh1);	// turn OFF led4 when motor 1 stop
				led5=bh_2_runstat(add_bh1);	// turn ON led5 when motor 2 run
				for(i=0;i<10;i++)			// delay for motor stop/run time
				{
					delay(50000);
				}
				bh_1_ccw(add_bh1);			// Run motor 1 in counter-clockwise
				bh_1_speed(add_bh1,150);	// motor 1 speed = 150
				bh_2_stop(add_bh1);			// stop motor 2
				delay(40000);                   // BH02 need 40ms for relay direction changing
								// so some delay is needed to get the correct runstat value 
				led4=bh_1_runstat(add_bh1);	// turn ON led4 when motor 1 run
				led5=bh_2_runstat(add_bh1);	// turn OFF led5 when motor 2 stop
				for(i=0;i<10;i++)			// delay for motor run/stop time
				{
					delay(50000);
				}
				bh_1_brake(add_bh1);		// brake motor 1
				bh_2_ccw(add_bh1);			// Run motor 2 in counter-clockwise
				bh_2_speed(add_bh1,150);	// motor 2 speed = 150
				delay(40000);                   // BH02 need 40ms for relay direction changing
								// so some delay is needed to get the correct runstat value 
				led4=bh_1_runstat(add_bh1);	// turn OFF led4 when motor 1 brake
				led5=bh_2_runstat(add_bh1);	// turn ON led5 when motor 2 run
				for(i=0;i<10;i++)			// delay for motor brake/run time
				{
					delay(50000);
				}
			}
		}

		else if(sw2==0)							// Test whether SW2 on IFC-MB00 is pressed
		{
			led2 = 1;						// turn ON LED2 on MB00 by sending value '1'
			for(i=0;i<(2*2);i++)			// buzzer beeps 2 time 
			{
				buzzer ^= 1;				// toggle buzzer
				delay(50000);				// time delay for the toggled buzzer
			}

			// Motor 1 will change the running direction to 
			// counter-clockwise and speed to 100 when encoder 1 value = 1000.
			bh_1_cw(add_bh1);					// Run motor 1 in clockwise
			bh_1_speed(add_bh1,250);			// set motor 1 speed = 250
			bh_1_encon(add_bh1, 1000, 4, 100);	// change motor 1 direction to ccw with 
												// speed 100 when encoder 1 value = 1000.

			// Motor 2 will stop when encoder 2 value = 500.
			bh_2_cw(add_bh1);					// Run motor 2 in clockwise
			bh_2_speed(add_bh1,180);			// set motor 2 speed = 180
			bh_2_encon(add_bh1, 500, 1, 0);		// motor 2 stop when encoder 2 value = 500.

			while(1)
			{
				// this is an infinity loop after SW2 on IFC-MB00 is pressed, 
				// user need to press reset button on MB00 to exit from this mode.
				// this loop tests the status of both encoders by using led6 and led7 as indicator

				led6=bh_1_enstat(add_bh1);		// turn ON led6 when encoder 1 in process
				led7=bh_2_enstat(add_bh1);		// turn ON led7 when encoder 2 in process
				delay(10000);					// time delay for every status testing				
			}
		}

		else if(sw3==0)			   				// Test whether SW3 on IFC-MB00 is pressed
		{
			temp1=0;						// clear temp1
			speed1=0;						// clear speed
			flag1=0;						// set flag 1 to 0 first
			dir1=0;							// set dir 1 to 0 first
			speed2=128;						// set speed 2 to 128 first
			flag2=0;						// set flag 2 to 0 first
			dir2=0;							// set dir 2 to 0 first
			led3 = 1;						// turn ON LED3 on MB00 by sending value '1'
			for(i=0;i<(3*2);i++)			// buzzer beeps 3 time
			{
				buzzer ^= 1;				// toggle buzzer
				delay(50000);				// time delay for the toggled buzzer
			}
			bh_2_ccw(add_bh1);				// set motor 2 to counter clockwise

			while(1)
			{
				// This is an infinity loop after SW3 on IFC-MB00 is pressed. 
				// The two motors will running independently.
				// The speeds will increase and decrease gradually and then direction will be changed.
				// This program will running repeatedly until the reset button is pressed.

				temp1+=1;						// increase temp1
				if(temp1>10000)					// the value of temp1 here will determine the 
				{								// acceleration of the motors, the bigger the value here, 
												// the slower the speed of the increment or decrement
					temp1=0;					// clear temp1	
					
					if(speed1>=255)flag1=1;		// set flag 1 to 1 when maximum speed 1 is reached.
					else if(speed1<=0)			
					{
						flag1=0;						// set flag 1 to 0 when minimum speed 1 is reached.
						dir1^=1;						// change direction of motor 1 for 
						if(dir1==0)bh_1_ccw(add_bh1);	// every speed 1 is equal to 0.
						else bh_1_cw(add_bh1);
					}
					if(flag1==0)speed1++;		// increase speed 1 by one if flag 1 is 0.
					else speed1--;				// decrease speed 1 by one if flag 1 is 1.
					bh_1_speed(add_bh1,speed1);	// set motor 1 speed

					
					if(speed2>=255)flag2=1;		// set flag 2 to 1 when maximum speed 2 is reached.
					else if(speed2<=0)
					{
						flag2=0;						// set flag 2 to 0 when minimum speed 2 is reached.
						dir2^=1;						// change direction of motor 2 for 
						if(dir2==0)bh_2_ccw(add_bh1);	// every speed 2 is equal to 0.
						else bh_2_cw(add_bh1);
					}
					if(flag2==0)speed2++;		// increase speed 2 by one if flag 2 is 0.
					else speed2--;				// decrease speed 2 by one if flag 2 is 1.
					bh_2_speed(add_bh1,speed2);	// set motor 2 speed
				}		
			}
		}
	}
}

//=====================================================================================
//	functions
//  User can write all the necessary function here
//=====================================================================================
void delay(unsigned long data)
{
	// this is a delay function for user to use when the program need a delay
	// this function can be call by type : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}


void init(void)
{
	// this is a initialization for set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;			// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;			// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'
}


//=====================================================================================
//	Interrupt vector
//=====================================================================================
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow			// jump to interrupt routine
	_endasm
}
#pragma code

//=====================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//=====================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

