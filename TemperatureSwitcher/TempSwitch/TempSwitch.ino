#include <Bounce2.h>
#include <OneWire.h>
#include <TM1637.h>
/************** БУКВЫ И СИМВОЛЫ *****************/
#define _A 0x77
#define _B 0x7f
#define _C 0x39
#define _D 0x3f
#define _E 0x79
#define _F 0x71
#define _G 0x3d
#define _H 0x76
#define _J 0x1e
#define _L 0x38
#define _N 0x37
#define _O 0x3f
#define _P 0x73
#define _S 0x6d
#define _U 0x3e
#define _Y 0x6e
#define _a 0x5f
#define _b 0x7c
#define _c 0x58
#define _d 0x5e
#define _e 0x7b
#define _f 0x71
#define _h 0x74
#define _i 0x10
#define _j 0x0e
#define _l 0x06
#define _n 0x54
#define _o 0x5c
#define _q 0x67
#define _r 0x50
#define _t 0x78
#define _u 0x1c
#define _y 0x6e
#define _minus 0x40
#define __ 0x08
#define _equally 0x48
#define _empty 0x00

#define _0 0x3f
#define _1 0x06
#define _2 0x5b
#define _3 0x4f
#define _4 0x66
#define _5 0x6d
#define _6 0x7d
#define _7 0x07
#define _8 0x7f
#define _9 0x6f
/************** БУКВЫ И СИМВОЛЫ *****************/

// Для работы индикатора
#define CLK 9
#define DIO 8
#define TEN_SEC_SEGMENT 2
#define SEC_SEGMENT 3
#define PIN_RELAY 10
TM1637 tm1637(CLK, DIO);
bool point_blink = false;
// Для работы кнопки.
const int PIN_BUTTON_PLUS = 5;
const int PIN_BUTTON_MINUS = 6;
int pressValue = LOW;
int targetValue = 30;
bool btnChecked = false;
// Для работы датчика температуры
int TEMP_INPUT_PIN = 3; // пин для датчика темперауры
OneWire ds(TEMP_INPUT_PIN); // init temp sensor
long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 1000; // Определяем периодичность проверок
float temperature = 0; // very warm
bool isDetected = false;
byte data[2];

// the setup function runs once when you press reset or power the board
void setup() {
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);
  tm1637.point(true);
  writeAll(_0, _0, _0, _0);

  Serial.begin(9600);

  pinMode(PIN_BUTTON_PLUS, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MINUS, INPUT_PULLUP);
  pinMode(PIN_RELAY, OUTPUT); // Объявляем пин реле как выход
  digitalWrite(PIN_RELAY, LOW); // Выключаем реле - посылаем высокий сигнал
}

// the loop function runs over and over again forever
void loop() {
  detectTemperature();  
  int btnValPlus = digitalRead(PIN_BUTTON_PLUS);
  int btnValMinus = digitalRead(PIN_BUTTON_MINUS);
  if ((btnValPlus == pressValue || btnValMinus == pressValue) && !btnChecked) {
    //Serial.println("pressed");
    if (btnValPlus == pressValue){
      targetValue = targetValue + 1;
    }
    if (btnValMinus == pressValue){
      targetValue = targetValue - 1;
    }    
    btnChecked = true;
    delay(400);
    writeMinute(targetValue);
  } else {
    if (btnChecked) {
      //Serial.println("unpressed");
    }
    btnChecked = false;
  }

  // Serial.println(sensorVal);
  if (millis() - lastUpdateTime > TEMP_UPDATE_TIME)
  {
    point_blink = !point_blink;
    temperature = getTemperature();
    lastUpdateTime = millis();
    writeMinute(targetValue);
    int currTemp = temperature + 0.5;
    writeSecond(currTemp);
    if (targetValue > currTemp)
    {
      digitalWrite(PIN_RELAY, HIGH);
      //Serial.println("ON");
    } else {
      digitalWrite(PIN_RELAY, LOW);
      //Serial.println("OFF");
    }
  }
}
