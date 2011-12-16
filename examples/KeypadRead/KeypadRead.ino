#include <KeypadSimple.h>

const byte KEYS = 12; //number of keys

char keys[KEYS] = {
  '1','2','3','4',
  '5','6','7','8',
  '9','0','*','#'
};
byte keyPins[KEYS] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 30, 40, 41}; //connect to the row pinouts of the keypad

KeypadSimple keypad = KeypadSimple( makeKeymap(keys), keyPins, KEYS);

void setup(){
  Serial.begin(9600);
  keypad.setDebounceTime(500);
}

void loop(){
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
  }
}
