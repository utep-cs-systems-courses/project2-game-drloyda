#include <msp430.h>
#include "led.h"

extern int secondCount;

void greenBeat(){
  if(secondCount <= 125){
    P1OUT ^= LED_GREEN;
  }
  if(secondCount <= 62){
    P1OUT ^= LED_RED;
  }
}
