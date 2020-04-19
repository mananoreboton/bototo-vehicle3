//
// Created by mrbueno on 19/4/20.
//

#ifndef FROMIDE_GEARMOTOR_H
#define FROMIDE_GEARMOTOR_H

#include <HID.h>
#include "../../../include/Pin.h"

class GearMotor {
private:


public:
    void stopRightGearMotor();

    void stopLeftGearMotor();

    void moveRightGearMotor(int speed);

    void moveLeftGearMotor(int speed);
};


#endif //FROMIDE_GEARMOTOR_H
