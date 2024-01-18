#include "pitches.h"

int melody[] = {
    NOTE_E4, NOTE_E4, 0, NOTE_E4, 0, NOTE_C4, NOTE_E4, 0, NOTE_G4, 0, 0, 0, 0, 0, NOTE_C4,
    0, NOTE_G3, 0, NOTE_E3, 0, NOTE_A4, 0, NOTE_B4, 0, NOTE_AS4, 0, NOTE_G3, 0, NOTE_E4, 0, NOTE_G4, 0,
    NOTE_A4, 0, NOTE_F4, NOTE_G4, 0, NOTE_E4, 0, NOTE_C4, NOTE_D4, NOTE_B4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 4, 8, 4, 8, 4, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

void setup() {

}

void loop() {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
       int noteDuration = 1000 / noteDurations[thisNote];
       tone(8, melody[thisNote], noteDuration);
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);
    noTone(8);
    }
}
