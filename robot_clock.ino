// Code by : ScitiveX
// Arduino Robot Clock
//    ___              _      _        _                    __  __
//   / __|    __      (_)    | |_     (_)    __ __    ___   \ \/ /
//   \__ \   / _|     | |    |  _|    | |    \ V /   / -_)   >  <
//   |___/   \__|_   _|_|_   _\__|   _|_|_   _\_/_   \___|  /_/\_\  
// _|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|
// "`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'
//

// Video Link : https://youtu.be/SR46WDzWvUk

//Libraries

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

RTC_DS3231 rtc;
Servo neckServo, leftArmServo, rightArmServo;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

int leftArmPos = 0;
int rightArmPos = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error: Could not find OLED module!");
    while (1);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);

  if (!rtc.begin()) {
    Serial.println("Error: Could not find RTC module!");
    while (1);
  }

  neckServo.attach(9);
  leftArmServo.attach(10);
  rightArmServo.attach(11);


  neckServo.write(0);
  leftArmServo.write(0);
  rightArmServo.write(0);
}

void loop() {
  DateTime now = rtc.now();

  display.clearDisplay();

  // Draw left eye and display hour
  int leftEyeX = 28;
  int leftEyeY = 22;
  int leftEyeWidth = 32;
  int leftEyeHeight = 20;
  int leftEyeCornerRadius = 4;

  // Fill left eye
  display.fillRoundRect(leftEyeX, leftEyeY, leftEyeWidth, leftEyeHeight, leftEyeCornerRadius, WHITE);

  // Display hour in left eye
  if (now.second() % 2 == 0) {
    display.setTextColor(BLACK);
  }
  else {
    display.setTextColor(WHITE);
  }
  display.setCursor((leftEyeX + leftEyeWidth / 2 - 6) - 5, leftEyeY + leftEyeHeight / 2 - 8);
  display.setTextSize(2);
  display.print(now.hour(), DEC);

  // Draw right eye and display minute
  int rightEyeX = 68;
  int rightEyeY = 22;
  int rightEyeWidth = 32;
  int rightEyeHeight = 20;
  int rightEyeCornerRadius = 4;

  // Fill right eye
  display.fillRoundRect(rightEyeX, rightEyeY, rightEyeWidth, rightEyeHeight, rightEyeCornerRadius, WHITE);

  // Display minute in right eye
  // display.setTextColor(BLACK);
  display.setCursor((rightEyeX + rightEyeWidth / 2 - 6) - 5, rightEyeY + rightEyeHeight / 2 - 8);
  display.setTextSize(2);
  display.print(now.minute(), DEC);

  Serial.print(now.hour() );
  Serial.print(":");
  Serial.println(now.minute() );
  display.display(); 

  // Move servos every hour
  if (now.minute() == 0 && now.second()<20) {
    // Move arms up and down 5 times
    for (int i = 0; i < 5; i++) {
      // Move left arm clockwise
      for (leftArmPos = 0; leftArmPos <= 180; leftArmPos += 1) {
        leftArmServo.write(leftArmPos);
        rightArmServo.write(180 - leftArmPos);
        delay(10);
      }

      // Move left arm anticlockwise
      for (leftArmPos = 180; leftArmPos >= 0; leftArmPos -= 1) {
        leftArmServo.write(leftArmPos);
        rightArmServo.write(180 - leftArmPos);
        delay(10);
      }

      // Move right arm clockwise
      for (rightArmPos = 0; rightArmPos <= 180; rightArmPos += 1) {
        leftArmServo.write(rightArmPos);
        rightArmServo.write(180 - rightArmPos);
        delay(10);
      }

      // Move right arm anticlockwise
      for (rightArmPos = 180; rightArmPos >= 0; rightArmPos -= 1) {
        leftArmServo.write(rightArmPos);
        rightArmServo.write(180 - rightArmPos);
        delay(10);
      }
    }
  }

 
  delay(1000); 
}
