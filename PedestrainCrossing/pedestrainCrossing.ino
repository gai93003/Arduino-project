/*
-------------------------------------------------------
Mini Pedestrian Crossing System
-------------------------------------------------------

This program simulates a pedestrian crossing.

Initial State
-------------
STOP
Red LED ON
OLED displays "STOP"

When Button is Pressed
----------------------
1. Countdown begins
2. System switches to GO state
3. Buzzer alerts pedestrians
4. Crossing time allowed
5. System returns to STOP

Optional Extensions (currently commented out)
---------------------------------------------
1. Countdown display during crossing phase
2. Flashing LED / buzzer warning before returning to STOP

These sections are commented out because they will be
implemented by another team member.
-------------------------------------------------------
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Hardware pins
#define RED_LED 8
#define BUTTON 2
#define BUZZER 9

// Timing values in seconds
int waitTime = 5;
int crossTime = 5;

void setup() {

  // Using INPUT_PULLUP prevents floating button signals
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(true);
  }

  display.clearDisplay();

  // Start system in STOP state
  showStop();
}

void loop() {

  // Button reads LOW when pressed
  if(digitalRead(BUTTON) == LOW) {

    startCountdown();

    crossingPhase();

    showStop();
  }
}

/*
-------------------------------------------------------
STOP STATE
-------------------------------------------------------
Displays STOP message and ensures
red LED is ON.
*/
void showStop(){

  digitalWrite(RED_LED, HIGH);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(20,20);
  display.println("STOP");

  display.display();
}


/*
-------------------------------------------------------
WAIT COUNTDOWN PHASE
-------------------------------------------------------
Pedestrians press button and must wait
before crossing is allowed.
*/
void startCountdown(){

  digitalWrite(RED_LED, HIGH);

  for(int i = waitTime; i > 0; i--){

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(10,10);
    display.print("Wait...");

    display.setTextSize(2);
    display.setCursor(50,30);
    display.println(i);

    display.display();

    delay(1000);
  }
}


/*
-------------------------------------------------------
CROSSING PHASE
-------------------------------------------------------
Pedestrians are allowed to cross.
*/
void crossingPhase(){

  digitalWrite(RED_LED, LOW);

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(30,20);
  display.println("GO");

  display.display();

  // Buzzer alert when crossing begins
  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(BUZZER, LOW);

  delay(crossTime * 1000);


  /*
  ---------------------------------------------------
  OPTIONAL EXTENSION (COMMENTED OUT)
  ---------------------------------------------------

  This extension will:

  1. Show countdown during crossing phase
  2. Flash LED and buzzer before STOP state

  Another team member will implement this section.
  To enable it, remove the comment markers.
  */

  /*
  for(int i = crossTime; i > 0; i--){

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0,10);
    display.print("Cross Now");

    display.setTextSize(2);
    display.setCursor(50,30);
    display.println(i);

    display.display();

    delay(1000);
  }

  // Warning phase before returning to STOP
  for(int i = 0; i < 3; i++){

    digitalWrite(BUZZER, HIGH);
    digitalWrite(RED_LED, HIGH);
    delay(200);

    digitalWrite(BUZZER, LOW);
    digitalWrite(RED_LED, LOW);
    delay(200);
  }
  */

}