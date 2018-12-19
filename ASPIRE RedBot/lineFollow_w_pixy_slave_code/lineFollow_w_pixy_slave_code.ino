#include <Wire.h>    //declare I2C library function
#define LED 13

#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

#define LINETHRESHOLD 70

int sensorPin1 = A3;
int sensorPin3 = A7;

int right;
int left;

byte line1;

int x;    //variable x determine whether led on master is on or off

void setup()
{
  Serial.begin(115200);   //set serial baud rate

  pinMode(L_CTRL1, OUTPUT);
  pinMode(L_CTRL2, OUTPUT);
  pinMode(L_PWM, OUTPUT);

  pinMode(R_CTRL1, OUTPUT);
  pinMode(R_CTRL2, OUTPUT);
  pinMode(R_PWM, OUTPUT);

  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin3, INPUT);
}

void loop()
{
  line1 = Serial.read();
  
  while (line1 == 1)
  {
    lineFollow();
  }
}

/*******************************************************************************/
void lineFollow () {
  right = analogRead(sensorPin1);
  left = analogRead(sensorPin3);

  // if the line is under the right sensor, adjust relative speeds to turn to the right
  if ((left < LINETHRESHOLD) && (right >= LINETHRESHOLD))
  {
    leftMotor(100);
    rightMotor(0);
  }
  // if the line is under the left sensor, adjust relative speeds to turn to the left
  else if ((right < LINETHRESHOLD) && (left >= LINETHRESHOLD))
  {
    leftMotor(0);
    rightMotor(100);
  }
  // if on the line, stop the motors and read the cards
  else if ((left < LINETHRESHOLD) && (right < LINETHRESHOLD))
  {
    pinMode(LED, OUTPUT);
    Wire.begin(4);
    Wire.onReceive(receiveEvent); //receive from master
    Wire.onRequest(requestEvent); //request from master
    rightMotor(0);
    leftMotor(0);
  }

  else if ((left >= LINETHRESHOLD) && (right >= LINETHRESHOLD)) //go forward if both sensors don't touch the line
  {
    rightMotor(100);
    leftMotor(100);
  }
}

/*******************************************************************************/
void leftMotor(int motorPower)
{
  motorPower = constrain(motorPower, -255, 255);
  if (motorPower <= 0)
  {
    // forward
    digitalWrite(L_CTRL1, LOW);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(L_PWM, abs(motorPower));
  }
  else
  {
    // reverse
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, LOW);
    analogWrite(L_PWM, abs(motorPower));
  }
}

/*******************************************************************************/
void leftBrake()
{
  digitalWrite(L_CTRL1, HIGH);
  digitalWrite(L_CTRL2, HIGH);
  analogWrite(L_PWM, 0);
}

/*******************************************************************************/
void rightMotor(int motorPower)
{
  motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
  if (motorPower <= 0)
  {
    // forward
    digitalWrite(R_CTRL1, HIGH);
    digitalWrite(R_CTRL2, LOW);
    analogWrite(R_PWM, abs(motorPower));
  }
  else
  {
    // reverse
    digitalWrite(R_CTRL1, LOW);
    digitalWrite(R_CTRL2, HIGH);
    analogWrite(R_PWM, abs(motorPower));
  }
}

/*******************************************************************************/
void rightBrake()
{
  digitalWrite(L_CTRL1, HIGH);
  digitalWrite(L_CTRL2, HIGH);
  analogWrite(R_PWM, 0);
}

/*******************************************************************************/

//when slave receive string from master, trigger this event.
void receiveEvent(int howMany)
{
  if (( Wire.available() > 1) && (left < LINETHRESHOLD) && (right < LINETHRESHOLD)  ) // execute repeatedly until last byte left in the data packet from master 
  {
    char c = Wire.read();    // receive data from master and assign it to char c
    Serial.print(c);    // display char c on serial monitor
  }
  
  //receive last byte of data packet from master
  x = Wire.read();    // receive last data from master and assign it to int x
  Serial.println(x);    // display int x on serial monitor
}

/*******************************************************************************/
//trigger this event when master request data from slave,
void requestEvent()
{ 
  if ((left < LINETHRESHOLD) && (right < LINETHRESHOLD)  ) {
    //transfer last byte of data packet receiving from master to master
    char c = Wire.write(x);

    if (x == 1) //if the pixy (from master) saw pixy signature 1, turn on LED
    { 
      digitalWrite(LED, HIGH);
    }

    else if (x == 0) //if the pixy (from master) saw pixy signature 6, blink LED
    { digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
    }
  }
}
