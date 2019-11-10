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
    writeValue(value, 0, 1, false);
}

void writeSecond(int value) {
      writeValue(value, TEN_SEC_SEGMENT, SEC_SEGMENT, false);
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
