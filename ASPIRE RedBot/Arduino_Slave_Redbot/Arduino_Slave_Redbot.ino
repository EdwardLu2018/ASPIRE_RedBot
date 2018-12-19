#include <Wire.h>    //declare I2C library function
#define LED A2
#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

#define LINETHRESHOLD 150
byte line1;

int sensorPin1 = A3;
int sensorPin3 = A7;

int right;
int left;
int x;    //variable x determine whether led on master is on or off//
//initializing
void setup()
{
  //join I2Cbus as slave with identified address
  // Wire.onReceive(receiveEvent);    //register a function to be called when slave receive a transmission from master//
  // Wire.onRequest(requestEvent);    //register a function when master request data from this slave device//
  Serial.begin(115200);   //set serial baud rate

  pinMode(L_CTRL1, OUTPUT);  
  pinMode(L_CTRL2, OUTPUT);  
  pinMode(L_PWM, OUTPUT);  
 
  pinMode(R_CTRL1, OUTPUT);  
  pinMode(R_CTRL2, OUTPUT);  
  pinMode(R_PWM, OUTPUT);  

  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(13, OUTPUT);
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
//main program


void loop()
{ if (Serial.available())
  {
    line1 = Serial.read();

    while (line1 == 1)
    {
      lineFollow();
    }
  }
}
// if the line is under the right sensor, adjust relative speeds to turn to the right

///*******************************************************************************/
void lineFollow() {
  right = analogRead(sensorPin1);
  left = analogRead(sensorPin3);

  if ((left < LINETHRESHOLD) && (right >= LINETHRESHOLD))
  {
    leftMotor(80);
    rightMotor(0);
  }
  // if the line is under the left sensor, adjust relative speeds to turn to the left
  else if ((right < LINETHRESHOLD) && (left >= LINETHRESHOLD))
  {
    leftMotor(0);
    rightMotor(80);
  }
  // if on the line, drive left and right at the same speed (left is CCW / right is CW)
  else if ((left < LINETHRESHOLD) && (right < LINETHRESHOLD))
  {
    pinMode(LED, OUTPUT);
    Wire.begin(4);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    rightMotor(0);
    leftMotor(0);
  }

  else if ((left >= LINETHRESHOLD) && (right >= LINETHRESHOLD))
  {
    rightMotor(80);
    leftMotor(80);
  }
}
/*******************************************************************************/
//when slave receive string from master, trigger this event.
void receiveEvent(int howMany)
{
  if (( Wire.available() > 1) && (left < LINETHRESHOLD) && (right < LINETHRESHOLD)  ) // execute repeatedly until last byte left in the data packet from master//
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
{ if ((left < LINETHRESHOLD) && (right < LINETHRESHOLD)  ) {
    //transfer last byte of data packet receiving from master to master
    char c = Wire.write(x);
    //receive one byte from slave and assign to variable c
    //if c is 0, light up led
    //char c = Wire.read();
    if (x == 1)
    {
      digitalWrite(LED, LOW);
    }
    else loop(); {
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
    }
  }
}

