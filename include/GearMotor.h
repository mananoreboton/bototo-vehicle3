//
// Created by mrbueno on 25/4/20.
//

#ifndef FROMIDE_GEARMOTOR_H
#define FROMIDE_GEARMOTOR_H

void initGearMotors() {
    pinMode(PIN_LEFT_GEARMOTOR_PWD, OUTPUT);
    pinMode(PIN_LEFT_GEARMOTOR_AHEAD, OUTPUT);
    pinMode(PIN_LEFT_GEARMOTOR_REVERSE, OUTPUT);
    pinMode(PIN_RIGHT_GEARMOTOR_PWD, OUTPUT);
    pinMode(PIN_RIGHT_GEARMOTOR_AHEAD, OUTPUT);
    pinMode(PIN_RIGHT_GEARMOTOR_REVERSE, OUTPUT);
}

void move(int speed, uint8_t pwdPin, uint8_t aheadPin, uint8_t reversePin) {
    if (speed == 0) {
        digitalWrite(aheadPin, LOW);
        digitalWrite(reversePin, LOW);
    } else if (speed > 0) {
        digitalWrite(reversePin, LOW);
        digitalWrite(aheadPin, HIGH);
    } else if (speed < 0) {
        speed *= -1;
        digitalWrite(aheadPin, LOW);
        digitalWrite(reversePin, HIGH);
    }
    analogWrite(pwdPin, speed);
}

void moveRightGearMotor(int speed) {
    move(speed, PIN_RIGHT_GEARMOTOR_PWD, PIN_RIGHT_GEARMOTOR_AHEAD, PIN_RIGHT_GEARMOTOR_REVERSE);
}

void moveLeftGearMotor(int speed) {
    move(speed, PIN_LEFT_GEARMOTOR_PWD, PIN_LEFT_GEARMOTOR_AHEAD, PIN_LEFT_GEARMOTOR_REVERSE);
}

void stopVehicle() {
    moveRightGearMotor(0);
    moveLeftGearMotor(0);
}

#endif //FROMIDE_GEARMOTOR_H
