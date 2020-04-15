//
// Created by mrbueno on 14/4/20.
//

#include "Sonar.h"

Sonar::Sonar() {
    ultrasonicSensor.begin();
}

float Sonar::getMedianFilterDistance() {
    float distance = ultrasonicSensor.getMedianFilterDistance();
    if (distance == HCSR04_OUT_OF_RANGE) {
        distance = OUT_OF_RANGE;
    }
    return distance;
}
