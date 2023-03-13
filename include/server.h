#ifndef _SERVER_H_
#define _SERVER_H_

//IMPORTAÇÕES
#include "config.h"
#include "sensor.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

//PROTÓTIPOS
void setupWiFi(void);
void setupServer(void);
void setupPins(void);
void handleRoot(void);
void handleNotFound(void);
void handleClient(void);
void execute(String action, String value);
String head(void);
String body(void);
String button(int number, int index);

#endif