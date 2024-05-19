//IGT Development production
//Version-0.9.5

//Необходимые библиотеки
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Параметры конфигурации PIN
const byte greenLedPin = 2;
const byte redLedPin = 3;
const byte buttonPin = 4;
const byte gearconPin = 5;
const byte speakerTxPin = 10;
const byte speakerRxPin = 11;
SoftwareSerial softwareSerial(speakerTxPin, speakerRxPin); // Присвоение пинов связи с TX и RX

bool lethalMode;
bool collarReady;
unsigned int time;
byte track;
byte carma;
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


//Функция voice возспроизводит требуемый аудиофайл принимая его название как входящий параметр
void voice(byte track) { //Принимает параметр типа int
    p.playMp3Folder(track); //Воспроизведение аудиофайла по названию
    delay(3000); //Задежка для корректного воспроизведения аудиофайла
}

/*
1 - Оповещении о детонации ошейника;
2 - Оповещение об открытии замка ошейника;
3 - Оповещение о включении режима смертника;
4 - Оповещение о включении режима пленника;
5 - Инструкция с дальшейшими действиями при детонации;
6 - Инструкция с дальнейшими действиями при открытии;
7 - Оповещение об активации ошейника;
10 - Оповещение об окончении таймера через 10 минут;
20 - Оповещение об окончении таймера через 20 минут;
30 - Оповещение об окончении таймера через 30 минут;
40 - Оповещение об окончении таймера через 40 минут;
50 - Оповещение об окончении таймера через 50 минут.
*/