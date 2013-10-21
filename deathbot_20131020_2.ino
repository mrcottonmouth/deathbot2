#include <SerialCommand.h>
#include <NewTone.h>
SerialCommand sCmd;     // The demo SerialCommand object

//=====[ PINS ]=================================================================
#define TRIGGER_PIN 4
#define ECHO_PIN 5
#define RANDOM_ANALOG_PIN 2             /**< unused analog pin to use as random seed */
int pinI1=8;//define I1 interface
int pinI2=11;//define I2 interface 
int speedpinA=9;//enable motor A
int pinI3=12;//define I3 interface 
int pinI4=13;//define I4 interface 
int speedpinB=10;//enable motor B
int spead =127;//define the spead of motor
int led = A5;
//#define MotionDet A1*/
int IRcameraPin = A0;

//=====[ VARIABLES ]============================================================
int pos = 100; 
int LeftSpeed = 0;
int RightSpeed = 0;
int Throttle;
int LeftRight;
int LeftTrim;
int RightTrim;
int Reverse;
int Claw;
int RobotPower;
int SuperSpeed;
int LiGht;
int IRcamera;
unsigned long currentMillis = millis();
unsigned long previousMillis = 0; // last time update
long interval = 2000; // interval at which to do something (milliseconds)
int AliveFlag = 0;
int SPinL = 0;
int SPinR = 0;

//=====[ SETUP ]================================================================
void setup() {
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  digitalWrite(speedpinA,LOW);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
  digitalWrite(speedpinB,LOW);
  Serial.begin(115200);
  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("T",     processThrottle);
  sCmd.addCommand("L",     processLeftRight);
  sCmd.addCommand("R",     processReverse);
  sCmd.addCommand("C",     processClaw);
  sCmd.addCommand("P",     processRobotPower);
  sCmd.addCommand("S",     processSuperSpeed);
  sCmd.addCommand("G",     processLiGht);
  sCmd.addCommand("I",     processIRcamera);
  sCmd.addCommand("SPL",     processSPL);
  sCmd.addCommand("SPR",     processSPR);
//  sCmd.addCommand("ON",    LED_on);          // Turns LED on
//  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
//  sCmd.addCommand("HELLO", sayHello);        // Echos the string argument back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  Serial.println("Deathbot Ready");
}

//=====[ LOOP ]================================================================
void loop() {
  sCmd.readSerial();     // We don't do much, just process serial commands
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      AliveFlag = 0;
  }     
  if (AliveFlag == 1)  {
    Motivate();
  }
}

//=====[ FUNCTIONS ]============================================================
void Motivate()  {
  if (RobotPower != 0)  {
    LeftSpeed = abs(Throttle + LeftTrim);
    RightSpeed = abs(Throttle + RightTrim);
    if (SuperSpeed == 1)  {
      LeftSpeed = abs(LeftSpeed * 2);
      RightSpeed = abs(RightSpeed * 2);
    }
    switch (Reverse)  {
      case 0:  ///Going forward
        Serial.println("Going forward");
        Serial.print("Right speed:");
        Serial.println(RightSpeed);
        Serial.print(" Left speed:");
        Serial.println(LeftSpeed);
        digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
        digitalWrite(pinI3,LOW);
        digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
        digitalWrite(pinI1,HIGH);
        analogWrite(speedpinA,constrain(LeftSpeed, 50, 150));//input a simulation value to set the speed
        analogWrite(speedpinB,constrain(RightSpeed, 50, 150));
        break;
      case 1:  //Reverse enabled
        Serial.println("Going Reverse");
        Serial.print("Right speed:");
        Serial.println(RightSpeed);
        Serial.print(" Left speed:");
        Serial.println(LeftSpeed);
        digitalWrite(pinI4,LOW);//turn DC Motor B move anticlockwise
        digitalWrite(pinI3,HIGH);
        digitalWrite(pinI2,HIGH);//turn DC Motor A move clockwise
        digitalWrite(pinI1,LOW);
        analogWrite(speedpinA,constrain(LeftSpeed, 50, 150));//input a simulation value to set the speed
        analogWrite(speedpinB,constrain(RightSpeed, 50, 150));
        break;
    }
      AliveFlag = 0;
  }
  else  {
    Stoppp();
  }
}

void Stoppp()  {
  digitalWrite(speedpinA,LOW);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
  digitalWrite(speedpinB,LOW);
  LeftSpeed = 0;
  RightSpeed = 0;
}

void processSPL()  {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL) {
    SPinL = atoi(arg);    // Converts a char string to an integer
    switch(SPinL)  {
      case 0:
        Stoppp();
        break;
      case 1:
        Serial.println("Spin Left");
        digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
        digitalWrite(pinI3,LOW);
        digitalWrite(pinI2,HIGH);//turn DC Motor A move anticlockwise
        digitalWrite(pinI1,LOW);
        analogWrite(speedpinA,120);
        analogWrite(speedpinB,120);
        AliveFlag = 0;
        break;
      }
  }
}

void processSPR()  {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL) {
    SPinR = atoi(arg);    // Converts a char string to an integer
    switch(SPinR)  {
      case 0:
        Stoppp();
        break;
      case 1:
        Serial.println("Spin Right");
        digitalWrite(pinI4,LOW);//turn DC Motor B move clockwise
        digitalWrite(pinI3,HIGH);
        digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
        digitalWrite(pinI1,HIGH);
        analogWrite(speedpinA,120);
        analogWrite(speedpinB,120);
        AliveFlag = 0;
        break;
      }
  }
}

void processThrottle() {
  char *arg;
//  Serial.println("Throttle");
  arg = sCmd.next();
  if (arg != NULL) {
    Throttle = atoi(arg);    // Converts a char string to an integer
//    Serial.print("T Value: ");
//    Serial.println(Throttle);
    AliveFlag = 1;
  }
  else {
//    Serial.println("No arguments");
  }
}

void processLeftRight() {
  char *arg;
//  Serial.println("LeftRight");
  arg = sCmd.next();
  if (arg != NULL) {
    LeftRight = atoi(arg);    // Converts a char string to an integer
//    Serial.print("L Value: ");
//    Serial.println(LeftRight);
    if (LeftRight < 0)  {
      LeftTrim = abs(LeftRight) * -1;
      RightTrim = 0;
    }
    else if  (LeftRight > 0)  {
      RightTrim = abs(LeftRight) * -1;
      LeftTrim = 0;
    }
    else  {
      RightTrim = 0;
      LeftTrim = 0;
    }
    AliveFlag = 1;
  }
  else {
//    Serial.println("No arguments");
  }
}

void processReverse() {
  char *arg;
//  Serial.println("Reverse");
  arg = sCmd.next();
  if (arg != NULL) {
    Reverse = atoi(arg);    // Converts a char string to an integer
//    Serial.print("R Value: ");
//    Serial.println(Reverse);
    AliveFlag = 1;
  }
  else {
    Serial.println("No arguments");
  }
}

void processClaw() {
  char *arg;
//  Serial.println("Claw");
  arg = sCmd.next();
  if (arg != NULL) {
    Claw = atoi(arg);    // Converts a char string to an integer
//    Serial.print("C Value: ");
//    Serial.println(Claw);
  }
  else {
//    Serial.println("No arguments");
  }
}

void processRobotPower() {
  char *arg;
//  Serial.println("RobotPower");
  arg = sCmd.next();
  if (arg != NULL) {
    RobotPower = atoi(arg);    // Converts a char string to an integer
//    Serial.print("P Value: ");
//    Serial.println(RobotPower);
    if (RobotPower == 1)  {
//      Serial.println("Breaks Off!!");
      digitalWrite(speedpinA,HIGH);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
      digitalWrite(speedpinB,HIGH);
      AliveFlag = 1;
    }
    else if (RobotPower == 0)  {
//      Serial.print("Breaks On");
      Stoppp();
      AliveFlag = 1;
    }
  }
  else {
//    Serial.println("No arguments");
  }
}

void processSuperSpeed() {
  char *arg;
//  Serial.println("SuperSpeed");
  arg = sCmd.next();
  if (arg != NULL) {
    SuperSpeed = atoi(arg);    // Converts a char string to an integer
//    Serial.print("S Value: ");
//    Serial.println(SuperSpeed);
    AliveFlag = 1;
  }
  else {
//    Serial.println("No arguments");
  }
}

void processLiGht() {
  char *arg;
//  Serial.println("LiGht");
  arg = sCmd.next();
  if (arg != NULL) {
    LiGht = atoi(arg);    // Converts a char string to an integer
//    Serial.print("G Value: ");
//    Serial.println(LiGht);
    if (LiGht == 1)  {
      digitalWrite(led, LOW);
//      Serial.println("LiGht On");
    }
    else if (LiGht == 0)  {
      digitalWrite(led, HIGH);
//      Serial.println("LiGht Off");
    }
  }
  else {
//    Serial.println("No arguments");
  }
}

void processIRcamera() {
  char *arg;
//  Serial.println("IRcamera");
  arg = sCmd.next();
  if (arg != NULL) {
    IRcamera = atoi(arg);    // Converts a char string to an integer
//    Serial.print("I Value: ");
//    Serial.println(IRcamera);
    if (IRcamera == 0)  {
      digitalWrite(IRcameraPin, LOW);
//      Serial.println("Camera Off");
    }
    else if (IRcamera == 1)  {
      digitalWrite(IRcameraPin, HIGH);
//      Serial.println("Camera On");
    }
  }
  else {
//    Serial.println("No arguments");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
//  Serial.println("What?");
}

/*
void LED_on() {
  Serial.println("LED on");
  digitalWrite(arduinoLED, HIGH);
}

void LED_off() {
  Serial.println("LED off");
  digitalWrite(arduinoLED, LOW);
}

void sayHello() {
  char *arg;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL) {    // As long as it existed, take it
    Serial.print("Hello ");
    Serial.println(arg);
  }
  else {
    Serial.println("Hello, whoever you are");
  }
}

void processCommand() {
  int aNumber;
  char *arg;

  Serial.println("We're in processCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No arguments");
  }

  /*arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atol(arg);
    Serial.print("Second argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No second argument");
  }
}
*/



