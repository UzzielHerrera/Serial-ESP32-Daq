#include "SerialExecuter.h"

SerialExecuter::SerialExecuter(){
  valueReset();
  keyValuesQueue.clear();
  confQueue.clear();
  COM.println("SerialExecuter Module initialized");
}

void SerialExecuter::Handle(){
  if(serialReceiver.stringQueue.count() == 0)
    return;
  String stringToDecode = serialReceiver.stringQueue.pop();
  if(stringToDecode.indexOf("CONF") == -1)
    commandDecode(stringToDecode);
  else
    configurationDecode(stringToDecode);
}

void SerialExecuter::configurationDecode(String str){
  String buffer = "";
  for(uint8_t i; i < str.length(); i++){
    if(str[i]==':'){
      confQueue.push(buffer);
      buffer = "";
    } else{
      buffer += str[i];
    }
  }
  confQueue.push(buffer);
  if(confQueue.pop() == "CONF"){
    configurationExecuter(confQueue.pop());
  }
}

void SerialExecuter::configurationExecuter(String configuration){
  if(configuration == "INIT"){
    String action = confQueue.pop();
    switch (action[0])
    {
    case 'N':
      espPreferences.CreateInitPin(confQueue.pop());
      break;
    
    case 'D':
      espPreferences.DeleteInitPin(confQueue.pop().toInt());
      break;

    case 'C':
      espPreferences.ClearInitPin();
      break;
    case 'W':
      espPreferences.OverWriteInitPin();
    }
  }
  
  if (configuration == "ESP"){
    String action = confQueue.pop();
    if(action == "RESET")
      ESP.restart();
    if(action == "SERIAL")
      COM.println("CONF:ESP:SERIAL:OK");
  }
  confQueue.clear();
}

void SerialExecuter::commandDecode(String stringToDecode){
  getKeyValues(stringToDecode);
  KeyValue firstKeyValue;
  int _counter = 0;
  while(keyValuesQueue.count() > 0){
    KeyValue currentKeyValue = keyValuesQueue.pop();
    //COM.println((String) currentKeyValue.Key + ": " + currentKeyValue.Value );
    if(_counter == 0){
      firstKeyValue = currentKeyValue;
      _counter++;
    }
    switch(currentKeyValue.Key){
      case 'E':
        E = currentKeyValue.Value;
        //COM.println((String) "E: " + E );
        break;
      case 'F':
        F = currentKeyValue.Value;
        //COM.println((String) "F: " + F );
        break;
      case 'B':
        B = currentKeyValue.Value;
        //COM.println((String) "B: " + B );
        break;
      case 'W':
        W = currentKeyValue.Value;
        //COM.println((String) "W: " + W );
        break;
      case 'D':
        D = currentKeyValue.Value;
        //COM.println((String) "W: " + W );
        break;
      case 'H':
        H = currentKeyValue.Value;
        //COM.println((String) "H: " + H );
        break;
    }
  }
  commandExecuter(firstKeyValue);
  valueReset();
}

bool SerialExecuter::CheckPinForUse(uint8_t pin){
  if(!pinManager.PinController[pin]->ForUse){
    COM.println("!-Error(NotForUse)");
    return true;
  }
  return false;
}

void SerialExecuter::commandExecuter(KeyValue command){
  switch(command.Key){
    case 'O':
      if(E == NULL_NUMBER){
        COM.println("!-Error(E)");
        break;
      }
      for(uint8_t i = 0; i < MAX_PIN; i++){
        if(pinManager.PinController[i]->arduinoPin == command.Value){
          if(CheckPinForUse(i)) break;
          E = (E < 0) ? 0 : E;
          E = (E > 1) ? 1 : E;
          if(pinManager.PinController[i]->currentMode != OUTPUTS){
            bool buff = pinManager.PinController[i]->defineOutput();
            CheckChange(buff);
          }
          if(E == 1) pinManager.PinController[i]->On();
          else pinManager.PinController[i]->Off();
          COM.println((String) "O" + command.Value + ":" + E );
          break;
        }
      }
      break;
    case 'R':
      if(W == NULL_NUMBER){
        COM.println("!-Error(W)");
        break;
      }
      for(uint8_t i = 0; i< MAX_PIN; i++){
        if(pinManager.PinController[i]->arduinoPin == command.Value){
          if(CheckPinForUse(i)) break;
          W = (W < 0) ? 0 : W;
          W = (W > 2) ? 2 : W;
          pinModes pinCurrentMode = pinManager.PinController[i]->currentMode;
          if((pinCurrentMode != INPUT_RESISTOR && W!=0) || (pinCurrentMode != INPUTS && W == 0) || W != _lastW)
            bool buff = pinManager.PinController[i]->defineInput((inputOptions) W );
          bool buffer = pinManager.PinController[i]->ReadDigital();
          _lastW = W;
          if(!pinManager.PinController[i]->errorFlag){
            COM.println((String) "R" + command.Value + ":" + buffer );
          } else {
            pinManager.PinController[i]->errorFlag = false;
            COM.println("!-Error.");
          }
          break;
        }
      }
      break;
    case 'A':
      for(uint8_t i = 0; i< MAX_PIN; i++){
        if(pinManager.PinController[i]->arduinoPin == command.Value){
          if(CheckPinForUse(i)) break;
          if(pinManager.PinController[i]->currentMode != INPUTS){
            bool buff = pinManager.PinController[i]->defineInput(NO_RESISTOR);
            CheckChange(buff);
          } 
          uint16_t buffer = pinManager.PinController[i]->ReadAnalog();
          COM.println((String) "A" + command.Value + ":" + buffer );
          break;
        }
      }
      break;
    case 'P':
      if(D == NULL_NUMBER){
        COM.println("!-Error(D)");
        break;
      }
      for(uint8_t i = 0; i< MAX_PIN; i++){
        if(pinManager.PinController[i]->arduinoPin == command.Value){
          if(CheckPinForUse(i)) break;
          if(pinManager.PinController[i]->currentMode != PWMS){
            bool buff = pinManager.PinController[i]->definePWM();
            CheckChange(buff);
          }
          if(B != NULL_NUMBER || F != NULL_NUMBER){
            B = (B == NULL_NUMBER) ? DEFAULT_PWM_RESOLUTION : B;
            F = (F == NULL_NUMBER) ? DEFAULT_PWM_FREQUENCY : F;
            pinManager.PinController[i]->setuUpPWM(F, B);
          }
          pinManager.PinController[i]->usePWM(D);
          COM.println((String) "P" + command.Value + ":" + D );
          break;
        }
      }
      break;
    case 'C':
      if(H == NULL_NUMBER){
        COM.println("!-Error(H)");
        break;
      }
      for(uint8_t i = 0; i < MAX_PIN; i++){
        if(pinManager.PinController[i]->arduinoPin == command.Value){
          if(CheckPinForUse(i)) break;
          if(pinManager.PinController[i]->currentMode != DACS){
            bool buff = pinManager.PinController[i]->defineOutput();
            CheckChange(buff);
          }
          pinManager.PinController[i]->useDAC((uint8_t) H);
          COM.println((String) "C" + command.Value + ":" + H );
          break;
        }
      }
      break;
    case 'I':
      if(command.Value == 0){
        for(uint8_t i; i < MAX_PIN; i++){
          if(pinManager.PinController[i]->Input && pinManager.PinController[i]->ForUse)
            pinManager.PinController[i]->defineInput(NO_RESISTOR);
        }
        COM.println("I0");
        break;
      }
      if(command.Value == 1){
        if(E == NULL_NUMBER){
          COM.println("!-Error(E)");
          break;
        }
        for(uint8_t i; i < MAX_PIN; i++){
          if(pinManager.PinController[i]->Output && pinManager.PinController[i]->ForUse){
            pinManager.PinController[i]->defineOutput();
            if(E == 1) pinManager.PinController[i]->On();
            if(E == 0) pinManager.PinController[i]->Off();
          }
        }
        COM.println((String) "I" + command.Value + ":" + E);
        break;
      }
      break;
  }
}

void SerialExecuter::CheckChange(bool inValue){
  if(!inValue){
    COM.println("!-Error(NotAllowedByPin)");
  }
}

void SerialExecuter::getKeyValues(String query){
  String splitWord = "";
  query += " ";
  for(uint8_t i = 0; i < query.length(); i ++){
    if(query[i] == ' '){
      if(splitWord == "")
        continue;
      KeyValue keyValue;
      keyValue.Key = splitWord[0];
      keyValue.Value = splitWord.substring(1).toInt();
      keyValuesQueue.push(keyValue);
      splitWord = "";
      continue;
    }
    splitWord += String(query[i]);
  }
}

void SerialExecuter::valueReset(){
  E = NULL_NUMBER;
  F = NULL_NUMBER;
  B = NULL_NUMBER;
  W = NULL_NUMBER;
  D = NULL_NUMBER;
  H = NULL_NUMBER;
}
