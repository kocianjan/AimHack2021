// Arduino motor shield L293D

// pripojeni knihoven
#include <AFMotor.h>
#include <Servo.h> 

// pripojeni DC motoru na konektor M1, M2
AF_DCMotor motorL(1);
AF_DCMotor motorR(2);

static int START_DELAY = 3;
static int MAX_SPEED = 127;


void setup() {
  // zapnuti prace s DC motorem
  motorL.setSpeed(MAX_SPEED);
  motorL.run(RELEASE);
  motorR.setSpeed(MAX_SPEED);
  motorR.run(RELEASE);
}

void loop() {
  //move(FORWARD, RELEASE, 1000); // left only
  //move(RELEASE, FORWARD, 1000); // right only
  //circle();
}

void circle() {
  forward(100);
  turnLeft(20);
}

void startMotors(int maxSpeed, int startDelay) {
  for (int i = 0; i < maxSpeed; i++) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(startDelay);
  }
}

void startMotors() {
  startMotors(MAX_SPEED, START_DELAY);
}

void stopMotors(int maxSpeed, int startDelay) {
  for (int i = maxSpeed; i >= 0; i--) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(startDelay);
  }
}

void stopMotors() {
  stopMotors(MAX_SPEED, START_DELAY);
}

void move(int leftMotorDirection, int rightMotorDirection, int moveTime) {
  motorL.run(leftMotorDirection);
  motorR.run(rightMotorDirection);
  startMotors();
  delay(moveTime);
  stopMotors();
}

void forward(int moveTime) {
  move(FORWARD, FORWARD, moveTime);
}

void back(int moveTime) {
  move(BACKWARD, BACKWARD, moveTime);
}

void turnRight(int moveTime) {
  move(FORWARD, BACKWARD, moveTime);
}

void turnLeft(int moveTime) {
  move(BACKWARD, FORWARD, moveTime);
}
