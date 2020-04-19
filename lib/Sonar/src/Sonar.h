//
// Created by mrbueno on 14/4/20.
//

#ifndef FROMIDE_SONAR_H
#define FROMIDE_SONAR_H


#include "../../../include/Pin.h"
#include "../../../.pio/libdeps/uno/HCSR04_ID6099/src/HCSR04.h"

class Sonar {

public:
    Sonar();
    float getMedianFilterDistance();

    const float OUT_OF_RANGE = 38000;
};


#endif //FROMIDE_SONAR_H
