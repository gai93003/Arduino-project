/*
-------------------------------------------------------
Smart Night-Light System
-------------------------------------------------------

This system automatically turns on an LED when
ambient light levels fall below a threshold.

Inputs
------
LDR (Light Dependent Resistor) -> Detects light level
Potentiometer                  -> User-adjustable threshold

Outputs
-------
LED        -> Turns ON in darkness
OLED       -> Displays light values and system state

Concept
-------
The Arduino continuously reads the light sensor
and compares it with the threshold value.

If light < threshold -> LED ON
If light >= threshold -> LED OFF

-------------------------------------------------------
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Pin definitions
#define LDR_PIN A0
#define POT_PIN A1
#define LED_PIN 9


// Variables storing analog sensor values
int lightValue;
int threshold;



void setup() {

  Serial.begin(9600);

  // LED must be configured as output
  pinMode(LED_PIN, OUTPUT);


  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(true);
  }

  display.clearDisplay();
}



void loop() {

  /*
  analogRead() converts analog voltage into
  a digital number between 0 and 1023.

  Higher value usually means more light depending
  on how the LDR circuit is wired.
  */
  lightValue = analogRead(LDR_PIN);


  /*
  The potentiometer controls the threshold value.

  This allows the user to adjust when the LED
  should activate.
  */
  threshold = analogRead(POT_PIN);


  /*
  Decision Logic
  --------------

  Compare the measured light level with the
  threshold chosen by the user.
  */

  if(lightValue < threshold) {

    // Dark environment → turn LED ON
    digitalWrite(LED_PIN, HIGH);

  }
  else {

    // Bright environment → turn LED OFF
    digitalWrite(LED_PIN, LOW);

  }


  /*
  OLED Display Feedback
  ---------------------

  Shows sensor values and LED state.
  */

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("Light Level: ");
  display.println(lightValue);

  display.setCursor(0,20);
  display.print("Threshold: ");
  display.println(threshold);

  display.setCursor(0,40);

  if(lightValue < threshold){
    display.println("LED: ON");
  }
  else{
    display.println("LED: OFF");
  }

  display.display();


  /*
  Small delay to prevent excessive display
  refreshing and reduce flickering.
  */
  delay(500);
}