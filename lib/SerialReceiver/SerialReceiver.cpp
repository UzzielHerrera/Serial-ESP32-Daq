#include "SerialReceiver.h"

SerialReceiver::SerialReceiver(){

  stringQueue.clear();
  _receiveString.reserve(100);
  _stringBackUp.reserve(100);
  _isStringComplete = false;
  _stringBackUp = "";
  _receiveString = "";

  while (espPreferences.serialQueue.count() > 0) {
    stringQueue.push(espPreferences.serialQueue.pop());
  }
  
  COM.println("SerialReceiver Module initiailized.");
}

void SerialReceiver::Handle(){
  while(COM.available()>0){
    char inChar = (char) COM.read();
    if(inChar == '\n'){
      _isStringComplete = true;
      break;
    }
    if(inChar != '\r'){
        _receiveString += inChar;
    }
  }
  if(!_isStringComplete){
    if(_receiveString.length() > 70)
      _receiveString = "";
  } else {
    stringQueue.push(_receiveString);
    _receiveString = "";
    _isStringComplete = false;
    return;
  }
}