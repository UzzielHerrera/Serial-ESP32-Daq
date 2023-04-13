#ifndef SerialReceiver_h
#define SerialReceiver_h

#include "Constants.h"
#include "Arduino.h"
#include "ESPPreferences.h"
#include "Queue.h"

class SerialReceiver{
  public:
    SerialReceiver();
    void Handle();
    Queue<String> stringQueue = Queue<String>(MAX_STRINGS);
  private:
    String _receiveString;
    String _stringBackUp;
    bool _isStringComplete;
};

extern SerialReceiver serialReceiver;

#endif