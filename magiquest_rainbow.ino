#define DECODE_MAGIQUEST

//#define DECODE_DISTANCE     // universal decoder for pulse width or pulse distance protocols
//#define DEBUG // Activate this for lots of lovely debug output from the decoders.
//#define INFO                // To see valuable informations from universal decoder for pulse width or pulse distance protocols
//#define DECODE_HASH         // special decoder for all protocols

#define NOTE_C4  261
#define NOTE_C4sharp  277
#define NOTE_D4 293
#define NOTE_D4sharp  311
#define NOTE_E4 329
#define NOTE_F4 349
#define NOTE_F4sharp  369
#define NOTE_G4 392
#define NOTE_G4sharp  415
#define NOTE_A4 440
#define NOTE_A4sharp  466
#define NOTE_B4 493
#define NOTE_C5 523
#define NOTE_C5sharp  554
#define NOTE_D5 587
#define NOTE_D5sharp  622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_F5sharp  739
#define NOTE_G5 783
#define NOTE_G5sharp  830
#define NOTE_A5 880
#define NOTE_A5sharp  932
#define NOTE_B5 987
#define NOTE_C6 1046
#define NOTE_C6sharp 1108

#include <Arduino.h>
#include <IRremote.hpp>
#include <Adafruit_CircuitPlayground.h>
#include <ctime>
#include <cstdlib>


void setup() {
  srand((unsigned)time(0));
  CircuitPlayground.begin();
  Serial.begin(9600);
  Serial.println("Ready to receive IR Signals");
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  /*
     Start the receiver, enable feedback LED and take LED feedback pin from the internal boards definition
  */
  IrReceiver.begin(CPLAY_IR_RECEIVER, true, false);

  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.print(F("at pin "));
  Serial.println(CPLAY_IR_RECEIVER);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void loop() {

  if (IrReceiver.decode()) {

    int magiquest = 24;
    int wandRawData = 16464;
    if (IrReceiver.decodedIRData.protocol == magiquest &&
        IrReceiver.decodedIRData.decodedRawData == wandRawData) {
      IrReceiver.printIRResultShort(&Serial);
      Serial.println(IrReceiver.decodedIRData.decodedRawData);
      IrReceiver.printIRResultRawFormatted(&Serial, true);

      togglePin(2);

      if (rand() % 20 == 0) {
        playChestOpenSong();
      }
      spinRainbow()
      //blinkLights(8);
    }


    /*
       !!!Important!!! Enable receiving of the next value,
       since receiving has stopped after the end of the current received data packet.
    */
    IrReceiver.resume(); // Enable receiving of the next value
  }
}

void blinkLights(int iters) {
  for (int i = 0; i < iters; i++) {
    for (int i = 0; i < 10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, i*25, i*10, 200);
    }
    // Show all the pixels.
    CircuitPlayground.strip.show();
    delays(19000);
    turnOffLeds();
    delays(6000);
  }
}

void delays(int iters) {
  for (int i = 0; i < iters; i++) {
    Serial.println();
  }
}

void spinRainbow(int iters) {
  for (int i = 0; i < iters; i++) {
    // Make an offset based on the current millisecond count scaled by the current speed.
    uint32_t offset = millis() / 5;
    // Loop through each pixel and set it to an incremental color wheel value.
    for (int i = 0; i < 10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, CircuitPlayground.colorWheel(((i * 256 / 10) + offset) & 255));
    }
    // Show all the pixels.
    CircuitPlayground.strip.show();
  }
  turnOffLeds();
}
void turnOffLeds() {
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.strip.setPixelColor(i, 0);
  }
  CircuitPlayground.strip.show();
}

void playChestOpenSong() {
  CircuitPlayground.playTone(NOTE_F4sharp, 80);
  CircuitPlayground.playTone(NOTE_A4sharp, 80);
  CircuitPlayground.playTone(NOTE_C5sharp, 80);
  CircuitPlayground.playTone(NOTE_B4, 80);
  CircuitPlayground.playTone(NOTE_D5sharp, 80);
  CircuitPlayground.playTone(NOTE_F5sharp, 80);
  CircuitPlayground.playTone(NOTE_C6sharp, 400);
}

void togglePin(int whichPin) {
  if (digitalRead(whichPin) == LOW) {
    digitalWrite(whichPin, HIGH);
  } else {
    digitalWrite(whichPin, LOW);
  }
}
