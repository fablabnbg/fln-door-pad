/*
FLN-Door-Pad
by: mose@fabfolk.com
under: MIT License
tested with Arduino 1.86
Inspired and partially based on
  keyPadHiduino Example Code
  by: Jim Lindblom
  MIT license
This program was designed for use with an Arduino Pro Micro.
It provides an HID or serial interface for "Storm Interface" 1000 series keypads
with 8-pin connector. Both 12 and 16 key types are supported. Connect D2-D9 to
Pin 1-8 on keypad. Interface mode selectable with definition "KEYPAD_12_KEY" or
"KEYPAD_16_KEY".

*/

#include "Arduino.h"



#define INTERFACE_SERIAL              1
#define INTERFACE_KEYBOARD            2
/** @brief switch for SERIAL or KEYBOARD interface */
#define INTERFACE           INTERFACE_SERIAL

#define KEYPAD_12_KEY       12
#define KEYPAD_16_KEY       16
/** @brief switch for Keypad with 12 Keys oder 16 keys */
#define KEYPAD_TYP        KEYPAD_12_KEY



/** @brief Time for Key Debouncing */
#define DEBOUNCING_TIME_MS  50

#if KEYPAD_TYP == KEYPAD_12_KEY
    #define ROW_SIZE            4
    #define COLUMN_SIZE         3
    const int rowPins[ROW_SIZE]    = {9,8,2,3};
    const int columnPins[COLUMN_SIZE] = {7,6,5};

    /** @brief Number of Keys */
    #define BUTTON_COUNT        (ROW_SIZE * COLUMN_SIZE)

    /** @brief keys on keyboard */
    char layout[BUTTON_COUNT] = {'1','2','3','4','5','6','7','8','9','*','0','#'};

#elif KEYPAD_TYP == KEYPAD_16_KEY
    #define ROW_SIZE            4
    #define COLUMN_SIZE         4
    const int rowPins[ROW_SIZE]    = {9,8,2,3};
    const int columnPins[COLUMN_SIZE] = {7,6,5,4};

    /** @brief Number of Keys */
    #define BUTTON_COUNT        (ROW_SIZE * COLUMN_SIZE)

    /** @brief keys on keyboard */
    char layout[BUTTON_COUNT] = {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};



#endif


int getKeypadStatus(void);
void sendKeyPress(int key);



unsigned int keypadStatusPressedOld;

void setup()
{
    for (int i=2; i<=9; i++)
    {
        pinMode(i,INPUT);                                       // Set all keypad pins as inputs
        digitalWrite(i,HIGH);                                   // pullup
    }


#if INTERFACE == INTERFACE_KEYBOARD
    Keyboard.begin();
    // use * as BACKSPACE and # as RETURN
    layout[12] = 8;
    layout[14] = '\n';

#elif INTERFACE == INTERFACE_SERIAL

    Serial.begin(9600);

#endif

}



void loop()
{
    unsigned int keypadStatusNew = 0;
    unsigned int keypadStatusOld = 0;
    unsigned int keypadStatusPressed = 0;

    keypadStatusOld = getKeypadStatus();                                // read which buttons are pressed

    if (keypadStatusOld != 0)                                           // If a button is pressed go into here
    {
        delay(DEBOUNCING_TIME_MS);                                      // Wait until bouncing is over

        keypadStatusNew = getKeypadStatus();                            // Read actual value

        keypadStatusPressed = keypadStatusOld & keypadStatusNew;        // Is Key still pressed

        if(keypadStatusPressed != 0)
        {
            keypadStatusNew &= ~keypadStatusPressedOld;                 // Key is only set if its not hold for one loop
            keypadStatusPressedOld = keypadStatusPressed;

            for(int index = 0; index < BUTTON_COUNT; index++)
            {
                if(keypadStatusNew & (1 << index))
                {
                    sendKeyPress(index);                                // Send Key
                }
            }
        }
        else
        {
            keypadStatusPressedOld = 0;
        }
    }
}




/**
 *  @brief Send Keypad button pressed
 *
 *    sendKeyPress(int key): This function sends a single key over USB
 *    It requires an int, of which the 12 LSbs are used. Each bit in
 *    key represents a single button on the keypad.
 *    This function will only send a key press if a single button
 *    is being pressed.
 *
 *    @param    pressd keypad number
 *    @return   -
 */
void sendKeyPress(int key)
{
#if INTERFACE == INTERFACE_SERIAL
    Serial.write(layout[key]);
#elif INTERFACE == INTERFACE_KEYBOARD
    Keyboard.write(layout[key]);
#endif
}



/**
 *  @brief Read Keypad
 *
 *    This function returns an int that represents
 *    the status of the 12-button keypad. Only the 12 LSb's of the return
 *    value hold any significange. Each bit represents the status of a single
 *    key on the button pad. '1' is bit 0, '2' is bit 1, '3' is bit 2, ...,
 *    '#' is bit 11.
 *    This function doesn't work for multitouch.
 *
 *
 *  @param    -
 *  @return   pressed Button
 */
int getKeypadStatus(void)
{
    int keypadStatus = 0;                                           // this will be what's returned


    for (int i=2; i<=9; i++)
    {
        pinMode(i, INPUT_PULLUP);                                   // initialize all pins, inputs w/ pull-ups
    }

    for (int row=0; row<ROW_SIZE; row++)                            // initial for loop to check all 4 rows
    {
        pinMode(rowPins[row], OUTPUT);                              // set the row pin as an output
        digitalWrite(rowPins[row], LOW);                            // pull the row pins low

        for (int col=0; col<COLUMN_SIZE; col++)                     // embedded for loop to check all 3 columns of each row
        {
            if (!digitalRead(columnPins[col]))
            {
                keypadStatus |= 1 << ((row * COLUMN_SIZE) + col);   // set the status bit of the keypad return value
                return keypadStatus;
            }
        }
        pinMode(rowPins[row], INPUT_PULLUP);                        // reset the row pin as an input
    }

    return 0;
}
