#ifndef State_h
#define State_h

#include "Constants.h"
#include "Arduino.h"

class State{
  public:
    State();
    void blinkHandle();
    int blinksPerCycle;
    uint32_t blinkCycleTime;
    uint32_t blinkLapse;
  private:
    bool _canBlink;
    uint32_t _lastBlinkCycle;
    uint32_t _lastBlink;
    bool _lastBlinkState;
    int _currentBlink;
};

extern State state;

#endif