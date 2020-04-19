//
// Created by mrbueno on 19/4/20.
//

#include "GearMotor.h"

void GearMotor::moveRightGearMotor(int speed) {
    move(speed, PIN_RIGHT_GEARMOTOR_PWD, PIN_RIGHT_GEARMOTOR_AHEAD, PIN_RIGHT_GEARMOTOR_REVERSE);
}

void GearMotor::moveLeftGearMotor(int speed) {
    move(speed, PIN_LEFT_GEARMOTOR_PWD, PIN_LEFT_GEARMOTOR_AHEAD, PIN_LEFT_GEARMOTOR_REVERSE);
}

void GearMotor::stopLeftGearMotor() {
    stop(PIN_LEFT_GEARMOTOR_PWD, PIN_LEFT_GEARMOTOR_AHEAD, PIN_LEFT_GEARMOTOR_REVERSE);
}

void GearMotor::stopRightGearMotor() {
    stop(PIN_RIGHT_GEARMOTOR_PWD, PIN_RIGHT_GEARMOTOR_AHEAD, PIN_RIGHT_GEARMOTOR_REVERSE);
}


