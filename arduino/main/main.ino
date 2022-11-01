#include "led.h"
#include "motor.h"
#include "dac.h"

//For encoder
#define CLK 20
#define DT 21

//  Varibles for encoder
int counter = 0;
int currentStateCLK;
int lastStateCLK;


// Creating global Servo object
Servo elevator;

// Creating global Stepping object (door)
Stepping door;

// Creating global LED object
LED led;


void setup() {
  //Serial
  Serial.begin(9600);
  Serial.setTimeout(1);

  //LED
  int led1 = 48;
  int led2 = 47;
  int led3 = 46;
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  //Servo motor
  dac_init();
  set_dac(4095, 4095);
  int servoPwm = 7;  // Pin PWM is conetcted to, for controlling speed
  int servoDir = 6;  // Pin Servo dirrectron [Phase]
  pinMode(servoDir, OUTPUT);
  pinMode(servoPwm, OUTPUT);

  //Initialice stepper pins
  pinMode(A12, OUTPUT);  // b phase
  pinMode(A13, OUTPUT);  // b enable
  pinMode(A14, OUTPUT);  // a phase
  pinMode(A15, OUTPUT);  // a enable


  //Encoder
  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);


  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  // Attaching intterupt callback funcktion:
  attachInterrupt(1, updateEncoder, CHANGE);
  attachInterrupt(2, updateEncoder, CHANGE);
}

void updateEncoder() {

  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      elevator.steps--;

    } else {
      // Encoder is rotating CW so increment
      elevator.steps++;
    }
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}


// Varibles used in main loop/state machine
int userInputInt = 0;
int elevatorRetVal = 0;  // not in use
int doorOpenTime = 500;  //[ms] time the door is open/closed
bool alamActive = 0;

bool okToMoove() {
  if (door.doorClosed == 1) {
    //door.closeDoor();
    return 1;
  }
}

void loop() {

  userInputInt = Serial.parseInt();  // Read data from serial/Python GUI

  // Som print out for debugging:
  Serial.print("Actual floor: ");
  Serial.print(elevator.actualFloor);
  Serial.print(" | Door is: ");
  if (door.doorClosed == 1) {
    Serial.println("closed");
  } else if (door.doorClosed == 0) {
    Serial.println("open");
  }

  delay(1000);  //main loop/state machine  "frequency"


  // Use "userInputInt" as innput in state machine:
  switch (userInputInt) {
    case 0:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      // Do nothing, no new innput (Waiting/defalt)
      break;


      // floor buttons (outside cabin)
    case 1:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      if (elevator.actualFloor == 1) {
        door.beforMooving(doorOpenTime);

        break;
      }
      Serial.println("Elevator is called to 1. floor (outside cabin)");
      //door.beforMooving(doorOpenTime);
      if (okToMoove() == 1) {
        elevatorRetVal = elevator.goTo(1);
      }
      door.afterMooving(doorOpenTime);
      break;

    case 23:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      if (elevator.actualFloor == 2) {
        door.beforMooving(doorOpenTime);
        break;
      }
      Serial.println("Elevator is called to 2. floor (outside cabin), person is going up");
      //door.beforMooving(doorOpenTime);
      if (okToMoove() == 1) {
        elevatorRetVal = elevator.goTo(2);
      }
      door.afterMooving(doorOpenTime);
      break;

    case 21:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      if (elevator.actualFloor == 2) {
        door.beforMooving(doorOpenTime);
        break;
      }
      Serial.println("Elevator is called to 2. floor (outside cabin), person is going down");
      //door.beforMooving(doorOpenTime);
      if (okToMoove() == 1) {
        elevatorRetVal = elevator.goTo(2);
      }
      door.afterMooving(doorOpenTime);
      break;

    case 3:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      if (elevator.actualFloor == 3) {
        door.beforMooving(doorOpenTime);
        break;
      }
      Serial.println("Elevator is called to 3. floor (outside cabin)");
      //door.beforMooving(doorOpenTime);
      if (okToMoove() == 1) {
        elevatorRetVal = elevator.goTo(3);
      }
      door.afterMooving(doorOpenTime);
      break;
      //Elevator buttons (inside cabin)
    case 11:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      if (elevator.actualFloor == 1) {
        break;
      }
      Serial.println("Elevator is called to 1. floor (inside cabin)");
      if (door.doorClosed == 0) {
        door.closeDoor();
      }
      if (okToMoove() == 1) {
        elevatorRetVal = elevator.goTo(1);
      }
      door.afterMooving(doorOpenTime);
      break;
    case 22:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      if (elevator.actualFloor == 2) {
        break;
      }
      Serial.println("Elevator is called to 2. floor (inside cabin)");
      if (door.doorClosed == 0) {
        door.closeDoor();
      }
      if (okToMoove() == 1) {
        elevatorRetVal = elevator.goTo(2);
      }
      door.afterMooving(doorOpenTime);
      break;
    case 33:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      if (elevator.actualFloor == 3) {
        break;
      }
      Serial.println("Elevator is called to 3. floor (inside cabin)");
      if (door.doorClosed == 0) {
        door.closeDoor();
      }
      if (okToMoove() == 1) {
        elevatorRetVal = elevator.goTo(3);
      }
      door.afterMooving(doorOpenTime);
      break;

    case 911:
      if (alamActive == 1) {
        Serial.println("Alam active! Press 999 to rest!");
        break;
      }
      elevator.emStop();  // stops servo motor
      alamActive = 1;
      break;
    case 999:
      alamActive = 0;
      break;
  }

  led.setLED(elevator.actualFloor);  // updating indicating led's
}