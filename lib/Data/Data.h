#ifndef Data_h
#define Data_h

#include "Constants.h"
#include "Arduino.h"

typedef struct{
  bool inUse;
  int pinAttached;
} pwmChannel;

class Data{
  public:
    Data();
    uint32_t Baudrate;

    pwmChannel pwmChannels[MAX_PWM_CHANNELS];
};

extern Data commonData;

#endif