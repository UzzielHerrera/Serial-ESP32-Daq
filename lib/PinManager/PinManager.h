#ifndef PinManager_h
#define PinManager_h

#include "Constants.h"
#include "Arduino.h"
#include "Pin.h"

class PinManager{
  public:
    PinManager();
    Pin *PinController[MAX_PIN];
    void attach(Pin *pinx);
  private:
    uint8_t attachedPins;
};

extern PinManager pinManager;

#endif