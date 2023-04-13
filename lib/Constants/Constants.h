#ifndef Constants_h
#define Constants_h

#define DEVICE_NAME "PX-SERIES"
#define MYSSID "WLJUAR0218"
#define MYPASSWORD "123456789"

#define COM Serial
#define BAUDRATE 115200

#define MAX_STRINGS 50
#define MAX_PWM_CHANNELS 16
#define MAX_PIN 30
#define MAX_INDEXES 15
#define MAX_SETTINGS 15

#define DEFAULT_PWM_RESOLUTION 8
#define DEFAULT_PWM_FREQUENCY 1000

// --- PIN DEFINITION
#define STATE_LED 15

// --- BLINK DEFAULT CONFIGURATION
#define BLINK_CYCLE_TIME 1500
#define BLINK_LAPSE 250
#define BLINKS_PER_CYCLE 3

#define NULL_NUMBER 98765

class KeyValue{
  public:
    char Key;
    int Value;
};

#endif