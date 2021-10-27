
#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (BIT0 | BIT6)
#define SW1 BIT0                /* switch1 is p1.3 */
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)   /* only 1 switch on this board */



int main() {

  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;
  
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;
  
  configureClocks();
  enableWDTInterrupts();
  
  or_sr(0x18);          // CPU off
}
int sw1Down = 0;
int sw2Down = 0;
int sw3Down;
int sw4Down;
void
switch_interrupt_handler()

{

  char p2val = P2IN;/* switch is in P2 */

  /* update switch interrupt sense to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */

  /* up=no sequence, down=sequence */
  //toggle sw1 on or off
  if((p2val & SWITCHES) == 14){
    sw1Down ^= 1;
    buzzer_init();
  //toggle sw2 on or off
  }else if((p2val & SWITCHES) == 13){
    sw2Down ^= 1;
  }
  else {
    buzzer_set_period(0);
  }
}


/* Switch on P2 (S2) */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }
}

int secondCount = 0;
int notes[64] = {2272, 3407, 4291, 5115, 2554, 3407, 4291, 5115, 2706, 3407, 4291, 5155,
		 2554, 3407, 4291, 5115, 2554, 3824, 4545, 5730, 2865, 3824, 4545, 5730,
		 3034, 3824, 4545, 5730, 2865, 3824, 4545, 5730, 2865, 4291, 5115, 6079,
		 3034, 4291, 5115, 6079, 3215, 4291, 5115, 6079, 3034, 4291, 5115, 6079,
		 3034, 4545, 5730, 6825, 3407, 4545, 5730, 6825, 3610, 4545, 5730, 6825,
		 3407, 4545, 5730, 6825};

int notes2[26] = {5115, 0, 5115, 0, 5115, 0, 5115, 0, 5115, 4819, 5730, 0, 5730, 0, 5730,
		  0, 5730, 0, 5730, 0 ,5730, 0 , 5730, 5115, 4291, 5115};
int time[26] = {62, 31, 62, 31, 62, 31, 62, 31, 62, 31, 31, 62, 31, 125, 31, 62, 31, 62,
		31, 31, 31, 62, 31, 31, 31, 125};
int i = 0;
void
__interrupt_vec(WDT_VECTOR) WDT(){
  secondCount++;
  
  if(sw1Down == 1){ //if sw1 pressed
    playSongOne();
   
  }else if(sw2Down == 1 ){ //if sw2 pressed
    playSongTwo();
  }else{
    buzzer_set_period(0);
  }
}
void playSongOne(){
  if(secondCount >= 46){
    secondCount = 0;
    if(i == 64){
      i = 0;
    }
    P1OUT ^= LED_GREEN;
    P1OUT ^= LED_RED;
    buzzer_set_period(notes[i]);
    i++;
  }
}
void playSongTwo(){
  if(secondCount >= time[i]){
    secondCount = 0;
    if(i >= 26){
      i = 0;
    }
    buzzer_set_period(notes2[i]);
    i++;
  }
}
