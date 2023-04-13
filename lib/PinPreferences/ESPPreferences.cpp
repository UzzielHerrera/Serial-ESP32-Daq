#include "ESPPreferences.h"

ESPPreferences::ESPPreferences(){

    COM.begin(BAUDRATE);

    if(!SPIFFS.begin(true)) return;
    for(uint8_t i; i < MAX_PIN; i++){
        PinInits[i].pin = 0;
        PinInits[i].command = "";
        outString[i] = "";
    }
    COM.println("ESPPreferences Module initialized");
    serialQueue.push("I1 E0");
    StringDecode();
}

void ESPPreferences::StringDecode(){
    if(SPIFFS.exists("/PinInitConfig.txt")){
        File initPinConFigFile = SPIFFS.open("/PinInitConfig.txt", FILE_READ);
        if(!initPinConFigFile) return;
        String inString = initPinConFigFile.readString();
        initPinConFigFile.close();
        String intermediateString;
        for(uint32_t j = 0; j < inString.length(); j++){
            if(inString[j] != '\r' && inString[j] != '\n'){
                intermediateString += inString[j];
            }
            if(inString[j]=='\n'){
                //COM.println((String) j + ": " + intermediateString);
                getKeyValues(intermediateString);
                serialQueue.push(intermediateString);
                intermediateString = "";
            }
        }
    }
}

void ESPPreferences::CreateInitPin(String initCommand){
    //COM.println( "InitCommand: " + initCommand);
    getKeyValues(initCommand);
    WriteNewInitPinConfig();
    COM.println((String) "CONF:INIT:NEW:ADD:" + initCommand);
}

void ESPPreferences::DeleteInitPin(int pin2Delete){
    for(uint8_t i = 0; i < MAX_PIN; i++){
        if(PinInits[i].pin != 0){
            if(PinInits[i].pin == pin2Delete){
                PinInits[i].pin = 0;
                PinInits[i].command = "";
                COM.println("CONF:INIT:DEL:" + String(pin2Delete));
                break;
            }
        }
    }
}

void ESPPreferences::OverWriteInitPin(){
    WriteNewInitPinConfig();
    COM.println("CONF:INIT:WRITEN");
}

void ESPPreferences::ClearInitPin(){
    if(!SPIFFS.remove("/PinInitConfig.txt")) return;
    for(uint8_t i = 0; i < MAX_PIN; i++){
        PinInits[i].pin = 0;
        PinInits[i].command = "";
    }
    File clearfile = SPIFFS.open("/PinInitConfig.txt", FILE_WRITE, true);
    clearfile.close();
    COM.println("CONF:INIT:CLEAR:DONE");
}

void ESPPreferences::WriteNewInitPinConfig(){
    if(!SPIFFS.remove("/PinInitConfig.txt")) return;
    String bufferString = "";
    for(uint8_t i = 0; i < MAX_PIN; i++){
        if(PinInits[i].pin != 0){
            bufferString += PinInits[i].command + "\r\n";
        }
    }
    File bufferFileToWrite = SPIFFS.open("/PinInitConfig.txt", FILE_WRITE, true);
    bufferFileToWrite.print(bufferString);
    bufferFileToWrite.close();
}

void ESPPreferences::getKeyValues(String query){
    String splitWord = "";
    String fullString = query;
    query += " ";
    KeyValue keyValue;
    for(uint8_t i = 0; i < query.length(); i ++){
        if(query[i] == ' '){
            if(splitWord == "") continue;
            keyValue.Key = splitWord[0];
            keyValue.Value = splitWord.substring(1).toInt();
            splitWord = "";
            break;
            continue;
        }
        splitWord += String(query[i]);
    }
    PinInitConfig bufferInit;
    bufferInit.pin = keyValue.Value;
    bufferInit.command = fullString;
    DeleteInitPin(keyValue.Value);
    attachInit(&bufferInit);
}

void ESPPreferences::attachInit(PinInitConfig *initConfig){
    for(uint8_t i = 0; i < MAX_PIN; i++){
        if(PinInits[i].pin == 0){
            PinInits[i].pin = initConfig->pin;
            PinInits[i].command = initConfig->command;
            break;
        }
    }
}