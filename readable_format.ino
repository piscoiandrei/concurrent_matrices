#include "LedControl.h"

LedControl lc = LedControl(12,11,10,2);

// animations are stored in the original file

void setup() {
  // Serial.begin(115200);
   lc.shutdown(0,false);
   lc.shutdown(1,false);
   lc.setIntensity(0,5);
   lc.setIntensity(1,5);
   lc.clearDisplay(0);
   lc.clearDisplay(1);

   //activating the normal mode for the timer2
   TCCR2A &= ~(1<<WGM21);
   TCCR2A &= ~(1<<WGM20);
   TCCR2B &= ~(1<<WGM22);

   TCCR2B |= (B00000111);// ps = 1024
   TIMSK2 |= (1<<TOIE2); // removing the mask 
}

volatile bool ok = true;
volatile uint8_t flag0 = 0, flag1 = 0;
uint8_t frame0,frame1; 

const int FRAMES0 = sizeof(C)/8;
const int FRAMES1 = sizeof(A)/8;

#define CASE_0 20   // *16.32ms
#define CASE_1 5

ISR (TIMER2_OVF_vect)
{  
  ++flag0;
  ++flag1;
  ok = true;
}
void loop() {
  if(frame0 == FRAMES0)
     frame0 = 0;
     
  if(frame1 == FRAMES1)
     frame1 = 0; 
     
  if(ok){
    if(flag0%CASE_0 == 0){
      nextframe0(C);
      flag0 = 0;
    }
    if(flag1%CASE_1 == 0){
      nextframe1(A);
      flag1 = 0;
    }
    ok = false;
  }
}
void nextframe0(byte animation[][8]){
  for(int i = 0; i < 8; i++)
      lc.setRow(0,i,animation[frame0][i]);
  ++frame0;
}
void nextframe1(byte animation[][8]){
  for(int i = 0; i < 8; i++){
    lc.setRow(1,i,animation[frame1][i]);
  } 
  ++frame1;  
}