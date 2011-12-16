/*
||
|| @file KeypadSimple.cpp
|| @version 1.8-r1
|| @author Mark Stanley, Alexander Brevig, Derek Chafin
|| @contact mstanley@technologist.com, alexanderbrevig@gmail.com, infomaniac50@gmail.com
||
|| @description
|| | This library provides a simple interface for using non-matrix
|| | keypads. It supports the use of multiple keypads with the
|| | same or different sets of keys.  It also supports user
|| | selectable pins and definable keymaps.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#include "KeypadSimple.h"

// <<constructor>> Allows custom keymap. pin configuration and keypad size
KeypadSimple::KeypadSimple(char *userKeymap, byte *keyPins, byte keys)
{
  Pins = keyPins;

	size.keys = keys;

  begin(userKeymap);

	lastUpdate = 0;
	debounceTime = 50;
	holdTime = 1000;
	keypadEventListener = 0;
	currentKey = NO_KEY;
	state = IDLE;

	initializePins();
}

// Let the user define a keymap - assume the same row- / columncount as defined in constructor
void KeypadSimple::begin(char *userKeymap){
    keymap = userKeymap;
}

// Returns the keykode of the pressed key, or NO_KEY if no key is pressed
char KeypadSimple::getKey(){
	char key = NO_KEY; // Assume that no key is pressed, this is the default return for getKey()
  for (byte k=0; k<size.keys; k++){	// Scan all the rows for a key press.
    //  The user pressed a button for more then debounceTime microseconds.
    if (currentKey == keymap[k]){
      // Button hold
      if (((millis()-lastUpdate) >= holdTime) && digitalRead(Pins[k]) == LOW){
        transitionTo(HOLD);
      }
      // Button release
      if (((millis()-lastUpdate) >= debounceTime) && digitalRead(Pins[k]) == HIGH){
        transitionTo(RELEASED);
        currentKey = NO_KEY;
      }
    }
    // Button pressed event.  The user pressed a button.
    else if (((millis()-lastUpdate) >= debounceTime) && digitalRead(Pins[k]) == LOW){
      key = keymap[k];
      lastUpdate = millis();
      goto EVALUATE_KEY; 			// Save resources and do not attempt to parse two keys at a time
    }
  }

	EVALUATE_KEY:
	if (key != NO_KEY && key != currentKey){
		currentKey = key;
		transitionTo(PRESSED);
		return currentKey;
	}
	else{
		return NO_KEY;
	}
}


KeypadState KeypadSimple::getState(){
	return state;
}

void KeypadSimple::setDebounceTime(unsigned int debounce){
	debounceTime = debounce;
}
void KeypadSimple::setHoldTime(unsigned int hold){
	holdTime = hold;
}

void KeypadSimple::addEventListener(void (*listener)(char)){
	keypadEventListener = listener;
}

//private
void KeypadSimple::transitionTo(KeypadState newState){
	if (state!=newState){
		state = newState;
		if (keypadEventListener!=NULL){
			keypadEventListener(currentKey);
		}
	}
}

void KeypadSimple::initializePins(){
	for (byte k=0; k<size.keys; k++){
 		//configure row pin modes and states
		pinMode(Pins[k],INPUT);
		digitalWrite(Pins[k],HIGH);
  }
}

/*
|| @changelog
|| | 2011-12-15 - Derek Chafin  : Modified library to work with non-matrix keypads.
|| | 2011-11-29 - Tom Putzeys : Use internal pull-up, no  more collum diodes needed, and consumes less power
|| | 2009-07-08 - Alexander Brevig : Library does not use 2d arrays
|| | 2009-06-15 - Alexander Brevig : Added transitionTo
|| | 2009-06-15 - Alexander Brevig : Added getState()
|| | 2009-06-13 - Mark Stanley : Fixed bug in getKey() that returns the wrong key if debounceTime is too short.
|| | 2009-06-13 - Mark Stanley : Minor bug fix:  Added 'currentKey = NO_KEY' to constructors.
|| | 2009-05-19 - Alexander Brevig : Added setHoldTime()
|| | 2009-05-15 - Alexander Brevig : Changed begin() amd getKey(), this Library should be operational.
|| | 2009-05-09 - Alexander Brevig : Changed getKey()
|| | 2009-04-28 - Alexander Brevig : Modified API, and made variables private
|| | 2007-XX-XX - Mark Stanley : Initial Release
|| #
*/
