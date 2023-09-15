#include "REG52.H"

#define BLINK_INTERVAL_IN_MILLISECOND 200

typedef enum{
	ON,
	OFF
}LED_STATE;

unsigned int TimeOutEvent;

sbit LED_pin=P1^0;
LED_STATE Gu8LedPinState = OFF;

void configTimer0(void);
void configTimer0(void){
	TMOD = 0x01;	//16-Bit Timer mode
	TH0 = 0xfc;
	TL0 = 0x17;
	EA = 1;			//Enable All Interrupt
	ET0 = 1;		//Enable Timer0 Interrupt
	TR0 = 1;		//Start the Timer0
}

void main(){
	configTimer0();
	while(1){
	}
}

void blink() interrupt 1{
	static unsigned int InterruptCount = 0;
	InterruptCount < BLINK_INTERVAL_IN_MILLISECOND ? (InterruptCount++): (InterruptCount = 0,TimeOutEvent = 1); // Time out Event, this event comes from ISR
	/*Use the FSM state machine framework. with two LED state:"OFF" state and "ON" state.*/
	switch(Gu8LedPinState){
		case OFF:{
					if(TimeOutEvent){	//Resoponse to the time Event
						LED_pin = 0;	//The Event action, it can add more actins at here.	

						TimeOutEvent = 0;	//Time out Event reset
					}
					Gu8LedPinState = ON;	//The state tansimision
					break;
				} 

		case ON:{
					if(TimeOutEvent){
						LED_pin = 1;

						TimeOutEvent = 0;
					}
					Gu8LedPinState = OFF;
					break;
				} 
	}

	TH0 = 0xfc;	//reload the timer
	TL0 = 0x17;
}
