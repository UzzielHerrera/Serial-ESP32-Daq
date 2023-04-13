#include <Arduino.h>
#include "Constants.h"
#include "State.h"
#include "SerialReceiver.h"
#include "SerialExecuter.h"
#include "Pin.h"
#include "PinManager.h"
#include "Data.h"
#include "ESPPreferences.H"

Data commonData;
ESPPreferences espPreferences;
PinManager pinManager;
SerialReceiver serialReceiver;
SerialExecuter serialExecuter;
State state;

// Pin Definition for ESP32
Pin V3v3(1, 40, false, false, false, false, false, false);
Pin GND(2, 41, false, false, false, false, false, false);
Pin D15(3, 15, true, true, true, true, false, false);
Pin D2(4, 2, true, true, true, true, false, true);
Pin D4(5, 4, true, true, true, true, false, true);
Pin RXD2(6, 16, true, false, true, false, false, false);
Pin TXD2(7, 17, false, true, true, false, false, false);
Pin D5(8, 5, true, true, true, false, false, true);
Pin D18(9, 18, true, true, true, false, false, true);
Pin D19(10, 19, true, true, true, false, false, true);
Pin D21(11, 21, true, true, true, false, false, true);
Pin RXD0(12, 3, true, false, true, false, false, false);
Pin TXD0(13, 1, false, true, true, false, false, false);
Pin D22(14, 22, true, true, true, false, false, true);
Pin D23(15, 23, true, true, true, false, false, true);
Pin EN(16, 44, false, false, false, false, false, false);
Pin D36(17, 36, true, false, false, true, false, true);
Pin D39(18, 39, true, false, false, true, false, true);
Pin D34(19, 34, true, false, false, true, false, true);
Pin D35(20, 35, true, false, false, true, false, true);
Pin D32(21, 32, true, true, true, true, false, true);
Pin D33(22, 33, true, true, true, true, false, true);
Pin D25(23, 25, true, true, true, true, true, true);
Pin D26(24, 26, true, true, true, true, true, true);
Pin D27(25, 27, true, true, true, true, false, true);
Pin D14(26, 14, true, true, true, true, false, true);
Pin D12(27, 12, true, true, true, true, false, true);
Pin D13(28, 13, true, true, true, true, false, true);
Pin GND2(29, 41, false, false, false, false, false, false);
Pin VIN(30, 43, false, false, false, false, false, false);

void setup() {
  pinManager.attach(&V3v3);
  pinManager.attach(&GND);
  pinManager.attach(&D15);
  pinManager.attach(&D2);
  pinManager.attach(&D4);
  pinManager.attach(&RXD2);
  pinManager.attach(&TXD2);
  pinManager.attach(&D5);
  pinManager.attach(&D18);
  pinManager.attach(&D19);
  pinManager.attach(&D21);
  pinManager.attach(&RXD0);
  pinManager.attach(&TXD0);
  pinManager.attach(&D22);
  pinManager.attach(&D23);
  pinManager.attach(&EN);
  pinManager.attach(&D36);
  pinManager.attach(&D39);
  pinManager.attach(&D34);
  pinManager.attach(&D35);
  pinManager.attach(&D32);
  pinManager.attach(&D33);
  pinManager.attach(&D25);
  pinManager.attach(&D26);
  pinManager.attach(&D27);
  pinManager.attach(&D14);
  pinManager.attach(&D12);
  pinManager.attach(&D13);
  pinManager.attach(&GND2);
  pinManager.attach(&VIN);
  COM.println("PX SERIES VOLTAGE TEST");
}

void loop() {
  state.blinkHandle();
  serialReceiver.Handle();
  serialExecuter.Handle();
}