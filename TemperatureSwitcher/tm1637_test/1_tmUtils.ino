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

void setSegments(byte addr, byte data)
{
  tm1637.start();
  tm1637.writeByte(ADDR_FIXED);
  tm1637.stop();
  tm1637.start();
  tm1637.writeByte(addr|0xc0);
  if (minValue > 0) {
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

byte getByteByDigit(int digit) {
  byte result;
  switch (digit) {
    case 0:
      result = _0;
      break;
    case 1:
      result = _1;
      break;
    case 2:
      result = _2;
      break;
    case 3:
      result = _3;
      break;
    case 4:
      result = _4;
      break;
    case 5:
      result = _5;
      break;
    case 6:
      result = _6;
      break;
    case 7:
      result = _7;
      break;
    case 8:
      result = _8;
      break;
    case 9:
      result = _9;
      break;
  }
  return result;
}

void writeValue(int value, byte seg1, byte seg2, bool leadZero) {
  if (value >= 10) {
    setSegments(seg1, getByteByDigit(value/10));    
    setSegments(seg2, getByteByDigit(value%10));    
  } else if (value > 0) {
    // костыль для секунд.
    if (leadZero) {
      setSegments(seg1, _0);
    } else {
      setSegments(seg1, _empty);
    }    
    setSegments(seg2, getByteByDigit(value));
  }
}

void writeMinute(int value) {
  if (value == 0){
    setSegments(0, _empty);
    setSegments(1, _empty);
  } else {
    writeValue(value, 0, 1, false);
  }
}

void writeSecond(int value) {
  if (value == 0 && minValue > 0){
    setSegments(TEN_SEC_SEGMENT, _0);
    setSegments(SEC_SEGMENT, _0);
  } else if (minValue == 0){  
      writeValue(value, TEN_SEC_SEGMENT, SEC_SEGMENT, false);
    } else {
      writeValue(value, TEN_SEC_SEGMENT, SEC_SEGMENT, true);
    }
}
