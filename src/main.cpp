#include <Arduino.h>

#include "ircommand.h"
#include "fan_control.h"
#include "web_facade.h"

#include "local_wifi.h"
#include <ESPmDNS.h>
#include <WiFi.h>

#define ESPALEXA_ASYNC
#include <Espalexa.h>

const byte LED_PORT=2;
// const byte IR_RECEIVE_PIN=15;
const byte IR_SEND_PIN=18;
// const byte IR_SEND_PIN=9;

Espalexa *espalexa;
FanControl *fanControl;

void handleAlexaAction(uint8_t brightness, uint32_t rgb){
  if (rgb <= 0x000100){
    fanControl->startRotating();
  } else{
    fanControl->stopRotating();
  }
  if (brightness < 20){
    fanControl->turnOff();
  } else if (brightness < 85){
    fanControl->setSpeed(1);
  } else if (brightness < 165){
    fanControl->setSpeed(2);
  } else{
    fanControl->setSpeed(3);
  }
};

// // const uint16_t kCaptureBufferSize = 1024;
// // const uint8_t kTimeout = 50;  // Milli-Seconds
// // const uint16_t kFrequency = 38000;  // in Hz. e.g. 38kHz.

// // IRrecv irrecv(IR_RECEIVE_PIN, kCaptureBufferSize, kTimeout, false);

void setup() {
  pinMode(LED_PORT, OUTPUT);
  // // Serial.begin(115200, SERIAL_8N1);
  Serial.begin(115200);

  // irrecv.enableIRIn();

  IRCommander *irCommander = new IRCommander(IR_SEND_PIN);
  fanControl = new FanControl(irCommander);

  connectToWifi();
  MDNS.begin("fan");

  espalexa = new Espalexa();
  espalexa->addDevice("Fan", handleAlexaAction);
  WebFacade *webFacade = new WebFacade(fanControl, espalexa);

}

// decode_results results;

void loop() {
  // if (irrecv.decode(&results)) { 
  //   uint16_t *raw_array = resultToRawArray(&results);
  //   // Find out how many elements are in the array.
  //   uint16_t length = getCorrectedRawLength(&results);
  //   for (int i=0; i<length; i++){
  //     printf("%d, ", raw_array[i]);
  //   }
  //   printf("\nlength: %d\n", length);
  //   irrecv.resume();
  //   // Deallocate the memory allocated by resultToRawArray().
  //   delete [] raw_array;
  // }
  espalexa->loop();
  delay(1);
}