//
// Created by mrbueno on 14/4/20.
//

#ifndef FROMIDE_SONAR_H
#define FROMIDE_SONAR_H


#include "../../../include/Pin.h"
#include "../../../.pio/libdeps/uno/HCSR04_ID6099/src/HCSR04.h"

class Sonar {
    HCSR04 ultrasonicSensor = HCSR04(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO, 20, 300);
public:
    Sonar();
    float getMedianFilterDistance();

    const float OUT_OF_RANGE = 38000;
};


#endif //FROMIDE_SONAR_H
