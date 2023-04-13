#include <PinManager.h>

PinManager::PinManager(){
  
  for(uint8_t i = 0; i < MAX_PIN; i++){
    PinController[i] = {0};
  }

  COM.println("PinManager Module initialized");
}

void PinManager::attach(Pin *pinx){
  if(pinx->physicalPin - 1  < MAX_PIN){
    PinController[pinx->physicalPin - 1] = pinx;
    //COM.println((String) "! - New Pin Attached: " + pinx->arduinoPin);
    attachedPins++;
    //COM.println((String) "Total: " + attachedPins);
  }
}