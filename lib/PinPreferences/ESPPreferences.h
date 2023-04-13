#ifndef PinPreferences_h
#define PinPreferences_h

#include "Constants.h"
#include "SPIFFS.h"
#include "Arduino.h"
#include "Queue.h"

enum pinPreff{
    NOT_MODE_DEFINED,
    OUTPUT_ON,
    OUTPUT_OFF,
    INPUT_ONLY,
    INPUT_WITH_PULLUP,
    INPUT_WITH_PULLDOWN,
    INPUT_ADC,
    OUTPUT_DAC,
    OUTPUT_PWM
};

class PinInitConfig{
    public:
        uint8_t pin = 0;
        String command = "";
};

class ESPPreferences{
    public:
        ESPPreferences();
        void ClearInitPin();
        void CreateInitPin(String initCommand);
        void DeleteInitPin(int pin2Delete);
        void WriteNewInitPinConfig();
        void OverWriteInitPin();
        PinInitConfig PinInits[MAX_PIN];
        void StringDecode();
        Queue<String> serialQueue = Queue<String>(MAX_PIN + 1);

    private:
        void attachInit(PinInitConfig *initConfig);
        void getKeyValues(String query);
        String outString[MAX_PIN];
};

extern ESPPreferences espPreferences;

#endif