FLN-Door-Pad
============
by: mose@fabfolk.com

#### License
MIT License

#### Tested with
* Arduino 1.8.4
* Storm Interface 1000 Series
* Serial Interface

#### Inspired and partially based on

keyPadHiduino Example Code <br>
by: Jim Lindblom <br>
MIT license

Description
-----------
This program was designed for use with an Arduino Pro Micro.
It provides an HID or serial interface for "Storm Interface" 1000 series keypads
with 8-pin connector. Both 12 and 16 key types are supported. Connect D2-D9 to
Pin 1-8 on keypad.

#### Select Interface
```c
#define INTERFACE_SERIAL              1
#define INTERFACE_KEYBOARD            2
/** @brief switch for SERIAL or KEYBOARD interface */
#define INTERFACE           INTERFACE_SERIAL

#define KEYPAD_12_KEY       12
#define KEYPAD_16_KEY       16
/** @brief switch for Keypad with 12 Keys oder 16 keys */
#define KEYPAD_TYP        KEYPAD_12_KEY
```
