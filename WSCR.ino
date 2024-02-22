// AUTHOR - LAURENCE PROCTER
// A SCRIPT TO TEST SPINDLE CABLES FOR CONNECTION BREAKS WHILE CYCLING BACK AND FORTH

// LIBS
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// PINS
#define go_button 2
#define stp 3
#define dir 4
#define limit 5
#define home_button 6
#define cable_1 7
#define cable_2 8
#define cable_3 9
#define cable_4 10
#define cable_5 11
#define cable_1_LED 13
#define cable_2_LED 14
#define cable_3_LED 15
#define cable_4_LED 16
#define cable_5_LED 17

// OLED
#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
  
// ONE STEP PULSE FOR STEPPER MOTOR
void step() {
  digitalWrite(stp, HIGH);
  delayMicroseconds(500);
  digitalWrite(stp, LOW);
  delayMicroseconds(500);
}
  
// HOMING PROCEDURE
void home() {
  oled.clearDisplay(); // clear display
  oled.setTextSize(2);          // text size
  oled.setCursor(5, 10);        // position to display
  oled.println("HOMING..."); // text to display
  oled.display(); // show on OLED
  delay(1000);
  Serial.println("YAY");
  
  digitalWrite(dir, HIGH);
  while (digitalRead(limit) == LOW) {
    step();
  }
  digitalWrite(dir, LOW);
  for (int i = 0; i <= 100; i++) {
    step();
  }
  oled.clearDisplay(); // clear display
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 0);        // position to display
  oled.println("PLUG IN CABLES AND"); // text to display
  oled.println("PRESS GREEN BUTTON"); // text to display
  oled.println("TO BEGIN TEST"); // text to display
  oled.display();               // show on OLED
}
  
// SETS FLAG AS FAIL
void cable_1_fail() {
  digitalWrite(cable_1_LED, LOW); // RED LED ON, GREEN LED OFF
}

void cable_2_fail() {
  digitalWrite(cable_2_LED, LOW); // RED LED ON, GREEN LED OFF
}

void cable_3_fail() {
  digitalWrite(cable_3_LED, LOW); // RED LED ON, GREEN LED OFF
}

void cable_4_fail() {
  digitalWrite(cable_4_LED, LOW); // RED LED ON, GREEN LED OFF
}

void cable_5_fail() {
  digitalWrite(cable_5_LED, LOW); // RED LED ON, GREEN LED OFF
}

// CYCLES AND TESTS CABLES FOR CONNECTION BREAKS
void test_procedure() {
  oled.clearDisplay(); // clear display
  oled.setTextSize(2);          // text size
  oled.setCursor(5, 10);        // position to display
  oled.println("TESTING..."); // text to display
  oled.display(); // show on OLED
  delay(100);

  pinMode(cable_1, INPUT_PULLUP);
  pinMode(cable_2, INPUT_PULLUP);
  pinMode(cable_3, INPUT_PULLUP);
  pinMode(cable_4, INPUT_PULLUP);
  pinMode(cable_5, INPUT_PULLUP);

  delay(50);
  
// SETS ALL RESULT LEDS GREEN
  digitalWrite(cable_1_LED, HIGH); // RED LED OFF, GREEN LED ON
  digitalWrite(cable_2_LED, HIGH); // RED LED OFF, GREEN LED ON
  digitalWrite(cable_3_LED, HIGH); // RED LED OFF, GREEN LED ON
  digitalWrite(cable_4_LED, HIGH); // RED LED OFF, GREEN LED ON
  digitalWrite(cable_5_LED, HIGH); // RED LED OFF, GREEN LED ON

  delay(500);

  digitalWrite(cable_1_LED, not(digitalRead(cable_1)));
  digitalWrite(cable_2_LED, not(digitalRead(cable_2)));
  digitalWrite(cable_3_LED, not(digitalRead(cable_3)));
  digitalWrite(cable_4_LED, not(digitalRead(cable_4)));
  digitalWrite(cable_5_LED, not(digitalRead(cable_5)));

  delay(50);

  // SETS INTERRUPTS FOR TEST
 
  attachInterrupt(digitalPinToInterrupt(cable_1), cable_1_fail, RISING);
  attachInterrupt(digitalPinToInterrupt(cable_2), cable_2_fail, RISING);
  attachInterrupt(digitalPinToInterrupt(cable_3), cable_3_fail, RISING);
  attachInterrupt(digitalPinToInterrupt(cable_4), cable_4_fail, RISING);
  attachInterrupt(digitalPinToInterrupt(cable_5), cable_5_fail, RISING);

  delay(50);

  // oled.clearDisplay(); // clear display
  // oled.setTextSize(2);          // text size
  // oled.setCursor(5, 10);        // position to display
  // oled.println("TESTING..."); // text to display
  // oled.display(); // show on OLED
  // delay(1000);

  // CYCLES CABLES TEN TIMES
  for (int i = 0; i <= 10; i++) {
    digitalWrite(dir, LOW);
    delay(100);
    for (int i = 0; i <= 5000; i++) {
      step();
    }
    delay(100);
    digitalWrite(dir, HIGH);
    delay(100);
    for (int i = 0; i <= 5000; i++) {
      step();
    }
    delay(100);

  }

  // SETS PROMPT MESSAGE ON OLED SCREEN
  oled.clearDisplay(); // clear display
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 0);        // position to display
  oled.println("PLUG IN CABLES AND"); // text to display
  oled.println("PRESS GREEN BUTTON"); // text to display
  oled.println("TO BEGIN TEST"); // text to display
  oled.display();               // show on OLED

}

void setup() {
  Serial.begin(9600);
  
  pinMode(home_button, INPUT_PULLUP);
  pinMode(go_button, INPUT_PULLUP);
  pinMode(limit, INPUT_PULLUP);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(cable_1_LED, OUTPUT);
  pinMode(cable_2_LED, OUTPUT);
  pinMode(cable_3_LED, OUTPUT);
  pinMode(cable_4_LED, OUTPUT);
  pinMode(cable_5_LED, OUTPUT);

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      while (true);
  }
  
      delay(2000);         // wait for initializing
      oled.clearDisplay(); // clear display

      // SETS PROMPT MESSAGE ON OLED SCREEN
      oled.setTextSize(1);          // text size
      oled.setTextColor(WHITE);     // text color
      oled.setCursor(0, 10);        // position to display
      oled.println("PRESS ORANGE BUTTON"); // text to display
      oled.println("TO HOME"); // text to display
      oled.display();               // show on OLED
  

}
void loop() {
  while (digitalRead(home_button) == HIGH) {  
    Serial.println("NT");
  }
  home();

  while (true) {
    while (digitalRead(go_button) == HIGH) {  
      Serial.println("NT");
    }
    test_procedure();
  }

}