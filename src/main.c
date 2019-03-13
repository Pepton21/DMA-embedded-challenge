/*
===============================================================================
 Name        : main.c
 Author      : Bojan Gruevski
 Version     : 1.2
 Copyright   : SEAVUS Group
 Description : main definition
===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "md5.h"
#include "iap_driver.h"
#include "payload_generator.h"
#include "leds.h"
#include "definitions.h"

/**
* delay of approximately 1 second
*/
void delay(){
    uint64_t delay = 3200000ULL;
    while(delay)
        delay--;
}

/**
* main function
*/
int main(void) {
    e_iap_status iap_status;
    int flag;
    double elapsed;

    iap_status = (e_iap_status) generator_init();
    if (iap_status != CMD_SUCCESS) {
        while(1);   // Error !!!
    }

    /* set up a timer */  /*
	LPC_SC->PCONP |= 1 << 2; // Power up Timer 1
	LPC_SC->PCLKSEL0 |= 0x01 << 4; // CCLK
	LPC_TIM1->PR = 0xffffffff;
	LPC_TIM1->TCR |= 1 << 1; // Manually reset Timer 1 (forced)
	LPC_TIM1->TCR &= ~(1 << 1); // Stop resetting the timer
	LPC_TIM1->MCR = 0x00;
	LPC_TIM1->TCR |= 1 << 0; // Start T1
	uint64_t start = LPC_TIM1->TC * 0xffffffff + LPC_TIM1->PC; // Start  */

	/* set testing pin to 1 */
	LPC_GPIO2->FIODIR = (1 << 13);
	LPC_GPIO2->FIOSET = (1 << 13);

	/* verify the archive */
    flag = verify();

    /* set testing pin to 0 */
    LPC_GPIO2->FIOCLR = (1 << 13);

    /* measure time until here */
    //uint64_t end = LPC_TIM1->TC * 0xffffffff + LPC_TIM1->PC; // End

    /* initialize led and turn it off by default */
    led2_init();
    led2_off();

    /* if the archive is hashed correctly, turn let on, otherwise make it blink */
    if (flag)
    	led2_on();
    else
    	while(1){
    		led2_invert();
    		delay();
    	}

    return 0 ;
}
