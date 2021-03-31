#include <Servo.h>
#include "QSerial.h"
QSerial myIRserial;
int THRESHOLD = 900;
Servo grip, tilt, pan;

//Configuring robot pins
int leftSensor = A0;
int centerSensor = A1;
int rightSensor = A2;
int fowwardSensor = A5;
int rSpe = 6;
int rDir = 7;
int lSpe = 5;
int lDir = 4;
int bumpers = 12;
int IR = 9;//
int panPin = 10;//
int tiltPin = 11;//
int gripPin = 8;//

//values need to be changed depending on battery level
int reverseDelay = 700;
int turnSensingDelay = 300;
int turnDelay = 0;
int reverseGrabDelay = 325;
int rSpeed = 158;
int lSpeed = 150;
int rSpeedReverse = 125;
int lSpeedReverse = 120;
int secondTurnSpeed = 18;
/////////////////////////////////////


//constants for grabbing and releasing
int grabHeight = 58;
int straightPan = 90;
int holdDice = 180;
int releaseDice = 0;
int straightUp = 160;

void setup() {
  Serial.begin(9600);

  //set pinmodes
  pinMode(rDir, OUTPUT);
  pinMode(lDir, OUTPUT);
  pinMode(rSpe, OUTPUT);
  pinMode(lSpe, OUTPUT);
  pinMode(bumpers, INPUT);
  pinMode(IR, INPUT);
  myIRserial.attach(IR, -1);
  
  //setup servo
  attachServo(true);
  grip.write(releaseDice);
  delay(100);
  tilt.write(straightUp);
  delay(100);
  pan.write(straightPan);
  delay(100);
  attachServo(false);


}

//loops until a valid ascii code is read
void loop() {
  int val = myIRserial.receive(100);
  if (char(val) == '0' || char(val) == '1' || char(val) == '2') {
    if (char(val) == '0') {
      position1();
    }
    if (char(val) == '1') {
      position2();
    }
    if (char(val) == '2') {
      position3();
    }

  }
}


void turnRight()
{
  Serial.println("RIGHT TURN");
  delay(turnDelay);
  digitalWrite(rDir, LOW);
  digitalWrite(lDir, HIGH);
  analogWrite(rSpe, rSpeed - 30);
  analogWrite(lSpe, lSpeed - 30);
  delay(turnSensingDelay);
  while (analogRead(rightSensor) < THRESHOLD) {}
  analogWrite(rSpe, secondTurnSpeed);    //-----added these three
  analogWrite(lSpe, secondTurnSpeed);
  while (analogRead(centerSensor) < THRESHOLD) {}
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
}

void turnLeft()
{
  Serial.println("LEFT TURN");
  delay(turnDelay);
  digitalWrite(rDir, HIGH);
  digitalWrite(lDir, LOW);
  analogWrite(rSpe, rSpeed - 30);
  analogWrite(lSpe, lSpeed - 30);
  delay(turnSensingDelay);
  while (analogRead(leftSensor) < THRESHOLD) {}
  analogWrite(rSpe, secondTurnSpeed);  //----added these three
  analogWrite(lSpe, secondTurnSpeed);
  while (analogRead(centerSensor) < THRESHOLD) {}
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
}

void turnAround()
{
  Serial.println("TURN AROUND");
  delay(turnDelay);
  digitalWrite(rDir, LOW);
  digitalWrite(lDir, HIGH);
  analogWrite(rSpe, rSpeed - 30);
  analogWrite(lSpe, lSpeed - 30);
  delay(turnSensingDelay);
  while (analogRead(rightSensor) < THRESHOLD) {}
  //  analogWrite(rSpe, secondTurnSpeed);    //------added these three
  //  analogWrite(lSpe, secondTurnSpeed);
  //  while (analogRead(centerSensor) < THRESHOLD) {}
  delay(100);
  digitalWrite(rDir, HIGH);
  digitalWrite(lDir, LOW);
  analogWrite(rSpe, rSpeed - 30);
  analogWrite(lSpe, lSpeed - 30);
  delay(200);
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
}

void grab()
{
  Serial.println("HITING TO GRAB");
  digitalWrite(rDir, HIGH);
  digitalWrite(lDir, HIGH);
  analogWrite(rSpe, rSpeed);
  analogWrite(lSpe, lSpeed);
  while (digitalRead(bumpers) == 1) {
    if ((analogRead(rightSensor) > THRESHOLD) && (analogRead(leftSensor) < THRESHOLD)) {    //Auto-calibration
      analogWrite(rSpe, rSpeed - 60);
      analogWrite(lSpe, lSpeed);
    }
    else if ((analogRead(rightSensor) < THRESHOLD) && (analogRead(leftSensor) > THRESHOLD)) {
      analogWrite(rSpe, rSpeed);
      analogWrite(lSpe, lSpeed - 60);
    }
    else {
      analogWrite(rSpe, rSpeed);
      analogWrite(lSpe, lSpeed);
    }
  }
  Serial.println("GRABBING");
  delay(50);                    //---------Changed this from 200 --------------
  digitalWrite(rDir, LOW);
  digitalWrite(lDir, LOW);
  analogWrite(rSpe, rSpeedReverse);
  analogWrite(lSpe, lSpeedReverse);
  delay(reverseGrabDelay);
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
  delay(50);               //---------Changed this from 200 -------------
  attachServo(true);
  tilt.write(straightUp);
  delay(0);
  pan.write(straightPan);
  delay(0);
  grip.write(0);
  delay(150);
  tilt.write(grabHeight);
  delay(1000);              //---------Changed this from 1500 -------------
  grip.write(180);
  delay(1000);
  tilt.write(straightUp);
  delay(250);
  attachServo(false);
  delay(0);
  reverse();
  delay(300);
  turnAround();
}

void release()
{
  digitalWrite(rDir, HIGH);
  digitalWrite(lDir, HIGH);
  analogWrite(rSpe, rSpeed);
  analogWrite(lSpe, lSpeed);

  while (digitalRead(bumpers) == 1) {
    Serial.println("RELEASE FOLLOW");
    if ((analogRead(rightSensor) > THRESHOLD) && (analogRead(leftSensor) < THRESHOLD)) {    //Auto-calibration
      analogWrite(rSpe, rSpeed - 60);
      analogWrite(lSpe, lSpeed);
    }
    else if ((analogRead(rightSensor) < THRESHOLD) && (analogRead(leftSensor) > THRESHOLD)) {
      analogWrite(rSpe, rSpeed);
      analogWrite(lSpe, lSpeed - 60);
    }
    else {
      analogWrite(rSpe, rSpeed);
      analogWrite(lSpe, lSpeed);
    }
  }
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
  attachServo(true);
  tilt.write(straightUp);
  delay(0);
  pan.write(90);
  delay(100);                 //---------Changed this from 250 -------------
  tilt.write(grabHeight);
  delay(50);
  grip.write(0);
  delay(250);
  tilt.write(180);
  delay(250);
  attachServo(false);
  delay(0);
  digitalWrite(rDir, LOW);
  digitalWrite(lDir, LOW);
  analogWrite(rSpe, rSpeedReverse);
  analogWrite(lSpe, lSpeedReverse);
  delay(300);
  turnRight();
}

void waitButton()
{
  while (digitalRead(bumpers) == HIGH) {}
  while (digitalRead(bumpers) == LOW) {}
  delay(500);
}

void thresholdTester()
{
  Serial.print(analogRead(leftSensor));
  Serial.print("\t");
  Serial.print(analogRead(centerSensor));
  Serial.print("\t");
  Serial.print(analogRead(rightSensor));
  Serial.println();
}

void forwardTester()
{
  digitalWrite(rDir, HIGH);
  digitalWrite(lDir, HIGH);
  analogWrite(rSpe, rSpeed);
  analogWrite(lSpe, lSpeed);
}

void reverseTester()
{
  digitalWrite(rDir, LOW);
  digitalWrite(lDir, LOW);
  analogWrite(rSpe, rSpeedReverse);
  analogWrite(lSpe, lSpeedReverse);
}

void lineFollowInt(int i) {
  for (int n = 0; n < i; n++) {
    digitalWrite(rDir, HIGH);
    digitalWrite(lDir, HIGH);
    analogWrite(rSpe, rSpeed);
    analogWrite(lSpe, lSpeed);
    while (1) {
      Serial.println("LINEFOLLOW");
      if ((analogRead(rightSensor) > THRESHOLD) && (analogRead(leftSensor) < THRESHOLD)) {    //Auto-calibration
        analogWrite(rSpe, rSpeed - 60);
        analogWrite(lSpe, lSpeed);
      }
      else if ((analogRead(rightSensor) < THRESHOLD) && (analogRead(leftSensor) > THRESHOLD)) {
        analogWrite(rSpe, rSpeed);
        analogWrite(lSpe, lSpeed - 60);
      }
      else {
        analogWrite(rSpe, rSpeed);
        analogWrite(lSpe, lSpeed);
      }
      if ((analogRead(rightSensor) > THRESHOLD) && (analogRead(centerSensor) > THRESHOLD ) && (analogRead(leftSensor) > THRESHOLD)) {
        delay(250);
        break;
      }
    }
  }
}

void reverse()
{
  digitalWrite(rDir, LOW);
  digitalWrite(lDir, LOW);
  analogWrite(rSpe, rSpeedReverse);
  analogWrite(lSpe, lSpeedReverse);
  while (!((analogRead(rightSensor) > THRESHOLD) && (analogRead(centerSensor) > THRESHOLD) && (analogRead(leftSensor) > THRESHOLD))) {}
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
}

void attachServo(bool tf)
{
  if (tf == true)
  {
    grip.attach(gripPin);
    tilt.attach(tiltPin);
    pan.attach(panPin);
  }
  else
  {
    tilt.detach();
    pan.detach();
  }
}

void position1()
{
  // -------First object-------
  lineFollowInt(5);
  turnRight();
  grab();
  lineFollowInt(3);
  turnLeft();
  release();

  //-----------Second-----------
  lineFollowInt(1);
  turnLeft();
  grab();
  lineFollowInt(1);
  turnRight();
  release();

  //----------Third-----------
  grab();
  release();

  //----------Fourth-----------
  lineFollowInt(2);
  turnLeft();
  grab();
  lineFollowInt(1);
  turnRight();
  release();

  //-----------Fifth------------
  lineFollowInt(4);
  turnLeft();
  grab();
  lineFollowInt(1);
  turnRight();
  release();
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
  while (true) {}
}
void position2 ()
{
  lineFollowInt(3);
  turnRight();
  grab();
  lineFollowInt(2);
  turnLeft();
  release();
  //
  lineFollowInt(3);
  turnLeft();
  grab();
  lineFollowInt(2);
  turnRight();
  release();
  //
  grab();
  release();
  //
  lineFollowInt(5);
  turnLeft();
  lineFollowInt(2);
  turnRight();
  grab();
  lineFollowInt(1);
  turnLeft();
  lineFollowInt(2);
  turnRight();
  release();
  //
  lineFollowInt(5);
  turnRight();
  lineFollowInt(2);
  turnLeft();
  grab();
  lineFollowInt(1);
  turnRight();
  lineFollowInt(2);
  turnLeft();
  release();
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
  while (true) {}
}
void position3()
{
  //------------------1--------
  lineFollowInt(1);
  turnRight();
  grab();
  lineFollowInt(1);
  turnLeft();
  release();
  //--------------------2-----------
  lineFollowInt(5);
  turnLeft();
  grab();
  lineFollowInt(3);
  turnRight();
  release();
  //--------------------3-------------
  grab();
  release();
  //--------------------4-----------
  lineFollowInt(2);
  turnRight();
  grab();
  lineFollowInt(1);
  turnLeft();
  release();
  //--------------5-----------
  lineFollowInt(4);
  turnRight();
  grab();
  lineFollowInt(1);
  turnLeft();
  release();
  analogWrite(rSpe, 0);
  analogWrite(lSpe, 0);
  while (true) {}

}
