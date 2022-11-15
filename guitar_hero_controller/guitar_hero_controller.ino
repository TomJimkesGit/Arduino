#include <Arduino.h>
#include <BleGamepad.h>

BleGamepad bleGamepad;

// GREEN = 18;  
// RED = 19; 
// YELLOW = 21; 
// BLUE = 2;
// ORANGE = 4;

int buttonCount = 9;
String buttons[9] = {"BLUE", "GREEN", "RED", "YELLOW", "ORANGE", "STRUM_DOWN", "STRUM_UP", "STAR_POWER", "PLUS"};
int buttonPinMapping[9] = {18, 19, 5, 17, 16, 13, 14, 27, 26};
int buttonStates[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int buttonControllerMapping[9] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9};

int WAMMY_BAR = 33;


void setup() {

  //Set all buttons to pulldown
  for (int i = 0; i < buttonCount; i++){
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

  //The input of the strum buttons is inversed
  if (buttons[index] == "STRUM_UP" || buttons[index] == "STRUM_DOWN"){
    input = (input == 0) ? 1 : 0;
  }
  
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
