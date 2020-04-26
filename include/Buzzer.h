
#ifndef FROMIDE_BUZZER_H
#define FROMIDE_BUZZER_H

#include <stdint.h>

#define TEMPO 113

const int song1_length = 18;
const char song1_notes[] = "cdfda ag cdfdg gf ";
const int song1_beats[] = {1, 1, 1, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 1, 1, 4, 4, 2};


/*void setup() {
    pinMode(buzzerPin, OUTPUT);
}*/


/*void loop() {
    int i, duration;

    for (i = 0; i < songLength; i++) {
        duration = beats[i] * tempo;
        if (notes[i] == ' ') {
            delay(duration);
        } else {
            tone(buzzerPin, frequency(notes[i]), duration);
            delay(duration);
        }
        delay(tempo / 10);
    }

    while (true) {}
}*/


int frequency(char note) {

    int i;
    const uint8_t numNotes = 8;

    char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
    int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};

    for (i = 0; i < numNotes; i++) {
        if (names[i] == note) {
            return (frequencies[i]);
        }
    }
    return (0);
}


#endif 