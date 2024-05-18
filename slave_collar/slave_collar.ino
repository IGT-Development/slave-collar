//IGT Development production
//Version-0.3.0

const int greenLedPin = 2;
const int redLedPin = 3;
const int buttonPin = 4;
const int gearconPin = 5;
const int speakerPin = 6;
bool mode = true;
bool greenLedState = true;
bool redLedState = false;
int time = 0;
bool timeIsOver = false;
byte track = 0;
float carma = 0;

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(gearconPin, INPUT_PULLUP);
}

void loop() {
  digitalWrite(redLedPin, HIGH);
  if ((digitalRead(gearconPin) == LOW) && !timeIsOver) {
    delay(3000);
    destiny(mode);
    timeIsOver = !timeIsOver;
    time = 0;
    delay(2000);
  }
  if (digitalRead(gearconPin) == HIGH ) {
    if (mode == true) {
      digitalWrite(greenLedPin, LOW);
      delay(100);
      digitalWrite(greenLedPin, HIGH);
      delay(100);
    } else if (mode == false) {
      digitalWrite(redLedPin, LOW);
      delay(100);
      digitalWrite(redLedPin, HIGH);
      delay(100);
    }

    if (digitalRead(buttonPin) == LOW) {
      mode = !mode;
      if (mode){
        voice(3);
      } else {
        voice(2);
      }
      delay(1000);
    }
    timeIsOver = !timeIsOver;
  }
}

void destiny(bool lucky) {
  if (lucky) {
    track = 1;
  } else {
    track = 0;
  }
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  voice(6);
  do {
    delay(1000);
    time+=1;
    time = hitry(time);
    if (digitalRead(buttonPin) == LOW) {
      carma += 1;
      if (carma == 5) {
        voice(1);
        return;
        }
      }
    if (time == 6) {
      voice(50);
    } else if (time == 12) {
      voice(40);
    } else if (time == 18) {
      voice(30);
    } else if (time == 24) {
      voice(20);
    } else if (time == 30) {
      voice(10);
    } else if (time == 36) {
      voice(track);
      return;
    }
  } while (time <= 36);
}

int hitry(int time){
  if (digitalRead(gearconPin) == HIGH) {
      voice(0);
      time = 37;
      }
    return(time);
}

void voice(byte track) {
  /*Необходимо написать функцию воспроизведения аудиофайлов*/
  Serial.begin(9600);
  Serial.println(track);
}


/*
voice(byte track)
Принимает параметр типа byte (от 0 до 255)
track - название дорожки которое будет воспроизводиться динамиком
ниже представлен номер трека и его содержание:
0 - Оповещении о детонации ошейника;
1 - Оповещение об открытии замка ошейника;
2 - Оповещение о включении режима смертника;
3 - Оповещение о включении режима пленника;
4 - Инструкция с дальшейшими действиями при детонации;
5 - Инструкция с дальнейшими действиями при открытии;
6 - Оповещение об активации ошейника;
10 - Оповещение об окончении таймера через 10 минут;
20 - Оповещение об окончении таймера через 20 минут;
30 - Оповещение об окончении таймера через 30 минут;
40 - Оповещение об окончении таймера через 40 минут;
50 - Оповещение об окончении таймера через 50 минут.
*/