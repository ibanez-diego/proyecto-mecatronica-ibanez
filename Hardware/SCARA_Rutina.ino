#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>

// ---------------------------- Pines ----------------------------
#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3

AccelStepper stepper1(1, 2, 5);
AccelStepper stepper2(1, 3, 6);
AccelStepper stepper3(1, 4, 7);
AccelStepper stepper4(1, 12, 13);

Servo gripperServo;

// ---------------------------- Variables ----------------------------
const float theta1AngleToSteps = 44.444444;
const float theta2AngleToSteps = 35.555555;
const float phiAngleToSteps = 10;
const float zDistanceToSteps = 100;

int theta1Array[] = {72 * theta1AngleToSteps, 72 * theta1AngleToSteps, 72 * theta1AngleToSteps, 72 * theta1AngleToSteps, 72 * theta1AngleToSteps, 72 * theta1AngleToSteps};
int theta2Array[] = {72 * theta2AngleToSteps, 72 * theta2AngleToSteps, 72 * theta2AngleToSteps, 72 * theta2AngleToSteps, -86 * theta2AngleToSteps, -86 * theta2AngleToSteps};
int phiArray[] = {0 * phiAngleToSteps, 0 * phiAngleToSteps, 0 * phiAngleToSteps, 0 * phiAngleToSteps, 0 * phiAngleToSteps, 0 * phiAngleToSteps};
int zArray[] = {118 * zDistanceToSteps, 50 * zDistanceToSteps, 50 * zDistanceToSteps, 118 * zDistanceToSteps, 118 * zDistanceToSteps, 118 * zDistanceToSteps};
int gripperArray[] = {25, 90, 90, 90, 90, 25};

int positionsCounter = 6;

String content = "";

void setup() {
  Serial.begin(115200);

  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);
  pinMode(limitSwitch4, INPUT_PULLUP);

  stepper1.setMaxSpeed(4000);
  stepper1.setAcceleration(2000);
  stepper2.setMaxSpeed(4000);
  stepper2.setAcceleration(2000);
  stepper3.setMaxSpeed(4000);
  stepper3.setAcceleration(2000);
  stepper4.setMaxSpeed(4000);
  stepper4.setAcceleration(2000);

  gripperServo.attach(A0, 600, 2500);
  gripperServo.write(25);
  delay(1000);
}

void loop() {
  if (Serial.available()) {
    content = Serial.readString();

    if (content.startsWith("H")) {
      homing();
      Serial.println("OK");
    }

    if (content.startsWith("R")) {
      ejecutarRutina();
      Serial.println("OK");
    }
  }
}

void ejecutarRutina() {
  for (int i = 0; i < positionsCounter; i++) {
    stepper1.moveTo(theta1Array[i]);
    stepper2.moveTo(theta2Array[i]);
    stepper3.moveTo(phiArray[i]);
    stepper4.moveTo(zArray[i]);

    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0 || stepper4.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
      stepper3.run();
      stepper4.run();
    }

    gripperServo.write(gripperArray[i]);
    delay(800);
  }
}

void homing() {
  while (digitalRead(limitSwitch4) != 1) {
    stepper4.setSpeed(1500);
    stepper4.runSpeed();
    stepper4.setCurrentPosition(17000);
  }
  delay(20);
  stepper4.moveTo(10000);
  while (stepper4.distanceToGo() != 0) stepper4.run();

  while (digitalRead(limitSwitch3) != 1) {
    stepper3.setSpeed(-1100);
    stepper3.runSpeed();
    stepper3.setCurrentPosition(-1662);
  }
  delay(20);
  stepper3.moveTo(0);
  while (stepper3.distanceToGo() != 0) stepper3.run();

  while (digitalRead(limitSwitch2) != 1) {
    stepper2.setSpeed(-1300);
    stepper2.runSpeed();
    stepper2.setCurrentPosition(-5420);
  }
  delay(20);
  stepper2.moveTo(0);
  while (stepper2.distanceToGo() != 0) stepper2.run();

  while (digitalRead(limitSwitch1) != 1) {
    stepper1.setSpeed(-1200);
    stepper1.runSpeed();
    stepper1.setCurrentPosition(-3955);
  }
  delay(20);
  stepper1.moveTo(0);
  while (stepper1.distanceToGo() != 0) stepper1.run();
}
