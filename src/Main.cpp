#include <Messenger.h>

// Setup function
void setup()
{
    Serial.begin(9600);
    cmdMessenger.printLfCr();
    attachCommandCallbacks();

    cmdMessenger.sendCmd(COMMAND_ACK,"Arduino has started!");
}

// Loop function
void loop()
{
    cmdMessenger.feedinSerialData();
}