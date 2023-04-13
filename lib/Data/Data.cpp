#include <Data.h>

Data::Data(){
  Baudrate = 115200;
  for(uint8_t i = 0; i < MAX_PWM_CHANNELS; i++){
    pwmChannels[i].inUse = false;
    pwmChannels[i].pinAttached = 40;
  }
}