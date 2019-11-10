#include <OneWire.h>
#include "TM1637.h"
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
TM1637 tm1637(CLK, DIO);

int TEMP_INPUT_PIN = 3; // пин для датчика темперауры
float temperature = 50; // very warm
bool point_blink = false;

// Для работы датчика температуры
OneWire ds(TEMP_INPUT_PIN); // init temp sensor

// the setup function runs once when you press reset or power the board
void setup() {
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);  
  tm1637.point(true);  
  writeAll(_0, _0, _0, _0);
  
  Serial.begin(9600);
}

void setSegments(byte addr, byte data)
{
  tm1637.start();
  tm1637.writeByte(ADDR_FIXED);
  tm1637.stop();
  tm1637.start();
  tm1637.writeByte(addr|0xc0);
  if (point_blink) {
    tm1637.writeByte(data + 0x80);  
  } else {
    tm1637.writeByte(data);  
  }  
  tm1637.stop();
  tm1637.start();
  tm1637.writeByte(tm1637.Cmd_DispCtrl);
  tm1637.stop();
}

void writeAll(byte first, byte second, byte third, byte fourth) {
  setSegments(0, first);
  setSegments(1, second);
  setSegments(2, third);
  setSegments(3, fourth);  
}

// the loop function runs over and over again forever
void loop() {
  byte data[2]; // Место для значения температуры   
  ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
  ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память

  delay(1000);                       // wait for a second      

  ds.reset(); // Теперь готовимся получить значение измеренной температуры
  ds.write(0xCC); 
  ds.write(0xBE); // Просим передать нам значение регистров со значением температуры
 
  // Получаем и считываем ответ
  data[0] = ds.read(); // Читаем младший байт значения температуры
  data[1] = ds.read(); // А теперь старший
 
  // Формируем итоговое значение: 
  //    - сперва "склеиваем" значение, 
  //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
  temperature =  ((data[1] << 8) | data[0]) * 0.0625;   
  Serial.println(temperature);
  point_blink = !point_blink;
  writeMinute(55); 
  writeSecond(temperature + 0.5); 
  delay(1000);                       // wait for a second
}
