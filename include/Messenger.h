//
// Created by mrbueno on 29/03/19.
//

#ifndef ARDUINOTESTBT_COMMANDS_H
#define ARDUINOTESTBT_COMMANDS_H

#include "../.pio/libdeps/uno/CmdMessenger_ID171/CmdMessenger.h"
#include "../lib/GearMotor/src/GearMotor.h"
#include "../lib/Sonar/src/Sonar.h"

enum {
    COMMAND_LEFT_GEARMOTOR,
    COMMAND_RIGHT_GEARMOTOR,

    COMMAND_SONAR,

    COMMAND_ACK,
};

CmdMessenger cmdMessenger = CmdMessenger(Serial);
GearMotor gearMotor = GearMotor();
Sonar sonar = Sonar();

// Called when a received command has no attached function
void OnUnknownCommand() {
    cmdMessenger.sendCmd(COMMAND_ACK, "OnUnknownCommand");
}

void OnLeftWheelCommand() {

    const int16_t speed = cmdMessenger.readInt16Arg();
    if (!cmdMessenger.isArgOk()) {
        return;
    }
    gearMotor.moveLeftGearMotor(speed);
}

void OnRightWheelCommand() {

    const int16_t speed = cmdMessenger.readInt16Arg();
    if (!cmdMessenger.isArgOk()) {
        return;
    }
    gearMotor.moveRightGearMotor(speed);
}

void OnSonarCommand(){
    float distance = sonar.getMedianFilterDistance();
    cmdMessenger.sendCmd(COMMAND_SONAR,(int)distance);
}


void attachCommandCallbacks() {
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);
    cmdMessenger.attach(COMMAND_LEFT_GEARMOTOR, OnLeftWheelCommand);
    cmdMessenger.attach(COMMAND_RIGHT_GEARMOTOR, OnRightWheelCommand);
    cmdMessenger.attach(COMMAND_SONAR, OnSonarCommand);
}

#endif //ARDUINOTESTBT_COMMANDS_H
