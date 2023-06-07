#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay timer;
int points;
int randomNum;
volatile bool switchFlag, leftButton, rightButton;
float midi[127];
int A_four = 440;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  CircuitPlayground.begin();
  generateMIDI();
  attachInterrupt(digitalPinToInterrupt(4), leftButtonPress, CHANGE);
  attachInterrupt(digitalPinToInterrupt(5), rightButtonPress, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), switchMove, CHANGE);
  randomSeed(analogRead(0));
}

void loop() {
  if (switchFlag){
    delay(10);
    leftButton = 0;
    rightButton = 0;
    switchFlag = 0;
    randomNum = random(2);
    if (randomNum == 0){
      CircuitPlayground.setPixelColor(random(10), 255, 255, 255);
    }
    if (randomNum == 1){
      playSound(points);
    }
    timer.expire();
    timer.start(2000 - (points*50), AsyncDelay::MILLIS);
    while(!timer.isExpired() && !leftButton && !rightButton) {
    }
    if (randomNum == 1 && rightButton){
      endOfGame(points);
      CircuitPlayground.clearPixels();
    }
    else if (randomNum == 0 && leftButton){
      endOfGame(points);
      CircuitPlayground.clearPixels();
    }
    else if (randomNum == 1 && leftButton){
      points++;
      CircuitPlayground.clearPixels();
      correctGuess();
    }
    else if (randomNum == 0 && rightButton){
      points++;
      CircuitPlayground.clearPixels();
      correctGuess();
    }
    else {
      endOfGame(points);
      CircuitPlayground.clearPixels();
    }
  }
}

void endOfGame(int score) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      CircuitPlayground.setPixelColor(j, 255, 0, 0);
    }
    delay(250);
    CircuitPlayground.clearPixels();
    delay(250);
  }
  colorPatterns(score);
  CircuitPlayground.clearPixels();
  points = 0;
}

void colorPatterns(int score) {
  if (score < 11) {
    for (int i = 0; i < score; i++){
      CircuitPlayground.setPixelColor(i, 255, 255, 255);
    }
    delay(5000);
  }
  else if (score < 21) {
    for (int i = 0; i < score - 10; i++){
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }
    delay(5000);
  }
  else if (score < 31){
    for (int i = 0; i < score - 20; i++){
      CircuitPlayground.setPixelColor(i, 0, 0, 255);
    }
    delay(5000);
  }
  else {
    for (int i = 0; i < 20; i++) {
      if (i % 2 == 0){
        for (int j = 0; j < 10; j++){
          if (j%2 == 0)
            CircuitPlayground.setPixelColor(j, 255, 0, 255);
          if (j%2 == 1)
            CircuitPlayground.setPixelColor(j, 255, 255, 255);
        }
      }
      else {
        for (int j = 0; j < 10; j++){
          if (j%2 == 1)
            CircuitPlayground.setPixelColor(j, 255, 0, 255);
          if (j%2 == 0)
            CircuitPlayground.setPixelColor(j, 255, 255, 255);
        }
      }
      delay(100);
    }
  }
  CircuitPlayground.clearPixels();
}

void correctGuess() {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);
  }
  delay(250);
  CircuitPlayground.clearPixels();
}

void generateMIDI()
{
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}

void playSound(int total) {
  CircuitPlayground.playTone(midi[random(50,70)], 400- (total*5));
}

void switchMove(){
  switchFlag = 1;
}

void leftButtonPress(){
  leftButton = 1;
}

void rightButtonPress(){
  rightButton = 1;
}