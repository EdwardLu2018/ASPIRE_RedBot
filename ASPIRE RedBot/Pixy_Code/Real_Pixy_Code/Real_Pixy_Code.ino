#include <Pixy.h>
#include <SPI.h>

// This is the main Pixy object
Pixy pixy;

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
  pixy.init();
  
  pinMode(A7, HIGH);
  pinMode(A6, HIGH);
}

void loop()
{
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];

  // grab blocks!
  blocks = pixy.getBlocks();

  // If there are detect blocks, print them!
  if (blocks)
  {
    i++;

    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i % 50 == 0)
    {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (j = 0; j < blocks; j++)
      {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf);
        pixy.blocks[j].print();
      }

      if (pixy.blocks[i].signature = 1) {
        Serial.println("hello");
      }
    }
  }
}

