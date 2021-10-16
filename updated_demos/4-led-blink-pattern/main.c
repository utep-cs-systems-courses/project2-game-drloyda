//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state vars that control blinking
//int blinkLimit = 5;  // duty cycle = 1/blinkLimit
//int blinkCount = 0;  // cycles 0...blinkLimit-1
//int secondCount = 0; // state var representing repeating time 0…1s
//int stateVariable = 250;

int blink_count = 0;
int main_state = 0;
int int_state = 1;
int fast_state = 0;
void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle blinking 
  // blinkCount ++;
  //if (blinkCount >= blinkLimit) { // on for 1 interrupt period
  // blinkCount = 0;
  // P1OUT |= LED_GREEN;
  // } else		          // off for blinkLimit - 1 interrupt periods
  // P1OUT &= ~LED_GREEN;

  // measure a second
  // secondCount ++;
  
  //if (secondCount >= stateVariable) {  // once each second
  // secondCount = 0;
  // blinkLimit ++;	     // reduce duty cycle
  // if (blinkLimit >= 8)     // but don't let duty cycle go below 1/7.
    //  blinkLimit = 0;
  // }
  blink_count++;
  if(blink_count % 250 == 0){
    main_state ^= 1; //alternating bewtween 1 and 0
  }

  if(blink_count >= 500){
    int_state = (int_state % 4) + 1;
    blink_count = 0;
  }
  
} 
void dim25(int state){
  switch(state){
  case 0:
  case 1:
  case 2:
    P1OUT &= ~LED_GREEN; //turn off led_green
    break;
  case 3:
    P1OUT |= LED_GREEN; //turn on led_green
    break;
  default:
    break;
  }
}

