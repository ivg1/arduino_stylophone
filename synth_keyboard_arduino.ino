#include "notes.h"
#include <LiquidCrystal_I2C.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int note_pins[] = {2,3,4,5,6,7,8,9};

String curr_mode = "Freeplay";

int play_pin = 0;
int rec_pin = 1;
int freeplay_pin = A3;

int speaker_pin = A2;
int lower_octave = A1;
int add_octave = A0;

int curr_octave = 2; //+1
int notes[] = { NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1, NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6, NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7, NOTE_C8 };
bool note_pressed = false;

//SD stuff.
const int chipSelect = 10;
bool sd_init = false;
File rec_file;

bool record_state = false;
bool play_state = false;

void lcd_message(String msg1, String msg2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg1);
  lcd.setCursor(0, 1);
  lcd.print(msg2);
}

void setup() {
  lcd.begin();

  if (!SD.begin(chipSelect)) {
    lcd_message("insert SD card", "then click reset");
    while (1);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(note_pins[i], INPUT);
  }
  
  pinMode(speaker_pin, OUTPUT);
  pinMode(lower_octave, INPUT_PULLUP);
  pinMode(add_octave, INPUT_PULLUP);

  pinMode(play_pin, INPUT_PULLUP);
  pinMode(rec_pin, INPUT_PULLUP);
  pinMode(freeplay_pin, INPUT_PULLUP);
  
  lcd_message("Loaded.", "Press any key...");
  delay(1000);
}

String prev_note_letter = "";
String note_letter = "";
bool prev_state = false;

void play_free_notes() {
  note_pressed = false;
 
  if (digitalRead(lower_octave) == LOW && curr_octave != 0) { 
    curr_octave--; 

    lcd_message(curr_mode, "Set octave: " + String(curr_octave+1));

    noTone(speaker_pin);
    delay(200); 
  } 
  if (digitalRead(add_octave) == LOW && curr_octave != 6) { 
    curr_octave++; 

    lcd_message(curr_mode, "Set octave: " + String(curr_octave+1));
    
    noTone(speaker_pin);
    delay(200);
  }

  for (int key_pin = 2; key_pin < 10; key_pin++) {
    int read_pin = digitalRead(key_pin);

    if (read_pin == HIGH) {
      note_pressed = true;
      prev_state = true;
      
      int note_value = (key_pin - 2) + (7 * curr_octave);
      switch (key_pin - 2) {
        case 0: note_letter = "C" + String(curr_octave+1); break;
        case 1: note_letter = "D" + String(curr_octave+1); break;
        case 2: note_letter = "E" + String(curr_octave+1); break;
        case 3: note_letter = "F" + String(curr_octave+1); break;
        case 4: note_letter = "G" + String(curr_octave+1); break;
        case 5: note_letter = "A" + String(curr_octave+1); break;
        case 6: note_letter = "B" + String(curr_octave+1); break;
        case 7: note_letter = "C" + String(curr_octave+2); break;
        default: note_letter = "heck naw how??????"; break;
      }
    
      if (note_letter != prev_note_letter) {
        prev_note_letter = note_letter;
        lcd_message(curr_mode, "Playing note: " + note_letter);
      }
      tone(speaker_pin, notes[note_value]);
      delay(40);
    }
  }
  if (!note_pressed) {
    noTone(speaker_pin);
    if (prev_note_letter != "") {
      lcd_message(curr_mode, "...");
      prev_note_letter = "";
    }
  }
}

unsigned long note_start_time = 0;
String last_rec_note = "";
bool is_note_play = false;

void record_notes() {
  //same as in freeplay but with changes
  note_pressed = false;
  if (digitalRead(lower_octave) == LOW && curr_octave != 0) { 
    curr_octave--; 

    lcd_message(curr_mode, "Set octave: " + String(curr_octave+1));

    noTone(speaker_pin);
    delay(200); 
  } 
  if (digitalRead(add_octave) == LOW && curr_octave != 6) { 
    curr_octave++; 

    lcd_message(curr_mode, "Set octave: " + String(curr_octave+1));
    
    noTone(speaker_pin);
    delay(200);
  }
  for (int key_pin = 2; key_pin < 10; key_pin++) {
    int read_pin = digitalRead(key_pin);

    if (read_pin == HIGH) {
      note_pressed = true;
      int note_value = (key_pin - 2) + (7 * curr_octave);

      String current_note = "";
      switch (key_pin - 2) {
        case 0: current_note = "C" + String(curr_octave + 1); break;
        case 1: current_note = "D" + String(curr_octave + 1); break;
        case 2: current_note = "E" + String(curr_octave + 1); break;
        case 3: current_note = "F" + String(curr_octave + 1); break;
        case 4: current_note = "G" + String(curr_octave + 1); break;
        case 5: current_note = "A" + String(curr_octave + 1); break;
        case 6: current_note = "B" + String(curr_octave + 1); break;
        case 7: current_note = "V" + String(curr_octave + 2); break;
        default: current_note = "heck naw how??????"; break;
      }

      if (!is_note_play || current_note != last_rec_note) {
        if (is_note_play) {
          unsigned long dur = millis() - note_start_time;
          rec_file.println(last_rec_note + "-" + String(dur));
        }

        last_rec_note = current_note;
        note_start_time = millis();
        is_note_play = true;

        lcd_message(curr_mode, "recording: " + current_note);
        tone(speaker_pin, notes[note_value]);
      }
      break;
    }
  }

  if (!note_pressed && is_note_play) {
    unsigned long duration = millis() - note_start_time;
    rec_file.println(last_rec_note + "-" + String(duration));

    last_rec_note = "";
    is_note_play = false;

    lcd_message(curr_mode, "recording: stop");
    noTone(speaker_pin);
  }
}

String read_line(File &file) {
  String line = "";
  while (file.available()) {
    char c = file.read();
    if (c == '\n') {
      break;
    }
    line += c;
  }
  return line;
}

void play_recorded_notes() {
  rec_file = SD.open("REC.txt");
  if (!rec_file) {
    lcd_message("SD read error", "Play failed");
    return;
  }

  lcd_message("playing notes...", "");

  while (rec_file.available()) {
    String line = read_line(rec_file);
    if (line == "") continue;

    int dashIndex = line.indexOf('-');
    if (dashIndex == -1) continue;

    String note = line.substring(0, dashIndex);
    unsigned long duration = line.substring(dashIndex + 1).toInt();

    int note_value = 0;
    char note_letter = note.charAt(0);
    int octave = note.substring(1).toInt();

    int shift = 0;
    switch (note_letter) {
      case 'C': note_value = NOTE_C1; shift = 0; break;
      case 'D': note_value = NOTE_D1; shift = 1; break;
      case 'E': note_value = NOTE_E1; shift = 2; break;
      case 'F': note_value = NOTE_F1; shift = 3; break;
      case 'G': note_value = NOTE_G1; shift = 4; break;
      case 'A': note_value = NOTE_A1; shift = 5; break;
      case 'B': note_value = NOTE_B1; shift = 6; break;
      case 'V': note_value = NOTE_C2; shift = 7; break;
      default: continue;
    }
    
    shift += (octave - 1) * 7;

    if (digitalRead(play_pin) == LOW) {
      break;
    }
    lcd_message("playing: ", note);
    tone(speaker_pin, notes[shift]);
    delay(duration);
    noTone(speaker_pin);
    delay(100);
  }
  rec_file.close();
  
  lcd_message("play finished", "");
  play_state = false;
  curr_mode = "Freeplay";
  delay(1000);
}

void loop() {
  //check what button mode pressed
  if (digitalRead(play_pin) == LOW) {
    lcd_message("pressed play", "");
    delay(1000);

    if (!record_state) {
      if (!play_state) {
        play_state = true;
        curr_mode = "Playing";
        lcd_message("playing last", "recording...");
        delay(100);
      } else {
        lcd_message("stopped playing", "");
        play_state = false;
        curr_mode = "Freeplay";
      }
    } else {
      curr_mode = "Freeplay";
      record_state = false;
      lcd_message("stopped rec", "press play again");
    }
  } else if (digitalRead(rec_pin) == LOW) {
    lcd_message("pressed record", "");
    if (!record_state) {
      rec_file = SD.open("REC.txt", FILE_WRITE);
      if (!rec_file) {
        lcd_message("SD write error", "fix it");
      }
      delay(1000);
      curr_mode = "Recording";
      lcd_message("recording...", "");
      record_state = true;
      delay(100);
    } else {
      delay(10);
      curr_mode = "Freeplay";
      lcd_message("stop record...", "(freeplay on)");
      rec_file.close();
      record_state = false;
      delay(100);
    }

  } else if (digitalRead(freeplay_pin) == LOW) {
    lcd_message("pressed freeplay", "");
    delay(1000);
    curr_mode = "Freeplay";
    lcd_message(curr_mode, "...");
    delay(100);

  }

  if (curr_mode == "Freeplay") {
    play_free_notes();
  } else if (curr_mode == "Recording") {
    record_notes();
  } else if (curr_mode == "Playing") {
    play_recorded_notes();
  }
}
