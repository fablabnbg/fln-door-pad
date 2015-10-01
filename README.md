FLN-Door-Pad
by: mose@fabfolk.com
under: MIT License

tested with Arduino 1.6.4

Inspired and partially based on
  keyPadHiduino Example Code
  by: Jim Lindblom
  MIT license

This programm was designed for use with an Arduino Pro Micro.
It provides an HID or serial interface for "Storm Interface" 1000 series keypads
with 8-pin connector. Both 12 and 16 key types are supported. Connect D2-D9 to
Pin 1-8 on keypad. Interface mode selectable with jumper. By pulling D0 (RXI
on Pro Micro) LOW it's in HID mode or w/o jumper it's a 9600 8N1 serial interface.
