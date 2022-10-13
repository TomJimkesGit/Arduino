#include <Arduino.h>
#include <BleGamepad.h>

BleGamepad bleGamepad;

// GREEN = 18;  
// RED = 19; 
// YELLOW = 21; 
// BLUE = 2;
// ORANGE = 4;

int buttonCount = 5;
String buttons[5] = {"GREEN", "RED", "YELLOW", "BLUE", "ORANGE"};
int buttonPinMapping[5] = {18, 19, 21, 2, 4};
int buttonStates[5] = {0, 0, 0, 0, 0};
int buttonControllerMapping[5] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5};

int WAMMY_BAR = 35;


void setup() {

  //Set all buttons to pulldown
  for (int i = 0; i < 5; i++){
    pinMode(buttonPinMapping[i], INPUT_PULLDOWN);
  }
  
  pinMode(WAMMY_BAR, INPUT);
  
  Serial.begin(115200);

//  BleGamepadConfiguration bleGamepadConfig;
//  bleGamepadConfig.setAutoReport(false); // This is true by default
//  bleGamepadConfig.setButtonCount(8);
//  bleGamepadConfig.setHatSwitchCount(0);
  bleGamepad.begin();
}

void loop() {
  if(bleGamepad.isConnected()) {

    //Handle Buttons
    for (int i = 0; i < buttonCount; i++){
      handleButton(i);
    }

    //Handle Whammy
    handleWhammy();
    
  }
}

void handleButton(int index){
  int input = digitalRead(buttonPinMapping[index]);
  if (buttonStates[index] > input){ //Stored state is pressed, read state is released
    bleGamepad.release(buttonControllerMapping[index]);
  } else if (buttonStates[index] < input) { //Stored state is released, read state is pressed
    bleGamepad.press(buttonControllerMapping[index]);
  } else {
    return;
  }
  buttonStates[index] = input;
}

void handleWhammy(){
  //The max read value is around 1700
  int wIn = analogRead(WAMMY_BAR);
  int wComp = (wIn > 1700) ? 1700 : wIn;
  

  //If the input is max, the joystick should be at 0
  //As the whammy is pressed down, the input should increase 1
  //In order to catch rounding errors etc. we clip to 0 at 5%
  int wInv = 1700 - wComp; // Invert
  
  //Normalize to 32767, which is the 16 bit thumb input
  int normalized = 32767 / 1700 * wInv;
  int halfway = 32767 / 2;
   //We only want the upper register of the joystick.
  normalized = (normalized < 2000) ? halfway : halfway + normalized / 2; //1650 is around 5% of 32767
  
  bleGamepad.setLeftThumb(normalized, normalized);
}
