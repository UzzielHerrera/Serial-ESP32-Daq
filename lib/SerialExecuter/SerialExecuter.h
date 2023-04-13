#ifndef SerialExecuter_h
#define SerialExecuter_h

#include "Constants.h"
#include "Arduino.h"
#include "Queue.h"
#include "SerialReceiver.h"
#include "Pin.h"
#include "PinManager.h"
#include "ESPPreferences.h"

class SerialExecuter{
  public:
    SerialExecuter();
    void Handle();
    int E, F, B, W, D, H;
  private:
    void configurationExecuter(String configuration);
    void commandDecode(String stringToDecode);
    void configurationDecode(String str);
    bool CheckPinForUse(uint8_t pin);
    void CheckChange(bool inValue);
    int _lastW;
    void getKeyValues(String query);
    
    Queue<KeyValue> keyValuesQueue = Queue<KeyValue>(10);
    Queue<String> confQueue = Queue<String>(10);
    void commandExecuter(KeyValue command);
    void valueReset();
};

extern SerialExecuter serialExecuter;

#endif