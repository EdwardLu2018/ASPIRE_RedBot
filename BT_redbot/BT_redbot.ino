

// H-Bridge motor driver pins
#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

// XBee SW_Serial pins
#define    SW_SER_TX A0
#define    SW_SER_RX A1

byte dist; //Defining a variable used for distance (in)
byte turn; //Defining a variable used for turning (degrees)
byte turnRight;

void setup()
{

  //Establishing serial communications:
  Serial.begin(115200);
  while (!Serial);

  //Redbot arduino pins
  pinMode(L_CTRL1, OUTPUT);  // used as a debug pin for an LED.
  pinMode(L_CTRL2, OUTPUT);  // used as a debug pin for an LED.
  pinMode(L_PWM, OUTPUT);  // used as a debug pin for an LED.

  pinMode(R_CTRL1, OUTPUT);  // used as a debug pin for an LED.
  pinMode(R_CTRL2, OUTPUT);  // used as a debug pin for an LED.
  pinMode(R_PWM, OUTPUT);  // used as a debug pin for an LED.

  pinMode(13, OUTPUT);  // used as a debug pin for an LED.

  Serial.println("Starting...");
}

void loop()
{

  if (Serial.available()) //Checks if serial communication is established
  {

    //GOING FORWARD/BACKWARD:

    dist = Serial.read(); //Reads distance from serial input

    //Checks if distance input is positive, indicating that the robot will go forward, running the code below:

    //Printing distance on serial monitor:
    Serial.print("I will go: ");
    Serial.print(dist);
    Serial.print(" inches forward and");
    Serial.println();

    float Time; //Establishes a varable for the milliseconds the motors will run forward
    Time = go(dist); //Establishes time as a result of variable/function "go" (see "float go" below)

    //Commands for redbot moving forward:
    rightMotor(100);
    leftMotor(100);
    delay(Time);
    rightBrake();
    leftBrake();

    //TURNING RIGHT/LEFT:

    turn = Serial.read(); //Reads degrees from serial input
    turnRight = Serial.read();

    //As stated above, if degrees is positive, the robot will turn left


    //Printing degrees on serial monitor:
    Serial.print("I will turn: ");
    Serial.print(turn);
    Serial.print(" degrees left");
    Serial.println();

    float turnTimeL; //Establishing a variable for the milliseconds the motors will run in opposite directions, making the robot turn left
    turnTimeL = goturn(turn); //Establishing time as a result of variable/function "goturn" (see "float goturn" below)

    //Commands for redbot turning left
    rightMotor(125);
    leftMotor(-125);
    delay(turnTimeL);
    rightBrake();
    leftBrake();


    //If the second serial input is not positive, then run code below:


    //Printing degrees on serial monitor:
    Serial.print("I will turn: ");
    Serial.print(turnRight);
    Serial.print(" degrees right");
    Serial.println();

    float turnTimeR; //Establishing a variable for the milliseconds the motors will run in opposite directions, making the robot turn left
    turnTimeR = goturn2(turnRight); //Establishing time as a result of variable/function "goturn" (see "float goturn" below)

    //Commands for redbot turning left
    rightMotor(-125);
    leftMotor(125);
    delay(turnTimeR);
    rightBrake();
    leftBrake();

    Serial.println();
  }
}


//Function for the time redbot will run, translating to distance:
float go(float dist) {
  float result; //Variable for the result of "go"
  result = 1000.0 * (abs(dist) / 8.375); //Multiplies 1000 by the absolute value of the serial input number divided by the speed of the robot (in inches per second). This makes the value put in the first set of delays above.
  return result; //Makes "float go" into the result of equation above, making the moving time for the rebot into result
}

//Function for the time redbot will turn, translating to degrees:
float goturn(float turn) {
  float result1; //Variable for the result of "go"
  result1 =  (1700.0 * abs(turn)) / 360; //Multiplies 1000 by the absolute value of the second serial input number divided by 360 degrees (full circle). This makes the value put in the second set of delays above.
  return result1; //Makes "float goturn" into the result of equation above, which makes the turning time for the redbot into result1
}

float goturn2(float turnRight) {
  float result1; //Variable for the result of "go"
  result1 =  (1700.0 * abs(turn)) / 360; //Multiplies 1000 by the absolute value of the second serial input number divided by 360 degrees (full circle). This makes the value put in the second set of delays above.
  return result1; //Makes "float goturn" into the result of equation above, which makes the turning time for the redbot into result1
}


//Below are the commands for driving and breaking the motors:

/*******************************************************************************/
void leftMotor(int motorPower)
{
  motorPower = constrain(motorPower, -255, 255); //Constrain motorPower to -255 to +255
  if (motorPower >= 0) //could make it <= if robot's wheels are turning in opposite directions
  {
    // spin CW
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, LOW);
    analogWrite(L_PWM, abs(motorPower));
  }
  else
  {
    //Spin CCW
    digitalWrite(L_CTRL1, LOW);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(L_PWM, abs(motorPower));
  }
}

/*******************************************************************************/
void leftBrake()
{
  //Sets both controls HIGH, shorts the motor out -- causing it to self brake.
  digitalWrite(L_CTRL1, HIGH);
  digitalWrite(L_CTRL2, HIGH);
  analogWrite(L_PWM, 0);
}

/*******************************************************************************/
void rightMotor(int motorPower)
{
  motorPower = constrain(motorPower, -255, 255); //Constrains motorPower to -255 to +255
  if (motorPower <= 0) //could make it >= if robot's wheels are turning in opposite directions
  {
    //Spin CW
    digitalWrite(R_CTRL1, HIGH);
    digitalWrite(R_CTRL2, LOW);
    analogWrite(R_PWM, abs(motorPower));
  }
  else
  {
    //Spin CCW
    digitalWrite(R_CTRL1, LOW);
    digitalWrite(R_CTRL2, HIGH);
    analogWrite(R_PWM, abs(motorPower));
  }
}

/*******************************************************************************/
void rightBrake()
{
  //Sets both controls HIGH, shorts the motor out -- causing it to self brake.
  digitalWrite(L_CTRL1, HIGH);
  digitalWrite(L_CTRL2, HIGH);
  analogWrite(R_PWM, 0);
}


