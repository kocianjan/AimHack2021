// Arduino motor shield L293D

// pripojeni knihoven
#include <AFMotor.h>
#include <Servo.h> 

// pripojeni DC motoru na konektor M1, M2
AF_DCMotor motorL(1);
AF_DCMotor motorR(2);

int i;
int dt = 3;
int runtime = 2000;
int maxSpeed = 127;

void setup() {
  // zapnuti prace s DC motorem
  motorL.setSpeed(maxSpeed);
  motorL.run(RELEASE);
  motorR.setSpeed(maxSpeed);
  motorR.run(RELEASE);
}

void loop() {
  //forwardAndBack();
}

void startMotors(int maxSpeed, int startDelay) {
  for (i = 0; i < maxSpeed; i++) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(startDelay);
  }
}

void startMotors() {
  startMotors(maxSpeed, dt);
}

void stopMotors(int maxSpeed, int startDelay) {
  for (i = maxSpeed; i >= 0; i--) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(startDelay);
  }
}

void stopMotors() {
  stopMotors(maxSpeed, dt);
}

void forwardAndBack() {
  motorL.run(FORWARD);
  motorR.run(FORWARD);
  startMotors();
  delay(runtime);
  stopMotors();
  
  motorL.run(BACKWARD);
  motorR.run(BACKWARD);
  startMotors();
  delay(runtime);
  stopMotors();
}
