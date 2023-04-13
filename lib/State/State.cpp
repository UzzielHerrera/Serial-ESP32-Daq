#include "State.h"

State::State(){
  blinkCycleTime = BLINK_CYCLE_TIME;
  blinkLapse = BLINK_LAPSE;
  blinksPerCycle = BLINKS_PER_CYCLE;

  _lastBlinkState = false;
  _canBlink = true;
  _lastBlink = millis();
  _lastBlinkCycle = millis();
  _currentBlink = 0;

  pinMode(STATE_LED, OUTPUT);
  digitalWrite(STATE_LED, 0);
  COM.println("State Module initialized.");
}

void State::blinkHandle(void){
  if(blinksPerCycle > 0){
    if(!_canBlink && millis() - _lastBlinkCycle >= blinkCycleTime){
      _lastBlinkCycle = millis();
      _canBlink = true;
      //COM.println("Resetting blink.");
    }

    if(_canBlink && millis() - _lastBlink >= blinkLapse/2) {
      _lastBlink = millis();
      if(_currentBlink < blinksPerCycle) {
        digitalWrite(STATE_LED, !_lastBlinkState);
        if(_lastBlinkState) _currentBlink++;
        //COM.println((String) "_currentBlink: " + _currentBlink );
        _lastBlinkState = !_lastBlinkState;
      } else {
        _currentBlink = 0;
        _canBlink = false;
      }
    }
  }
}