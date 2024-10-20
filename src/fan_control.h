#ifndef fan_control_h
#define fan_control_h

#include "ircommand.h"
#include "blink.h"

class FanControl{
  private:
    byte speed = 0;
    bool isRotating = false;
    IRCommander *commander;
  
  public:
    FanControl(IRCommander *commander){
        this->commander = commander;
    }

    void turnOn(){
      if (this->speed == 0){
        this->commander->execute(CMD_ON_OFF);
        blinkBuiltinLed(200);
      }
      this->speed = 1;
      this->isRotating = false;
    }

    void turnOff(){
      if (this->speed > 0){
        this->commander->execute(CMD_ON_OFF);
        blinkBuiltinLed(500);
      }
      this->speed = 0;
      this->isRotating = false;
    }

    void reset(){
      this->commander->execute(CMD_ON_OFF);
      this->speed = 0;
      this->isRotating = false;
    }

    void changeSpeed(){
      if (this->speed == 0){
        turnOn();
        delay(200);
        blinkBuiltinLed(200);
      }
      this->commander->execute(CMD_SPEED);
      this->speed = (this->speed % 3) + 1;
    }

    void setSpeed(byte newSpeed){
      if (newSpeed > 3) return;
      while(this->speed != newSpeed){
        this->changeSpeed();
        delay(200);
        blinkBuiltinLed(200);
      }
    }

    void startRotating(){
      if (!isRotating){
        this->commander->execute(CMD_ROTATE);
        blinkBuiltinLed(200);
      }
      isRotating = true;
    }

    void stopRotating(){
      if (isRotating){
        this->commander->execute(CMD_ROTATE);
        blinkBuiltinLed(200);
      }
      isRotating = false;
    }

    int getSpeed(){
      return this->speed;
    }

    bool getIsRotating(){
      return this->isRotating;
    }
};


#endif