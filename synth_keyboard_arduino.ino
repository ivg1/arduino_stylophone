#include "notes.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// you can comment anything that is related to Serial, but it might make sounds to fast

int note_pins[] = {2,3,4,5,6,7,8,9};
int speaker_pin = 10;
int lower_octave = 11;
int add_octave = 12;
int curr_octave = 2; //+1
int notes[] = { NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1, NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6, NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7, NOTE_C8 };

bool note_pressed = false;

void setup() {
  Serial.begin(9600);
  lcd.begin();

  for (int i = 0; i < 8; i++) {
    pinMode(note_pins[i], INPUT);
  }
  pinMode(lower_octave, INPUT_PULLUP);
  pinMode(add_octave, INPUT_PULLUP);
  
  lcd.print("Set octave: ");
  lcd.print(curr_octave+1);
}

void loop() {
  note_pressed = false;

  if (digitalRead(lower_octave) == LOW && curr_octave != 0) { 
    curr_octave--; 

    lcd.clear();
    lcd.print("Set octave: ");
    lcd.print(curr_octave+1);

    Serial.print("decreased octave. playing octave: "); 
    Serial.println(curr_octave);

    noTone(speaker_pin);
    delay(200); 
  } 
  if (digitalRead(add_octave) == LOW && curr_octave != 6) { 
    curr_octave++; 

    lcd.clear();
    lcd.print("Set octave: ");
    lcd.print(curr_octave+1);
    
    Serial.print("increased octave.  playing octave: "); 
    Serial.println(curr_octave); 
    
    noTone(speaker_pin);
    delay(200); 
  } 

  for (int key_pin = 2; key_pin < 10; key_pin++) {
    int read_pin = digitalRead(key_pin);

    if (read_pin == HIGH) {
      note_pressed = true;
      int note_value = (key_pin-2) + (7*curr_octave);

      tone(speaker_pin, notes[note_value]);

      Serial.print("pressed keypin: ");
      Serial.print(key_pin);
      Serial.print("  note value = ");
      Serial.println(notes[key_pin-2+(8*curr_octave)]);
    }
  }
  if (!note_pressed) noTone(speaker_pin);
}
