
#include "../.pio/libdeps/uno/HCSR04_ID6099/src/HCSR04.h"
#include "../.pio/libdeps/uno/CmdMessenger_ID171/CmdMessenger.h"
#include "../.pio/libdeps/uno/TaskScheduler_ID721/src/TaskScheduler.h"
#include "../.pio/libdeps/uno/Servo_ID883/src/Servo.h"

#include "../include/Pin.h"
#include "../include/GearMotor.h"
#include "../include/Buzzer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VEHICLE_CRASH_DISTANCE 10
#define VEHICLE_AWARE_DISTANCE 20
#define BEEP_DURATION 50
#define MIDDLE_ANGLE 140
#define SONAR_MAX_RANGE 300
#define RUN_AWAY_DISTANCE 30
#define SONAR_SINGLE_ANGLE 35
#define SONAR_INITIAL_ANGLE 70

int16_t current_temperature = 20;

Scheduler scheduler;
CmdMessenger cmdMessenger = CmdMessenger(Serial);
HCSR04 hcsr04 = HCSR04(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO, current_temperature, SONAR_MAX_RANGE);
Servo servoMotor;
bool acceptingCommands = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum {
    COMMAND_RIGHT_GEARMOTOR,     //0
    COMMAND_LEFT_GEARMOTOR,      //1
    COMMAND_SONAR,               //2
    COMMAND_HORN,                //3
    COMMAND_ACK,                 //4
    COMMAND_SERVO,               //5
    COMMAND_OBSTACLE_DISTANCE,   //6
    COMMAND_RUN_AWAY_ANGLE,      //7
    COMMAND_SEARCH_NEW_PATH,     //8
    COMMAND_SLOW_DOWN            //9
};

void avoidCrash();

void changeDirection(uint8_t angle);

bool isAcceptingCommands();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OnUnknownCommand() {
    cmdMessenger.sendCmd(COMMAND_ACK, "OnUnknownCommand");
}

void OnLeftWheelCommand() {
    if (isAcceptingCommands()) {
        const int speed = cmdMessenger.readInt16Arg();
        if (!cmdMessenger.isArgOk()) {
            return;
        }
        moveLeftGearMotor(speed);
        cmdMessenger.sendCmd(COMMAND_LEFT_GEARMOTOR, speed);
    }
}

void OnRightWheelCommand() {
    if (isAcceptingCommands()) {
        const int speed = cmdMessenger.readInt16Arg();
        if (!cmdMessenger.isArgOk()) {
            return;
        }
        moveRightGearMotor(speed);
        cmdMessenger.sendCmd(COMMAND_RIGHT_GEARMOTOR, speed);
    }
}

void OnSonarCommand() {
    if (isAcceptingCommands()) {
        float distance = hcsr04.getMedianFilterDistance();
        cmdMessenger.sendCmd(COMMAND_SONAR, (int) distance);
    }
}

void honkHorn() {
    if (isAcceptingCommands()) {
        tone(PIN_BUZZER, frequency('b'), BEEP_DURATION * 3);
        //    tone(PIN_BUZZER, frequency(' '), BEEP_DURATION);
        //    tone(PIN_BUZZER, frequency('C'), BEEP_DURATION);
    }
}

void OnSpinServoCommand() {
    if (isAcceptingCommands()) {
        const int angle = cmdMessenger.readInt16Arg();
        servoMotor.write(angle);
    }
}

bool isAcceptingCommands() {
    if (!acceptingCommands) {
        tone(PIN_BUZZER, frequency('g'), BEEP_DURATION * 5);
    }
    return acceptingCommands;
}

void changeDirection(uint8_t angle) {
    if (angle >= 170) {
        moveRightGearMotor(200);
        moveLeftGearMotor(-200);
        delay(400);
    } else if (angle >= 125) {
        moveRightGearMotor(200);
        moveLeftGearMotor(-200);
        delay(200);
    } else if (angle >= 80) {
        moveRightGearMotor(0);
        moveLeftGearMotor(0);
        delay(0);
    } else if (angle >= 35) {
        moveRightGearMotor(-200);
        moveLeftGearMotor(200);
        delay(400);
    } else if (angle >= 0) {
        moveRightGearMotor(-200);
        moveLeftGearMotor(200);
        delay(200);
    }
    stopVehicle();
    cmdMessenger.sendCmd(COMMAND_RUN_AWAY_ANGLE, angle);
}

void searchNewPath() {
    acceptingCommands = false;
    uint8_t runAwayAngle = 255;
    servoMotor.write(0);
    for (uint8_t i = 0; i * SONAR_SINGLE_ANGLE <= MIDDLE_ANGLE; i++) {
        float distance = hcsr04.getMedianFilterDistance();
        if (RUN_AWAY_DISTANCE < distance && distance < SONAR_MAX_RANGE) {
            runAwayAngle = i * SONAR_SINGLE_ANGLE;
        }
        cmdMessenger.sendCmdStart(COMMAND_OBSTACLE_DISTANCE);
        cmdMessenger.sendCmdArg(i * SONAR_SINGLE_ANGLE);
        cmdMessenger.sendCmdArg(distance);
        cmdMessenger.sendCmdEnd();
        delay(900);
        servoMotor.write(((i + 1) * SONAR_SINGLE_ANGLE) - 1);
    }

    if (runAwayAngle != 255) {
        cmdMessenger.sendCmd(COMMAND_RUN_AWAY_ANGLE, runAwayAngle);
        changeDirection(runAwayAngle);
    }
    servoMotor.write(SONAR_INITIAL_ANGLE);
    acceptingCommands = true;
}

void attachCommandCallbacks() {
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);
    cmdMessenger.attach(COMMAND_LEFT_GEARMOTOR, OnLeftWheelCommand);
    cmdMessenger.attach(COMMAND_RIGHT_GEARMOTOR, OnRightWheelCommand);
    cmdMessenger.attach(COMMAND_SONAR, OnSonarCommand);
    cmdMessenger.attach(COMMAND_HORN, honkHorn);
    cmdMessenger.attach(COMMAND_SERVO, OnSpinServoCommand);
    cmdMessenger.attach(COMMAND_SEARCH_NEW_PATH, searchNewPath);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void beep_ON();
//
//Task task_beep_ON(TASK_IMMEDIATE, TASK_ONCE, &beep_ON, &scheduler, false);
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void beep_OFF() {
//    //LEDOff();
//}
//
//void beep_ON() {
//    //LEDOn();
//    task_beep_ON.setCallback(&beep_OFF);
//}
//
//void beep() {
//    task_beep_ON.enable();
//}

void avoidCrash() {
    if (!isStopped()) {
        if (hcsr04.getMedianFilterDistance() <= VEHICLE_CRASH_DISTANCE) {
            tone(PIN_BUZZER, frequency('c'), BEEP_DURATION * 2);
            stopVehicle();
            searchNewPath();
        }
        if (hcsr04.getMedianFilterDistance() <= VEHICLE_AWARE_DISTANCE) {
            slowDown();
            cmdMessenger.sendCmdStart(COMMAND_SLOW_DOWN);
            cmdMessenger.sendCmdArg(currentSpeed[0]);
            cmdMessenger.sendCmdArg(currentSpeed[1]);
            cmdMessenger.sendCmdEnd();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);

    scheduler.init();
    //scheduler.addTask(task_beep);

    servoMotor.write(SONAR_INITIAL_ANGLE);

    initGearMotors();

    hcsr04.begin();

    cmdMessenger.printLfCr();
    attachCommandCallbacks();

    servoMotor.attach(PIN_SERVO);

    cmdMessenger.sendCmd(COMMAND_ACK, "Vehicle is ready!");
}

void loop() {
    scheduler.execute();
    cmdMessenger.feedinSerialData();
    avoidCrash();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////