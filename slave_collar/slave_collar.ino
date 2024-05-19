//IGT Development production by AlexSNAKE
//Version-1.0a - 19.05.2024

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const byte greenLedPin = 2;
const byte redLedPin = 3;
const byte buttonPin = 4;
const byte gearconPin = 5;
const byte speakerTxPin = 10;
const byte speakerRxPin = 11;
bool lethalMode;
bool collarReady;
unsigned int time;
byte track;
byte carma;
SoftwareSerial softwareSerial(speakerTxPin, speakerRxPin);
DFRobotDFPlayerMini p;

void setup() {
	softwareSerial.begin(9600);
  if (!p.begin(softwareSerial)) {while(true);}
  p.volume(30);

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(gearconPin, INPUT_PULLUP);
}

void loop() {
  if (!collarOpenedState() && collarReady) {
    collarActivation(lethalMode);
  }
  
  if (collarOpenedState()) {
    collarReady = true;
    selectedModeIndication();
    changeCollarMode();
  }
}

void collarActivation(bool monitoringMode) {
  track = monitoringMode ? 2 : 1;
  redLedOn();
  voice(7);
  do {
    delay(1000);
    time += 1;
    shouldCollarDetonate();
    goodBoy();
    checkTimeLeft();
  } while (time <= 36000);
  voice(track+4);
  collarReady = false;
  time = 0;
  carma = 0;
}

void shouldCollarDetonate() {
  if (collarOpenedState()) {
    track = 1;
    voice(track);
    time = 65535;
  }
}

void goodBoy() {
  if (buttonPressedState()) {
    carma += 1;
    if (carma == 5) {
      track = 2;
      voice(track);
      time = 65535;
    }
  } else {
    carma = 0;
  }
}

void checkTimeLeft() {
      if (time == 600) {
      voice(50);
    } else if (time == 1200) {
      voice(40);
    } else if (time == 1800) {
      voice(30);
    } else if (time == 2400) {
      voice(20);
    } else if (time == 3000) {
      voice(10);
    } else if (time == 3600) {
      voice(track);
    }
}

void selectedModeIndication() {
  delay(250);
  (lethalMode) ? redLedOn() : greenLedOn();
  delay(250);
  redLedOff();
  greenLedOff();
}

void changeCollarMode() {
      if (buttonPressedState()) {
      lethalMode = !lethalMode;
      (lethalMode) ? voice(3) : voice(4);
    }
}

void greenLedOn() {
  digitalWrite(greenLedPin, HIGH);
}

void greenLedOff() {
  digitalWrite(greenLedPin, LOW);
}

void redLedOn() {
  digitalWrite(redLedPin, HIGH);
}

void redLedOff() {
  digitalWrite(redLedPin, LOW);
}

bool buttonPressedState() {
  return(digitalRead(buttonPin) == LOW);
}

bool collarOpenedState() {
  return(digitalRead(gearconPin) == HIGH);
}

void voice(byte track) {
    p.playMp3Folder(track);
    delay(3000);
}