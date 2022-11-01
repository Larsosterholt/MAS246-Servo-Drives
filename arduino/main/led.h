#ifndef LED_h
#define LED_h
#include <Arduino.h>

class LED {
  //Class of updating led's
private:
  int led1 = 48;
  int led2 = 47;
  int led3 = 46;

public:
  void setLED(int floor) {
    digitalWrite(led1, 0);
    digitalWrite(led2, 0);
    digitalWrite(led3, 0);

    switch (floor) {
      case 0:
        // do nothing
        break;
      case 1:
        digitalWrite(led1, 1);
        break;
      case 2:
        digitalWrite(led2, 1);
        break;
      case 3:
        digitalWrite(led3, 1);
        break;
    }
  }
};


#endif