#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

int main() {
    configureClocks();
    enableWDTInterrupts();
 
    buzzer_init();
    // buzzer_set_period(1000);	/* start buzzing!!! 2MHz/1000 = 2kHz*/
    // playBuzzer();

    or_sr(0x18);          // CPU off, GIE on
}
int secondCount = 0;
               //A5    D5    A#4   G4    G5     D5    A#4   G4   F#5    D5    A#4   G4
int notes[64] = {2272, 3407, 4291, 5115, 2554, 3407, 4291, 5115, 2706, 3407, 4291, 5155,
		 2554, 3407, 4291, 5115, 2554, 3824, 4545, 5730, 2865, 3824, 4545, 5730,
		 3034, 3824, 4545, 5730, 2865, 3824, 4545, 5730, 2865, 4291, 5115, 6079,
		 3034, 4291, 5115, 6079, 3215, 4291, 5115, 6079, 3034, 4291, 5115, 6079,
		 3034, 4545, 5730, 6825, 3407, 4545, 5730, 6825, 3610, 4545, 5730, 6825,
		 3407, 4545, 5730, 6825};
int i = 0;

void 
__interrupt_vec(WDT_VECTOR) WDT(){
  secondCount++;
  if(secondCount >= 46){
    secondCount = 0;
    if(i == 64){
      i = 0;
    }
    buzzer_set_period(0);
    //buzzer_set_period(notes[i]);
    i++;
  }
}
