//
// Created by mrbueno on 19/4/20.
//

#ifndef FROMIDE_GEARMOTOR_H
#define FROMIDE_GEARMOTOR_H

#include <HID.h>
#include "../../../include/Pin.h"

class GearMotor {
private:
    boolean areInitializedGearMotors = false;

    void initGearMotors() {
        if (!areInitializedGearMotors) {
            pinMode(PIN_LEFT_GEARMOTOR_PWD, OUTPUT);
            pinMode(PIN_LEFT_GEARMOTOR_AHEAD, OUTPUT);
            pinMode(PIN_LEFT_GEARMOTOR_REVERSE, OUTPUT);
            pinMode(PIN_RIGHT_GEARMOTOR_PWD, OUTPUT);
            pinMode(PIN_RIGHT_GEARMOTOR_AHEAD, OUTPUT);
            pinMode(PIN_RIGHT_GEARMOTOR_REVERSE, OUTPUT);
        }
        areInitializedGearMotors = true;
    }

    void move(int speed, int pwdPin, int aheadPin, int reversePin) {
        initGearMotors();
        if (speed == 0) {
            digitalWrite(aheadPin, LOW);
            digitalWrite(reversePin, LOW);
        } else if (speed > 0) {
            digitalWrite(aheadPin, HIGH);
            digitalWrite(reversePin, LOW);

        } else if (speed < 0) {
            speed *= -1;
            digitalWrite(aheadPin, LOW);
            digitalWrite(reversePin, HIGH);
        }
        analogWrite(pwdPin, speed);

    }

    void stop(int pwdPin, int aheadPin, int reversePin) {
        initGearMotors();
        analogWrite(pwdPin, 0);
        digitalWrite(aheadPin, LOW);
        digitalWrite(reversePin, LOW);
    }

public:
    void stopRightGearMotor();

    void stopLeftGearMotor();

    void moveRightGearMotor(int speed);

    void moveLeftGearMotor(int speed);
};


#endif //FROMIDE_GEARMOTOR_H
