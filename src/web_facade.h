#ifndef web_facade_h
#define web_facade_h

#include "fan_control.h"
#include "local_wifi.h"
#include <ESPAsyncWebServer.h>
// #define ESPALEXA_DEBUG
#define ESPALEXA_ASYNC
#include <Espalexa.h>


class WebFacade{
private:
    AsyncWebServer *server;
    FanControl *fanControl;
    Espalexa *espalexa;
    

    char* getJsonStatus(){
        char *json = new char[80];
        const int speed = fanControl->getSpeed();
        const char *status = (speed>0)? "On" : "Off";
        const char *isRotating = (fanControl->getIsRotating()? "true" : "false");
        sprintf(json, "{\"status\": \"%s\", \"speed\": %d, \"isRotating\": %s}", status, speed, isRotating);
        return json;
    }

    void sendStatus(AsyncWebServerRequest *request){
        char *json = this->getJsonStatus();
        request->send(200, "application/json", json);
        blinkBuiltinLed(200);
        delete json;
    }
    
public:
    void setupRoutes(){
        WebFacade *self = this;

        this->server->onNotFound([self](AsyncWebServerRequest *request){
            if (!self->espalexa->handleAlexaApiCall(request)) {
                request->send(404, "text/plain", "Not found");
            }
        });

        this->server->on("/on", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->turnOn();
            self->sendStatus(request);
        });

        this->server->on("/off", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->turnOff();
            self->sendStatus(request);
        });

        this->server->on("/reset", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->reset();
            self->sendStatus(request);
        });

        this->server->on("/speed", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->changeSpeed();
            self->sendStatus(request);
        });

        this->server->on("/maxSpeed", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->setSpeed(3);
            self->sendStatus(request);
        });

        this->server->on("/minSpeed", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->setSpeed(1);
            self->sendStatus(request);
        });

        this->server->on("/rotate", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->startRotating();
            self->sendStatus(request);
        });

        this->server->on("/still", HTTP_GET, [self](AsyncWebServerRequest *request){
            self->fanControl->stopRotating();
            self->sendStatus(request);
        });

        this->server->on("/status", HTTP_GET, [self](AsyncWebServerRequest *request){
        self->sendStatus(request);
        });
    }


    WebFacade(FanControl *fanControl, uint16_t port = 80){
        this->fanControl = fanControl;
        this->server = new AsyncWebServer(port);
        setupRoutes();
    }

    WebFacade(FanControl *fanControl, Espalexa *espalexa, uint16_t port = 80){
        this->fanControl = fanControl;
        this->espalexa = espalexa;
        this->server = new AsyncWebServer(port);
        setupRoutes();
        this->espalexa->begin(this->server);
    }

};

#endif