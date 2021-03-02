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
  motorL.run(FORWARD);
  motorR.run(FORWARD);
  for (i = 0; i < maxSpeed; i++) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(dt);
  }
  delay(runtime);
  for (i = maxSpeed; i >= 0; i--) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(dt);
  }
  
  motorL.run(BACKWARD);
  motorR.run(BACKWARD);
  for (i=0; i < maxSpeed; i++) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(dt);
  }
  delay(runtime);
  for (i = maxSpeed; i >= 0; i--) {
    motorL.setSpeed(i);
    motorR.setSpeed(i);
    delay(dt);
  }
}
