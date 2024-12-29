# Arduino stylophone
A synth-like keyboard with 8 set keys to represent one octave with 7 octave pitches to switch between.

## Features
- Freeplay mode, allows users to play around with the keyboard and come up with melodies
- Recording mode, after the user creates a melody, or learns one, he/she can record the melody
- Playing mode, having recorded the melody, the user can listen to it infinitely
- Feedback on the LCD (every user action is commented on)

## Installing
1. Download this repository as a zip file
2. Extract the contents into a folder (e.g. synth_keyboard)
3. Then you need to download the libraries (shown below)
4. Wire the arduino as shown here: https://wokwi.com/projects/418610242763678721
6. After that just open the .ino file and upload to the arduino
7. You're all set

## Required parts
1. Arduino Uno R3
2. LCD I2C 16x2 (or bigger)
3. Buzzer/Speaker (recommended speaker for smoother sound)
4. SD card module
5. 5x buttons
6. 5x 220ohm resistors to apply to each of the 5 buttons
7. 8x Touch sensors (or try buttons, but you might need to alter the code)
8. Jumper wires (a lot)
9. A power supply to the arduino

## Downloading libraries
1. The notes.h library is already in this repository.
2. To download the <LiquidCrystal_I2C.h> library go to: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
3. Download SD library from ArduinoIDE library manager (or any other method because any one works I believe)

## Wiring of project
You can see the wiring I used for this project here: https://wokwi.com/projects/418610242763678721.
Ignore the code, it doesn't work half the time because I couldnt import libraries I used, But do look at the wiring.

### ! Attention !
I built this project using touch sensors, so if you have them please use them instead of buttons (as on the wowki).
I didn't try using buttons, so some issues might arise. So once again, if you have touch sensors, use them.
