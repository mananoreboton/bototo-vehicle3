//
// Created by mrbueno on 14/4/20.
//

#ifndef FROMIDE_SONAR_H
#define FROMIDE_SONAR_H


#include "../../../.pio/libdeps/uno/HCSR04_ID6099/src/HCSR04.h"

class Sonar {
    HCSR04 ultrasonicSensor = HCSR04(6, 5, 20, 300);
public:
    const float OUT_OF_RANGE = 38000;

    Sonar();
    float getMedianFilterDistance();
};


#endif //FROMIDE_SONAR_H
