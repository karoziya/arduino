#include "TM1637.h"

#define CLK 9
#define DIO 8
TM1637 tm1637(CLK, DIO);

#define ENC_SW 12
#define ENC_DT 3
#define ENC_CLK 10
#define TEN_SEC_SEGMENT 2
#define SEC_SEGMENT 3

int8_t DispMSG[] = {0, 0, 0, 0};
bool countdown = false;
bool timemode = false;
bool selectmode = false;
bool timepoint = true;
unsigned long timing;
unsigned long currentTiming;

int minValue = 0;
int secValue = 0;
unsigned char encoder_A, encoder_B, encoder_A_prev;
static bool SW_State = false;


void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH);  
  
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);  
  tm1637.point(false);  
  writeAll(_0, _0, _0, _0);

  pinMode(ENC_DT, INPUT);
  pinMode(ENC_CLK, INPUT);
  pinMode(ENC_SW, INPUT);
  encoder_A_prev =  digitalRead(ENC_CLK);
  // hello
  setSegments(0, 118);
  setSegments(1, 121);
  setSegments(2, 54);
  setSegments(3, 63);
}

bool delaySeconds() {
  currentTiming = millis();
  if (currentTiming - timing > 1000) {
    timing = currentTiming;
    return true;
  }  
  return false;
}

void countdownHandler() {
  if (secValue == 0 && minValue == 0){
    writeAll(_S, _t, _O, _P);      
    countdown = false;
    selectmode = false;
    timing = millis();
  } else {
    if (delaySeconds()) {
      writeMinute(minValue);
      writeSecond(secValue);
      if (secValue == 0) {
        minValue = minValue - 1;
        secValue = 59;
      } else {
        secValue = secValue - 1;
      }              
    }
  }        
}


void loop() {  
 writeAll(_S, _t, _O, _P); 
}
