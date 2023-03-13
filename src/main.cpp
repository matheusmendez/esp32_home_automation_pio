#include "main.h"

void setup(void)
{   
    setupWiFi();
    setupServer();
    setupPins();
}

void loop(void)
{
    handleClient();
}