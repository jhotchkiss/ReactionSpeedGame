#include <Adafruit_CircuitPlayground.h>

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  colorPatterns(5);
  delay(1000);
  colorPatterns(17);
  delay(1000);
  colorPatterns(21);
  delay(1000);
  colorPatterns(35);
  delay(1000);
}

void colorPatterns(int score) {
  if (score < 11) {
    for (int i = 0; i < score; i++){
      CircuitPlayground.setPixelColor(i, 255, 255, 255);
    }   
  }
  else if (score < 21) {
    for (int i = 0; i < score - 10; i++){
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }
  }
  else if (score < 31){
    for (int i = 0; i < score - 20; i++){
      CircuitPlayground.setPixelColor(i, 0, 0, 255);
    }
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
  delay(5000);
  CircuitPlayground.clearPixels();
}