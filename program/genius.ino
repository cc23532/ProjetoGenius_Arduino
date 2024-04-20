const int green = 11;
const int red = 13;
const int yellow = 9;
const int blue = 10;

const int btnGreen = 4;
const int btnRed = 6;
const int btnYellow = 5;
const int btnBlue = 3;
const int btnStart = 2;

const int buzzerPin = 11;

int arrayLeds[4] = {green, red, yellow, blue};
int arrayBtns[5] = {btnGreen, btnRed, btnYellow, btnBlue, btnStart};

int sequence[100];
int correctAnswersSequence = 0;
int index = 0;

bool runGame = false;
int volatileDelay;
int subtrator = 12;


void setup() {
 for (int i = 0; i < 4; i++) {
   pinMode(arrayLeds[i], OUTPUT);
 }
 for (int i = 0; i < 5; i++) {
   pinMode(arrayBtns[i], INPUT_PULLUP);
 }
 randomSeed(analogRead(0));

   Serial.begin(9600);

}


void startNewLevel() {
 for (int i = 0; i < 3; i++) {
    for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
      digitalWrite(arrayLeds[j], HIGH);
    }
    delay(200);
    for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
      digitalWrite(arrayLeds[j], LOW);
    }
    delay(200);
  }


}

void levelFailed() {
 createSound();
  for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
    digitalWrite(arrayLeds[j], HIGH);
  }
  delay(1000);
  for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
    digitalWrite(arrayLeds[j], LOW);
  }
  delay(200);

  for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
    digitalWrite(arrayLeds[j], HIGH);
  }
  delay(1500);
  for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
    digitalWrite(arrayLeds[j], LOW);
  }
}


void winGame(){
 createSound();
  for(int x = 0; x < 2; x++){
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
        digitalWrite(arrayLeds[j], HIGH);
      }
      delay(200);
      for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
        digitalWrite(arrayLeds[j], LOW);
      }
      delay(200);
    }
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
        digitalWrite(arrayLeds[j], HIGH);
      }
      delay(1000);
      for (int j = 0; j < sizeof(arrayLeds) / sizeof(arrayLeds[0]); j++) {
        digitalWrite(arrayLeds[j], LOW);
      }
      delay(200);
    }
  }
}


void sequenceNewLength() {
  if(correctAnswersSequence < 100){
    sequence[correctAnswersSequence++] = random(4); 
  }
  else{
    winGame();
  }
}


void runSequence() {
  for (int i = 0; i < correctAnswersSequence; i++) {
    blinkLed(sequence[i]);
    int t = 500;
    if(volatileDelay > t){
      delay(t);
    }
    else {
      delay(volatileDelay);
    }
  }
}


void blinkLed(int led) {
  digitalWrite(arrayLeds[led], HIGH);
  delay(volatileDelay);
  digitalWrite(arrayLeds[led], LOW);
}


void game(){
  for (int i = 0; i < sizeof(arrayLeds) / sizeof(arrayLeds[0]); i++) {
    if (digitalRead(arrayBtns[i]) == LOW) {
      delay(250);

      if (i == sequence[index]) {
        index++;
        
        if (index == correctAnswersSequence) {
          startNewLevel();
          delay(500);
          sequenceNewLength();
          runSequence();
          index = 0;
          volatileDelay -= subtrator;
          Serial.println(volatileDelay);
        }
      } 
      else {
        levelFailed();
        runGame = false;
      }
      break;
    }
  }
}

void createSound() {
  int melody[] = {262, 196 ,196, 220, 196 ,0, 247, 262};
  
  int noteDurations[] = {4, 8, 8, 4 ,4 ,4 ,4 ,4 };

 for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
    }
}


void loop() {
  if (digitalRead(btnStart) == LOW) { 
    delay(250);
    startNewLevel();
    delay(250);


    if (!runGame) {
      volatileDelay = 1500;
      runGame = true;
      correctAnswersSequence = 0;
      sequenceNewLength();
      runSequence();
      index = 0;
    }
  }

 if (runGame) {
    game();
  }
}
