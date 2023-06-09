#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

//initialize variables
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
  //create interrupts for both buttons and the slide switch
  attachInterrupt(digitalPinToInterrupt(4), leftButtonPress, CHANGE);
  attachInterrupt(digitalPinToInterrupt(5), rightButtonPress, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), switchMove, CHANGE);
  randomSeed(analogRead(0));
}

void loop() {
  //if the switch has been moved
  if (switchFlag){
    delay(10);
    leftButton = 0;
    rightButton = 0;
    switchFlag = 0;
    
    //set randomNum to 0 or 1
    randomNum = random(2);
    
    // if the randomNum is 0 set a random pixel to show white
    if (randomNum == 0){
      CircuitPlayground.setPixelColor(random(10), 255, 255, 255);
    }
    
    // if the randomNum is 1, play a random sound
    if (randomNum == 1){
      playSound(points);
    }
    
    // make the timer expire
    timer.expire();
    
    //start a new timer that will be 2000 milliseconds minus the number of points times 50
    timer.start(2000 - (points*50), AsyncDelay::MILLIS);
    
    // while nothing is pressed and the timer has not expired, do nothing
    while(!timer.isExpired() && !leftButton && !rightButton) {
    }
    
    // if the randomNum is 1 and the right button is pressed, end the game
    if (randomNum == 1 && rightButton){
      endOfGame(points);
      CircuitPlayground.clearPixels();
    }

    // else if the randomNum is 0 and left button is pressed, also end the game
    else if (randomNum == 0 && leftButton){
      endOfGame(points);
      CircuitPlayground.clearPixels();
    }
    
    // else if the randomNum is 1 and left button is pressed, add a point and light the board up green
    else if (randomNum == 1 && leftButton){
      points++;
      CircuitPlayground.clearPixels();
      correctGuess();
    }
    
    // else if the randomNum is 0 and the right button is pressed, increment points and light up the board green
    else if (randomNum == 0 && rightButton){
      points++;
      CircuitPlayground.clearPixels();
      correctGuess();
    }
    
    // if anything else happens, end the game
    else {
      endOfGame(points);
      CircuitPlayground.clearPixels();
    }
  }
}

void endOfGame(int score) {
  //flash the board red three times
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      CircuitPlayground.setPixelColor(j, 255, 0, 0);
    }
    delay(250);
    CircuitPlayground.clearPixels();
    delay(250);
  }

  // call the colorPatterns function
  colorPatterns(score);
  CircuitPlayground.clearPixels();

  //reset the points total
  points = 0;
}

void colorPatterns(int score) {
  // if the score is less than 11, light up the number of pixels that equal points on the board in white
  if (score < 11) {
    for (int i = 0; i < score; i++){
      CircuitPlayground.setPixelColor(i, 255, 255, 255);
    }
    delay(5000);
  }

  // if the score is less than 21, light the board up with points minus 10 pixels and make them green
  else if (score < 21) {
    for (int i = 0; i < score - 10; i++){
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }
    delay(5000);
  }

  // if the score is less than 31, light the board up with points minus 20 pixels and make them blue
  else if (score < 31){
    for (int i = 0; i < score - 20; i++){
      CircuitPlayground.setPixelColor(i, 0, 0, 255);
    }
    delay(5000);
  }

  // if score is any greater than 30, light up the board with an alternating, sparkling pattern flashing between white and magenta
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

// flash the board green for every correct response
void correctGuess() {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);
  }
  delay(250);
  CircuitPlayground.clearPixels();
}

// build an array that can be called to play a certain sound
void generateMIDI()
{
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}

// play a random sound that is a value in the array midi somewhere between 50 and 70
void playSound(int total) {
  CircuitPlayground.playTone(midi[random(50,70)], 400- (total*5));
}

// make switchFlag true if the switch moves
void switchMove(){
  switchFlag = 1;
}

// make the leftButton boolean true if the left button is pressed
void leftButtonPress(){
  leftButton = 1;
}

// make the rightButton boolean true if the right button is pressed
void rightButtonPress(){
  rightButton = 1;
}