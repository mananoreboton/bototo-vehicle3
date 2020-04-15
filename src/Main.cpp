#include <Arduino.h>
#include "../lib/Sonar/src/Sonar.h"
#include "../.pio/libdeps/uno/CmdMessenger_ID171/CmdMessenger.h"

Sonar sonar;
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// This is the list of recognized commands. These can be commands that can either be sent or received. 
// In order to receive, attach a callback function to these events
enum
{
    kSetLed              , // Command to request led to be set in specific state
    kStatus              , // Command to report status
};

// Called when a received command has no attached function
void OnUnknownCommand()
{
    cmdMessenger.sendCmd(kStatus,"Command without attached callback");
}

// Callback function that sets led on or off
void OnSetLed()
{
    // Read led state argument, interpret string as boolean
    bool ledState = cmdMessenger.readBoolArg();
    // Set led
    float distance = sonar.getMedianFilterDistance();
    // Send back status that describes the led state
    cmdMessenger.sendCmd(kStatus,(int)distance);
}

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);
    cmdMessenger.attach(kSetLed, OnSetLed);
}



// Setup function
void setup()
{
    // Listen on serial connection for messages from the PC
    Serial.begin(9600);

    // Adds newline to every command
    cmdMessenger.printLfCr();

    // Attach my application's user-defined callback methods
    attachCommandCallbacks();

    // Send the status to the PC that says the Arduino has booted
    // Note that this is a good debug function: it will let you also know 
    // if your program had a bug and the arduino restarted  
    cmdMessenger.sendCmd(kStatus,"Arduino has started!");

}

// Loop function
void loop()
{
    // Process incoming serial data, and perform callbacks
    cmdMessenger.feedinSerialData();
}