#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "led.h"

//#define LED_RED BIT0               // P1.0
//#define LED_GREEN BIT6             // P1.6
#define LEDS (BIT0 | BIT6)
#define SW1 BIT0               
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)

   
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
int sw3Down = 0;
int sw4Down = 0;

void switch_interrupt_handler()
{
  char p2val = P2IN;/* switch is in P2 */

  /* update switch interrupt sense to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */

  /* up=no sequence, down=sequence */
  buzzer_init();
  if((p2val & SWITCHES) == 14){ //toggle sw1 on or off
    turnOff();
    sw1Down ^= 1;
  }else if((p2val & SWITCHES) == 13){ //toggle sw2 on or off
    turnOff();
    sw2Down ^= 1;
  }else if((p2val & SWITCHES) == 11){
    turnOff();
    sw3Down ^= 1;
  }else if((p2val & SWITCHES) == 7){
    turnOff();
    sw4Down ^= 1;
  }else {
    buzzer_set_period(0); //if no switches are active
  }
}
void turnOff(){
  sw1Down &= ~sw1Down;
  sw2Down &= ~sw2Down;
  sw3Down &= ~sw3Down;
  sw4Down &= ~sw4Down;
}
/* Switch on P2 (S2) */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }
}

//global variables of songs and timing
int secondCount = 0;
int i = 0;
int j = 0;
int k = -1;
int ld_r = LED_RED;
int ld_g = LED_GREEN;
int notes[64] = {2272, 3407, 4291, 5115, 2554, 3407, 4291, 5115, 2706, 3407, 4291, 5155,
		 2554, 3407, 4291, 5115, 2554, 3824, 4545, 5730, 2865, 3824, 4545, 5730,
		 3034, 3824, 4545, 5730, 2865, 3824, 4545, 5730, 2865, 4291, 5115, 6079,
		 3034, 4291, 5115, 6079, 3215, 4291, 5115, 6079, 3034, 4291, 5115, 6079,
		 3034, 4545, 5730, 6825, 3407, 4545, 5730, 6825, 3610, 4545, 5730, 6825,
		 3407, 4545, 5730, 6825};

int notes2[26] = {5115, 0, 5115, 0, 5115, 0, 5115, 0, 5115, 4819, 5730, 0, 5730, 0, 5730,
		  0, 5730, 0, 5730, 0 ,5730, 0 , 5730, 5115, 4291, 5115};
int notes3[37] = {1517, 1607, 1517, 1607, 1517, 2025, 1703, 1910, 2273, 3822, 3034, 2273,
		  2025, 3034, 2408, 2025, 1910, 3034, 1517, 1607, 1517, 1607, 1517, 1607,
		  2025, 1703, 1910, 2273, 3822, 3034, 2273, 2025, 3034, 1910, 2025, 2273};
int time[26] = {62, 31, 62, 31, 62, 31, 62, 31, 62, 31, 31, 62, 31, 125, 31, 62, 31, 62,
		31, 31, 31, 62, 31, 31, 31, 125};
int time2[37] = {0,46, 46, 46, 46, 46, 46, 46, 46, 92, 46, 46, 46, 92, 46, 46, 46, 92, 46,
		 46, 46, 46, 46, 46, 46, 46, 46 ,46 ,92, 46, 46, 46, 92, 46, 46, 46, 92};
void
__interrupt_vec(WDT_VECTOR) WDT(){
  secondCount++;
  if(sw1Down == 1){ //if sw1 pressed
    playSongOne();
  }else if(sw2Down == 1 ){ //if sw2 pressed
    playSongTwo();
  }else if(sw3Down == 1){  
    playSongThree();
  }else if(sw4Down ==1){
    buzzer_set_period(0);
  }else{
    buzzer_set_period(0);
  }
}

//plays the buzzer based on notes in notes array
//at every second
void playSongOne(){
  if(secondCount >= 46){
    secondCount = 0;
    if(i >= 64){
      i = 0;
    }
    P1OUT ^= LED_GREEN;
    P1OUT ^= LED_RED;
    buzzer_set_period(notes[i]);
    i++;
  }
}

//plays the buzzer based on notes in second notes array
//based on the timing for each note
void playSongTwo(){
  if(secondCount >= time[j]){
    secondCount = 0;
    if(j >= 26){
      j = 0;
    }
    greenBeat();
    buzzer_set_period(notes2[j]);
    j++;
  }
}

void playSongThree(){
  if(secondCount >= time2[k]){
    secondCount = 0;
    if(k >= 37){
      k = -1;
    }
    greenBeat();
    buzzer_set_period(notes3[k]);
    k++;
  }
}
int blink_count = 0;
int main_state = 0;
int int_state = 1;
int fast_state = 0;

void dimLights(){
  blink_count++;
  if(blink_count % 250 == 0){
    main_state ^= 1; //alter ating bewtween 1 and 0
  }

  if(blink_count >= 500){
    int_state = (int_state % 4) + 1;
    blink_count = 0;
  }
  if(main_state == 0){
    P1OUT &= ~LED_GREEN;
    return;
  }
  fast_state = (fast_state +1) % 4;
  switch(int_state){
  case 1: dim25(fast_state); break;
  case 2: dim50(fast_state); break;
  case 3: dim75(fast_state); break;
  case 4: dim100(fast_state); break;
  default: break;
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

void dim50(int state){
  switch(state){
  case 0:
  case 1:
    P1OUT &= ~LED_GREEN;
    break;
  case 2:
  case 3:
    P1OUT |= LED_GREEN;
  default:
    break;
  }
}
void dim75(int state){
  switch(state){
  case 0:
    P1OUT &= ~LED_GREEN;
    break;
  case 1:
  case 2:
  case 3:
    P1OUT |= LED_GREEN; //turn on led_green
    break;
  default:
    break;
  }
}

void dim100(int state){
  P1OUT |= LED_GREEN;
}
