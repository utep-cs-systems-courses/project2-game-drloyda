#ifndef led_included
#define led_included

#define LED_RED BIT6               // P1.0
#define LED_GREEN BIT0             // P1.6
#define LEDS (BIT0 | BIT6)

void dimLights();
void dim25(int state);
void dim50(int state);
void dim75(int state);
void dim100(int state);
void turnOff();
void greenBeat();
#endif // included
