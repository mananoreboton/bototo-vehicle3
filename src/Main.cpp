
#include "../.pio/libdeps/uno/HCSR04_ID6099/src/HCSR04.h"
#include "../.pio/libdeps/uno/CmdMessenger_ID171/CmdMessenger.h"
#include "../.pio/libdeps/uno/TaskScheduler_ID721/src/TaskScheduler.h"
#include "../include/Pin.h"
#include "../include/GearMotor.h"
#include "../include/Buzzer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VEHICLE_CRASH_DISTANCE 10
#define BEEP_DURATION 50

int16_t current_temperature = 20;
int16_t sonar_max_range = 300;

Scheduler scheduler;
CmdMessenger cmdMessenger = CmdMessenger(Serial);
HCSR04 hcsr04 = HCSR04(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO, current_temperature, sonar_max_range);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum {
    COMMAND_RIGHT_GEARMOTOR, //0
    COMMAND_LEFT_GEARMOTOR,  //1
    COMMAND_SONAR,           //2
    COMMAND_HORN,            //3
    COMMAND_ACK              //4
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OnUnknownCommand() {
    cmdMessenger.sendCmd(COMMAND_ACK, "OnUnknownCommand");
}

void OnLeftWheelCommand() {
    const int speed = cmdMessenger.readInt16Arg();
    if (!cmdMessenger.isArgOk()) {
        return;
    }
    moveLeftGearMotor(speed);
    cmdMessenger.sendCmd(COMMAND_LEFT_GEARMOTOR, speed);
}

void OnRightWheelCommand() {
    const int speed = cmdMessenger.readInt16Arg();
    if (!cmdMessenger.isArgOk()) {
        return;
    }
    moveRightGearMotor(speed);
    cmdMessenger.sendCmd(COMMAND_RIGHT_GEARMOTOR, speed);
}

void OnSonarCommand() {
    float distance = hcsr04.getMedianFilterDistance();
    cmdMessenger.sendCmd(COMMAND_SONAR, (int) distance);
}

void honkHorn() {
    tone(PIN_BUZZER, frequency('b'), BEEP_DURATION * 3);
//    tone(PIN_BUZZER, frequency(' '), BEEP_DURATION);
//    tone(PIN_BUZZER, frequency('C'), BEEP_DURATION);
}

void attachCommandCallbacks() {
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);
    cmdMessenger.attach(COMMAND_LEFT_GEARMOTOR, OnLeftWheelCommand);
    cmdMessenger.attach(COMMAND_RIGHT_GEARMOTOR, OnRightWheelCommand);
    cmdMessenger.attach(COMMAND_SONAR, OnSonarCommand);
    cmdMessenger.attach(COMMAND_HORN, honkHorn);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(9600);

    scheduler.init();
    //scheduler.addTask(task_beep);

    initGearMotors();

    hcsr04.begin();

    cmdMessenger.printLfCr();
    attachCommandCallbacks();
    cmdMessenger.sendCmd(COMMAND_ACK, "Vehicle is ready!");
}

void loop() {
    scheduler.execute();
    cmdMessenger.feedinSerialData();
    if (hcsr04.getMedianFilterDistance() <= VEHICLE_CRASH_DISTANCE) {
        honkHorn();
        stopVehicle();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////