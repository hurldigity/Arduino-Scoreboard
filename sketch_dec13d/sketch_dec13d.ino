#include <Arduino.h>
#include <TM1637Display.h>
int num = 0;

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

TM1637Display display(CLK, DIO);

void setup()
{
}

void loop()
{
  display.setBrightness(0x0f);
  display.showNumberDec(num,true);
  num++;
  delay(1000);
}
