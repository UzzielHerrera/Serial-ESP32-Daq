#ifndef Pin_h
#define Pin_h

#include <Constants.h>
#include <Arduino.h>
#include <Data.h>

enum inputOptions{
  NO_RESISTOR,
  PULL_UP,
  PULL_DOWN,
};

enum pinModes{
  NOT_DEFINED,
  OUTPUTS,
  INPUTS,
  INPUT_RESISTOR,
  PWMS,
  ADCS,
  DACS,
};

class Pin{
  public:
    Pin(uint8_t pinNumber, uint8_t arduinoNumber, bool isInput, bool isOutput, bool isPWM, bool isADC, bool isDAC, bool isForUse);
    bool defineOutput();
    bool defineInput(inputOptions mode);
    bool ReadDigital();
    uint16_t ReadAnalog();
    bool useDAC(int value);
    bool definePWM();
    bool setuUpPWM(double frequency, uint8_t resolution);
    bool detachPWM();
    void usePWM(uint16_t dutyCycle);
    bool useDAC(uint8_t dacValue);
    void disableDAC();
    bool On();
    bool Off();
    bool Output;
    bool Input;
    bool PWM;
    bool ADC;
    bool DAC;
    bool ForUse;
    bool PullUp;
    bool PullDown;
    pinModes currentMode;
    bool errorFlag;
    uint8_t physicalPin;
    uint8_t arduinoPin;

  private:
    uint8_t currentPWMChannel; //0-(MAX_PWM_CHANNELS-1) IN USE, ==MAX_PWM_CHANNELS NOT IN USE;
    bool haveResistor;
};

#endif