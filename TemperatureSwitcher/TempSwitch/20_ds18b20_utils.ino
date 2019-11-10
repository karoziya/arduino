void detectTemperature() {
  if (!isDetected)
  {    
    ds.reset();
    ds.write(0xCC);
    ds.write(0x44);
    isDetected = true;
  }
}

float getTemperature()
{
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);
  data[0] = ds.read();
  data[1] = ds.read();
  
  isDetected = false;
  return ((data[1] << 8) | data[0]) * 0.0625;

}
