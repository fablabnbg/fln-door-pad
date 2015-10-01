/* 

FLN-Door-Pad
by: mose@fabfolk.com
under: MIT License

tested with Arduino 1.6.4

Inspired and partially based on
  keyPadHiduino Example Code
  by: Jim Lindblom
  MIT license

This program was designed for use with an Arduino Pro Micro.
It provides an HID or serial interface for "Storm Interface" 1000 series keypads
with 8-pin connector. Both 12 and 16 key types are supported. Connect D2-D9 to
Pin 1-8 on keypad. Interface mode selectable with jumper. By pulling D0 (RXI
on Pro Micro) LOW it's in HID mode or w/o jumper it's a 9600 8N1 serial interface.
   
*/

const long debounceDelay = 50;
const int rowPins[4] = {2,3,9,8};
const int columnPins[4] = {4,5,6,7};

int keypadStatus;  // Used to monitor which buttons are pressed.
int timeout;  // timeout variable used in loop
int lastkeypadStatus = 0;
long lastDebounceTime = 0;
boolean serial;


void setup(){
  for (int i=2; i<=9; i++){
    pinMode(i,INPUT);  // Set all keypad pins as inputs
    digitalWrite(i,HIGH);  // pullup
  }
  
  pinMode(0,INPUT_PULLUP);
  if(digitalRead(0) == LOW){
    Keyboard.begin();
    serial = false;
    const char layout[16] = {'1','2','3','A','4','5','6','B','7','8','9','C',8,'0','\n','D'}; //layout with * as BACKSPACE and # as RETURN
  }
  else {
    Serial.begin(9600);
    serial = true;
    const char layout[16] = {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
  }
}

void loop(){
  keypadStatus = getKeypadStatus();  // read which buttons are pressed
  if (keypadStatus != 0)  // If a button is pressed go into here
  {
    // If the switch changed, due to noise or pressing:
    if (keypadStatus != lastkeypadStatus) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      sendKeyPress(keypadStatus);  // send the button over USB
    
    // repeat
      timeout = 2000;  // top of the repeat delay
      while ((getKeypadStatus() == keypadStatus) && (--timeout))  // Decrement timeout and check if key is being held down
        delayMicroseconds(1);
      while (getKeypadStatus() == keypadStatus){  // while the same button is held down 
        sendKeyPress(keypadStatus);  // continue to send the button over USB
        delay(50);  // 50ms repeat rate
      }
    }
  }
  lastkeypadStatus = keypadStatus;
}

/* sendKeyPress(int key): This function sends a single key over USB
   It requires an int, of which the 12 LSbs are used. Each bit in
   key represents a single button on the keypad.
   This function will only send a key press if a single button
   is being pressed */
void sendKeyPress(int key){
  if(serial)
    Serial.write(layout[key-1]);
  else
    Keyboard.write(layout[key-1]);
}

/* getKeypadStatus(): This function returns an int that represents
the status of the 12-button keypad. Only the 12 LSb's of the return
value hold any significange. Each bit represents the status of a single
key on the button pad. '1' is bit 0, '2' is bit 1, '3' is bit 2, ..., 
'#' is bit 11.

This function doesn't work for multitouch.
*/
int getKeypadStatus(){
  int keypadStatus = 0;  // this will be what's returned
  
  /* initialize all pins, inputs w/ pull-ups */
  for (int i=2; i<=9; i++){
    pinMode(i, INPUT_PULLUP);
  }
  
  for (int row=0; row<4; row++){  // initial for loop to check all 4 rows
    pinMode(rowPins[row], OUTPUT);  // set the row pin as an output
    digitalWrite(rowPins[row], LOW);  // pull the row pins low
    for (int col=0; col<4; col++){  // embedded for loop to check all 3 columns of each row
      if (!digitalRead(columnPins[col]))
      {
        keypadStatus = (row+1)*4 + (col+1) - 4;  // set the status bit of the keypad return value
      }
    }
    pinMode(rowPins[row], INPUT_PULLUP);  // reset the row pin as an input
  }
  
  return keypadStatus;
}

