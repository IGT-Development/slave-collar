#IGT Development production
#Version-0.2.0

const int greenLedPin = 13;
const int redLedPin = 12;
const int speakerPin = 11;
const int buttonPin = 10;
const int gearconPin = 9;
bool mode = true;
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
  if (digitalRead(gearconPin) == HIGH) {
    destiny(mode);
  }
  if (digitalRead(gearconPin) == LOW ) {
    if (mode == true) {
      digitalWrite(greenLedPin, HIGH);
      delay(200);
      digitalWrite(greenLedPin, LOW);
      delay(1000);
    } else if (mode == false) {
      digitalWrite(redLedPin, HIGH);
      delay(200);
      digitalWrite(redLedPin, LOW);
      delay(1000);
    }

    if (digitalRead(buttonPin) == HIGH) {
      mode = !mode;
      if (mode){
        voice(3);
      } else {
        voice(2);
      }
    }
  }
}

void destiny(bool lucky) {
  if (lucky) {
    track = 1;
  } else {
    track = 0;
  }
  digitalWrite(redLedPin, HIGH);
  voice(6);
  for (int i = 0; i < 3600000; i += 500) {
    delay(500);
    if (digitalRead(gearconPin) == LOW) {
      Track = 0;
      break;
    }
	 if (digitalRead(buttonPin) == HIGH) {
	 	carma += 0.5;
		if (carma == 5) {
		lucky = 1;
		break;
		}
	 }
    if (i == 600000) {
		(mode ? voice(50) : voice(150));
    } else if (i == 1200000) {
      (mode ? voice(40) : voice(140));
    } else if (i == 1800000) {
      (mode ? voice(30) : voice(130));
    } else if (i == 2400000) {
      (mode ? voice(20) : voice(120));
    } else if (i == 3000000) {
      (mode ? voice(10) : voice(110));
    }
  }
  voice(track);
  delay(10000);
  voice(track+4);
}

void voice(byte track) {
  /*Необходимо написать функцию воспроизведения аудиофайлов*/
  Serial.print(track);
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