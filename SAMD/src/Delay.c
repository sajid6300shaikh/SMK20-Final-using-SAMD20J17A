
#include "compiler.h"

#include "Delay.h"


 void delay_1us() {			//gives 1 us delay
	 for (volatile int iter=0; iter<2;iter++)
	 {
		 asm("NOP");		 
		 asm("NOP");		 
	 }
 }

void delay_us(volatile uint32_t iter_us){	//gives us delay
	while(iter_us!=0){
		delay_1us();
		iter_us--;
	}
}

void delay_1ms() {
		delay_us(1050);	//value adjusted to give 1ms delay

}

void delay_ms(volatile uint32_t iter_ms){
	while (iter_ms)
	{
		delay_1ms();
		iter_ms--;
	}
}

void delay_sec(volatile uint16_t iter_s){
	while(iter_s!=0){
		delay_ms(1000);
		iter_s--;
	}
	
}