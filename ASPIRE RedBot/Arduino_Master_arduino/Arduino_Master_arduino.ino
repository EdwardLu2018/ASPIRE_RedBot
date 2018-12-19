#include <Wire.h>    //declare I2C library function
#define LED A3

#include <SPI.h>
#include <Pixy.h>
Pixy pixy;

byte x = 0;    //variable x determine whether led on master is on or off
//initializing

void setup()
{
  Wire.begin();    // join the I2C bus as master
  pinMode(LED, OUTPUT);   // initialize the digital pin 13 as output
  pixy.init();
}

//main program
void loop()
{
  static int i = 0;
  Wire.beginTransmission(4);    //begin a transmission to slave device 4
  Wire.write(x);    // write one byte data to slave
  Wire.endTransmission();  // end a transmission to slave
  uint16_t blocks;
  blocks = pixy.getBlocks(); //uses pixy to look for blocks

  if (pixy.blocks[i].signature == 2)  //sees signature 2 (in my case: blue) = sends a 0 to slave and blinks LED
  {
    x = 0;
  }
  else if (pixy.blocks[i].signature == 1)  //sees signature 1 (in my case: red) = sends a 1 to slave to turn on LED 
  {
    x = 1;
  }

  Wire.requestFrom(4, 1);    //request one byte from slave 4

  while (Wire.available() > 0) // when data is received from slave to master
  {
    byte c = Wire.read();    //receive one byte from slave and assign to variable c

    if (c == 1) //if red card is seen, send a 1 to slave
    {
      digitalWrite(LED, HIGH);
    }
    else if (c == 0) //if blue card is seen, send a 0 to slave which blinks LED
    {
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
    }
  }
}
