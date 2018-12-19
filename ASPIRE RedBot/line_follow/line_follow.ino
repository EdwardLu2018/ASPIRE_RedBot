#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5
 
#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6
 
int sensorPin1 = A3; 
//int sensorPin2 = A6;
int sensorPin3 = A7;

int right;
//int center;
int left;

int x = 1;

#define LINETHRESHOLD 150

 void setup()
{
  Serial.begin(9600);
 
  pinMode(L_CTRL1, OUTPUT);  
  pinMode(L_CTRL2, OUTPUT);  
  pinMode(L_PWM, OUTPUT);  
 
  pinMode(R_CTRL1, OUTPUT);  
  pinMode(R_CTRL2, OUTPUT);  
  pinMode(R_PWM, OUTPUT);  

  pinMode(sensorPin1, INPUT);
//  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(13, OUTPUT);

while (x==1) {
   
  if ((left < LINETHRESHOLD) && (right >= LINETHRESHOLD))
  {
    leftMotor(80);
    rightMotor(0); 
   // delay(150);
  }
  
  else if ((right < LINETHRESHOLD) && (left >= LINETHRESHOLD))
  {
      leftMotor(0);
      rightMotor(80); 
    // delay(150);
  }

  else if ((left < LINETHRESHOLD) && (right < LINETHRESHOLD))
  {
      rightMotor(0);
      leftMotor(0); 
    //  delay(150);
  } 
  
  else if ((left >= LINETHRESHOLD) && (right >= LINETHRESHOLD))
  {
      rightMotor(80);
      leftMotor(80);
     // delay(150);
  }
  }  
}


void loop()
{
  right = analogRead(sensorPin1);
  //center = analogRead(sensorPin2);
  left = analogRead(sensorPin3);

  
} 
/*****************************************************************/

void leftMotor(int motorPower)
{
  motorPower = constrain(motorPower, -255, 255);   
  if(motorPower <= 0)
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
  if(motorPower <= 0)
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


