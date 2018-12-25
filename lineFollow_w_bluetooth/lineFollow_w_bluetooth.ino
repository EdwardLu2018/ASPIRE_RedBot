
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

void setup() {
  Serial.begin(115200);

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

void loop() {

  right = analogRead(sensorPin1);
  left = analogRead(sensorPin3);
  
  line1 = Serial.read();

  if (line1 == 1)
  {
    lineFollow();
  }
}

/*****************************************************************************/

void lineFollow () {
      // if on the line, drive left and right at the same speed (left is CCW / right is CW)
    if ((left >= LINETHRESHOLD) && (right >= LINETHRESHOLD))
    {
      Serial.println("on line");
      rightMotor(80);
      leftMotor(80);
    }

    // if the line is under the right sensor, adjust relative speeds to turn to the right
    else if ((left < LINETHRESHOLD) && (right >= LINETHRESHOLD))
    {
      Serial.println("go right");
      leftMotor(80);
      rightMotor(0);
    }

    // if the line is under the left sensor, adjust relative speeds to turn to the left
    else if ((right < LINETHRESHOLD) && (left >= LINETHRESHOLD))
    {
      Serial.println("go left");
      leftMotor(0);
      rightMotor(80);
    }

    else if ((left < LINETHRESHOLD) && (right < LINETHRESHOLD))
    {
      Serial.println("off line");
      rightMotor(0);
      leftMotor(0);
    }
}

/******************************************************************************/

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

