#include "buzzer.h"
extern int secondCount;
extern int j;
extern int time[];
extern int notes2[];
void playSongTwo(){
  if(secondCount >= time[j]){
    secondCount = 0;
    if(j >= 26){
      j = 0;
    }
    buzzer_set_period(notes2[j]);
    j++;
  }
}
