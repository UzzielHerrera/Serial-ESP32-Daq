#include <Pin.h>

Pin::Pin(uint8_t pinNumber, uint8_t arduinoNumber, bool isInput, bool isOutput, bool isPWM, bool isADC, bool isDAC, bool isForUse){
  arduinoPin = arduinoNumber;
  physicalPin = pinNumber;
  PWM = isPWM;
  Output = isOutput;
  Input = isInput;
  ADC = isADC;
  DAC = isDAC;
  ForUse = isForUse;
  currentMode = NOT_DEFINED;
  errorFlag = false;
  currentPWMChannel = MAX_PWM_CHANNELS;
  if(Output && Input) haveResistor = true;
  else haveResistor = false;
}

bool Pin::defineOutput(){
  if(currentMode == PWMS) detachPWM();
  if(currentMode == DACS) disableDAC();
  if(Output || DAC){
    pinMode(arduinoPin, OUTPUT);
    currentMode = OUTPUTS;
    return true;
  } else {
    return false;
  }
}

bool Pin::On(){
  if(currentMode == OUTPUTS){
    digitalWrite(arduinoPin, 1);
    return true;
  } else {
    return false;
  }
}

bool Pin::Off(){
  if(currentMode == OUTPUTS){
    digitalWrite(arduinoPin, 0);
    return true;
  } else {
    return false;
  }
}

bool Pin::useDAC(uint8_t dacValue){
  if(DAC){
    dacWrite(arduinoPin, dacValue);
    currentMode = DACS;
    return true;
  }
  return false;
}

void Pin::disableDAC(){
  if(DAC){
    dacDisable(arduinoPin);
    pinMode(arduinoPin, OUTPUT);
    digitalWrite(arduinoPin, 0);
  }
}

bool Pin::defineInput(inputOptions mode){
  if(currentMode == DACS) disableDAC();
  if(currentMode == PWMS) detachPWM();
  if(currentMode == OUTPUTS) Off();
  if(Input){
    switch(mode){
      case NO_RESISTOR:
        pinMode(arduinoPin, INPUT);
        currentMode = INPUTS;
        break;
      case PULL_UP:
        if(haveResistor){
          pinMode(arduinoPin, INPUT_PULLUP);
          currentMode = INPUT_RESISTOR;
        } else { return false;}
        break;
      case PULL_DOWN:
        if(haveResistor){
          pinMode(arduinoPin, INPUT_PULLDOWN);
          currentMode = INPUT_RESISTOR;
        } else{ return false;}
        break;
      default:
        return false;
        break;
    }
    return true;
  } else {
    return false;
  }
}

bool Pin::ReadDigital(){
  if(currentMode == INPUTS || currentMode == INPUT_RESISTOR){
    bool buffer = digitalRead(arduinoPin);
    return buffer;
  } else {
    errorFlag = true;
    return false;
  }
}

uint16_t  Pin::ReadAnalog(){
  if(currentMode != INPUTS)
    pinMode(arduinoPin, INPUTS);
  currentMode = INPUTS;
  uint16_t buffer = analogRead(arduinoPin);
  return buffer;
}

bool Pin::definePWM(){
  if(currentMode == DACS) disableDAC();
  if(currentMode == OUTPUTS) Off();
  for(uint8_t i = 0; i < MAX_PWM_CHANNELS; i++){
    if(commonData.pwmChannels[i].inUse == false){
      ledcSetup(i, DEFAULT_PWM_FREQUENCY, DEFAULT_PWM_RESOLUTION);
      if(currentMode != OUTPUTS) pinMode(arduinoPin, OUTPUT);
      ledcAttachPin(arduinoPin, i);
      commonData.pwmChannels[i].inUse = true;
      currentPWMChannel = i;
      currentMode = PWMS;
      return true;
    }
  }
  return false;
}

bool Pin::setuUpPWM(double frequency, uint8_t resolution){
  if(currentMode == PWM && currentPWMChannel != MAX_PWM_CHANNELS){
    ledcSetup(currentPWMChannel, frequency, resolution);
    return true;
  }
  return false;
}

bool Pin::detachPWM(){
  if(currentPWMChannel != MAX_PWM_CHANNELS){
    ledcWrite(currentPWMChannel, 0);
    ledcDetachPin(arduinoPin);
    commonData.pwmChannels[currentPWMChannel].inUse = false;
    currentPWMChannel = MAX_PWM_CHANNELS;
    return true;
  }
  return false;
}

void Pin::usePWM(uint16_t dutyCycle){
  COM.println(currentPWMChannel);
  if(currentPWMChannel != MAX_PWM_CHANNELS){
    ledcWrite(currentPWMChannel, (uint32_t) dutyCycle);
  }
}
