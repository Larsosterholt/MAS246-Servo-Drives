#include <SPI.h>
#include <Stepper.h>
#include "dac.h"


#ifndef MOTOR_h
#define MOTOR_h


class Stepping {
public:
  Stepping() {
    //Initialice dac
    dac_init();
    set_dac(4095, 4095);

    //Initialice stepper pins
    pinMode(A12, OUTPUT);  // b phase
    pinMode(A13, OUTPUT);  // b enable
    pinMode(A14, OUTPUT);  // a phase
    pinMode(A15, OUTPUT);  // a enable
  }

  // motor pins:
  int motor_pin_1 = A15;
  int motor_pin_2 = A13;
  int motor_pin_3 = A14;
  int motor_pin_4 = A12;
  int number_of_steps = 200;

  bool doorClosed = 1;  // Door closed when 1

  //8 halfstep in door-open-dirrection:
  void halfStepUp(int Delay) {
    //set_dac(4095, 4095);

    for (int i = 0; i < 8; i++) {
      delay(Delay);
      //Serial.println("halfStep()");
      switch (i) {
        case 0:  // 1100
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, LOW);
          digitalWrite(motor_pin_4, LOW);
          //Serial.println("case 0");
          break;
        case 1:  // 01x0
          digitalWrite(motor_pin_1, LOW);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, LOW);
          //Serial.println("case 1");
          break;
        case 2:  //1110
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, LOW);
          //Serial.println("case 2");
          break;
        case 3:  //101x
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, LOW);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 3");
          break;
        case 4:  //1111
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 4");
          break;
        case 5:  //01x1
          digitalWrite(motor_pin_1, LOW);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 5");
          break;
        case 6:  //1101
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, LOW);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 6");
          break;
        case 7:  //100x
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, LOW);
          digitalWrite(motor_pin_3, LOW);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 7");
          break;
      }
    }
  }

  //8 halfstep in door-close-dirrection:
  void halfStepDwn(int Delay) {
    //set_dac(4095, 4095);

    for (int i = 8; i > 0; i--) {
      delay(Delay);
      //Serial.println("halfStep()");
      switch (i) {
        case 0:  // 1100
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, LOW);
          digitalWrite(motor_pin_4, LOW);
          //Serial.println("case 0");
          break;
        case 1:  // 01x0
          digitalWrite(motor_pin_1, LOW);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, LOW);
          //Serial.println("case 1");
          break;
        case 2:  //1110
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, LOW);
          //Serial.println("case 2");
          break;
        case 3:  //101x
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, LOW);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 3");
          break;
        case 4:  //1111
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 4");
          break;
        case 5:  //01x1
          digitalWrite(motor_pin_1, LOW);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, HIGH);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 5");
          break;
        case 6:  //1101
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, HIGH);
          digitalWrite(motor_pin_3, LOW);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 6");
          break;
        case 7:  //100x
          digitalWrite(motor_pin_1, HIGH);
          digitalWrite(motor_pin_2, LOW);
          digitalWrite(motor_pin_3, LOW);
          digitalWrite(motor_pin_4, HIGH);
          //Serial.println("case 7");
          break;
      }
    }
  }

  bool closeDoor() {
    Serial.println("Closing door...");
    for (int i = 0; i < 50; i++) {
      halfStepUp(5);
    }
    Serial.println("Door closed");
    doorClosed = 1;
    return 1;
  }

  bool openDoor() {
    Serial.println("Opening door..");
    for (int i = 0; i < 50; i++) {
      halfStepDwn(5);
    }
    Serial.println("Door open");
    doorClosed = 0;
    return 0;
  }

  void beforMooving(int doorOpenTime) {

    if (doorClosed == 1) {
      openDoor();
    }
    delay(doorOpenTime);
    closeDoor();
    delay(doorOpenTime);
  }

  void afterMooving(int doorOpenTime) {
    delay(doorOpenTime);
    openDoor();
    delay(doorOpenTime);
    closeDoor();
  }
};


class Servo {
public:
  //varibles for the floor
  const int stepsPerRev = 2096;

  int steps = 0;
  void emStop() {
    run(0);  // stops motor
  }

  int actualFloor = 1;  //defalt floor is 1;

  // go to function. Takes a floor as input, calculate number of steps and move and uses a pid
  // regulator for position controll
  int goTo(int floor) {

    if (floor == actualFloor) { return 0; }  // If input==actual, it returns.

    Serial.print("Going to: ");
    Serial.print(floor);
    Serial.println(". floor....");
    //resetting steps
    steps = 0;
    //compute numer of steps to move:
    int target = (floor - actualFloor) * stepsPerRev;

    // Temporary varible used to ensure the floor is reached, not just once.
    int temp = 0;

    while (1) {

      //Calculate PWM to motor with PID:
      long int motorSpeed = (long int)computePID((float)steps, (float)target);

      //Run motor with calculated value:
      run(motorSpeed);

      // Checs if floor i reached, and returns if OK:
      if (abs(target - steps) <= 1) {
        temp++;
      }
      // this uses a temorary varilbel "temp", checs if it has been incrimentet 10 times, this is to be shure tha the elevator have reached the floor. And nor just passed it.
      if (temp >= 10) {
        run(0);
        actualFloor = floor;
        Serial.print("Reached: ");
        Serial.print(floor);
        Serial.println(". floor");
        return actualFloor;
      }
    }
  }
private:

  //Servo motor
  int servoPwm = 7;  // Pin PWM is conetcted to, for controlling speed
  int servoDir = 6;  // Pin Servo dirrectron [Phase]
  int speedMap = 0;

  // For PID
  float kp = 1;
  float ki = 0.001;
  float kd = 0.4;
  const float dt = 10;  // ms
  unsigned long currentTime = 0, previousTime = 0;
  float elapsedTime;
  float error;
  float lastError;
  float cumError, rateError;

  float computePID(float actual, float SetPoint) {

    currentTime = millis();                             //get current time
    elapsedTime = (float)(currentTime - previousTime);  //compute time elapsed from previous computation

    //Waiting foor loop time:
    while (!(elapsedTime >= dt)) {
      currentTime = millis();                             //get current time
      elapsedTime = (float)(currentTime - previousTime);  //compute time elapsed from previous computation
    }

    error = SetPoint - actual;                      // determine error
    cumError += error * elapsedTime;                // compute integral
    rateError = (error - lastError) / elapsedTime;  // compute derivative

    // Durty-fix to prevent integrator wind up
    // Switches if off when the error is small:
    if (abs(error) >= 5) {
      cumError = 0;
    }

    float out = kp * error + ki * cumError + kd * rateError;  //PID output

    lastError = error;           //remember current error
    previousTime = currentTime;  //remember current time

    return out;  //have function return the PID output
  }


  // This function chooses wich dirrection tu run the
  // motor based on the sing of the innput:
  void run(long int speed) {
    if (speed > 0) {
      if (speed > 100) { speed = 100; }  //setting max speed
      runUp(speed);

    } else if (speed <= 0) {
      if (abs(speed) > 100) { speed = 100; }  //setting max speed
      runDown(abs(speed));
    }
  }


  void runUp(long int Speed) {
    digitalWrite(servoDir, 0);  // setting dirrection
    analogWrite(servoPwm, Speed);
  }

  void runDown(long int Speed) {
    digitalWrite(servoDir, 1);  // setting dirrection
    analogWrite(servoPwm, Speed);
  }
};

#endif