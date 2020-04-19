
#include "../.pio/libdeps/uno/HCSR04_ID6099/src/HCSR04.h"
#include "../.pio/libdeps/uno/CmdMessenger_ID171/CmdMessenger.h"

#define PIN_LEFT_GEARMOTOR_PWD 5
#define PIN_LEFT_GEARMOTOR_AHEAD 6
#define PIN_LEFT_GEARMOTOR_REVERSE 7

#define PIN_RIGHT_GEARMOTOR_PWD 10
#define PIN_RIGHT_GEARMOTOR_AHEAD 8
#define PIN_RIGHT_GEARMOTOR_REVERSE 9

#define PIN_ULTRASONIC_TRIGGER 3
#define PIN_ULTRASONIC_ECHO 11

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int16_t temperature = 20;
int16_t max_distance = 300;

CmdMessenger cmdMessenger = CmdMessenger(Serial);
HCSR04 ultrasonicSensor = HCSR04(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO, temperature, max_distance);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum {
    COMMAND_LEFT_GEARMOTOR,
    COMMAND_RIGHT_GEARMOTOR,

    COMMAND_SONAR,

    COMMAND_ACK,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initGearMotors() {
        pinMode(PIN_LEFT_GEARMOTOR_PWD, OUTPUT);
        pinMode(PIN_LEFT_GEARMOTOR_AHEAD, OUTPUT);
        pinMode(PIN_LEFT_GEARMOTOR_REVERSE, OUTPUT);
        pinMode(PIN_RIGHT_GEARMOTOR_PWD, OUTPUT);
        pinMode(PIN_RIGHT_GEARMOTOR_AHEAD, OUTPUT);
        pinMode(PIN_RIGHT_GEARMOTOR_REVERSE, OUTPUT);
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

void moveRightGearMotor(int speed) {
    move(speed, PIN_RIGHT_GEARMOTOR_PWD, PIN_RIGHT_GEARMOTOR_AHEAD, PIN_RIGHT_GEARMOTOR_REVERSE);
}

void moveLeftGearMotor(int speed) {
    move(speed, PIN_LEFT_GEARMOTOR_PWD, PIN_LEFT_GEARMOTOR_AHEAD, PIN_LEFT_GEARMOTOR_REVERSE);
}

void stopLeftGearMotor() {
    stop(PIN_LEFT_GEARMOTOR_PWD, PIN_LEFT_GEARMOTOR_AHEAD, PIN_LEFT_GEARMOTOR_REVERSE);
}

void stopRightGearMotor() {
    stop(PIN_RIGHT_GEARMOTOR_PWD, PIN_RIGHT_GEARMOTOR_AHEAD, PIN_RIGHT_GEARMOTOR_REVERSE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OnUnknownCommand() {
    cmdMessenger.sendCmd(COMMAND_ACK, "OnUnknownCommand");
}

void OnLeftWheelCommand() {

    const int16_t speed = cmdMessenger.readInt16Arg();
    if (!cmdMessenger.isArgOk()) {
        return;
    }
    moveLeftGearMotor(speed);
}

void OnRightWheelCommand() {

    const int16_t speed = cmdMessenger.readInt16Arg();
    if (!cmdMessenger.isArgOk()) {
        return;
    }
    moveRightGearMotor(speed);
}

void OnSonarCommand(){
    float distance = ultrasonicSensor.getMedianFilterDistance();
    cmdMessenger.sendCmd(COMMAND_SONAR,(int)distance);
}


void attachCommandCallbacks() {
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);
    cmdMessenger.attach(COMMAND_LEFT_GEARMOTOR, OnLeftWheelCommand);
    cmdMessenger.attach(COMMAND_RIGHT_GEARMOTOR, OnRightWheelCommand);
    cmdMessenger.attach(COMMAND_SONAR, OnSonarCommand);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
    Serial.begin(9600);

    initGearMotors();

    ultrasonicSensor.begin();

    cmdMessenger.printLfCr();
    attachCommandCallbacks();
    cmdMessenger.sendCmd(COMMAND_ACK,"Arduino has started!");
}

void loop()
{
    cmdMessenger.feedinSerialData();
}