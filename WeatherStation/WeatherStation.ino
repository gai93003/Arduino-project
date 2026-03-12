/*
-------------------------------------------------------
Mini Weather Station
-------------------------------------------------------

This program reads environmental data from sensors and
displays the results on an OLED screen.

Sensors Used
------------
DHT11  -> Temperature and Humidity
BMP280 -> Air Pressure

Outputs
-------
OLED display -> Shows sensor readings
Buzzer       -> Sounds when temperature exceeds threshold

Extra Feature
-------------
Temperature alert threshold can be adjusted using a
potentiometer.

Libraries
---------
Wire.h              -> Handles I2C communication
Adafruit_GFX.h      -> Graphics library for OLED
Adafruit_SSD1306.h  -> OLED display driver
DHT.h               -> DHT11 sensor communication
Adafruit_BMP280.h   -> Air pressure sensor communication

-------------------------------------------------------
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>


// OLED screen resolution
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Creates an OLED display object using I2C communication
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// DHT11 sensor setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// Pressure sensor object
Adafruit_BMP280 bmp;


// Hardware pins
#define BUZZER 8
#define POT_PIN A0


// Variables used to store sensor data
float temperature;
float humidity;
float pressure;


// Variable storing user-adjustable alarm threshold
int thresholdTemp;



void setup() {

  /*
  Serial communication allows us to debug sensor
  values through the Serial Monitor if needed.
  */
  Serial.begin(9600);

  // Configure buzzer pin as output
  pinMode(BUZZER, OUTPUT);

  // Start the DHT11 sensor
  dht.begin();

  /*
  Initialize the BMP280 pressure sensor.

  0x76 is the I2C address of the sensor.
  Some modules use 0x77 instead.
  */
  bmp.begin(0x76);

  /*
  Initialize the OLED display.

  If initialization fails the program stops
  using an infinite loop.
  */
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(true);
  }

  display.clearDisplay();
}



void loop() {

  /*
  Read temperature and humidity from the DHT11 sensor.

  The sensor returns floating-point values.
  */
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();


  /*
  Read atmospheric pressure from BMP280.

  The sensor returns pressure in Pascals.
  Dividing by 100 converts it to hectopascals (hPa),
  which is the common unit used in weather reports.
  */
  pressure = bmp.readPressure() / 100.0;


  /*
  Read potentiometer value.

  analogRead returns values between 0 and 1023.

  The map() function converts this range to a
  practical temperature threshold range (20–40°C).
  */
  int potValue = analogRead(POT_PIN);
  thresholdTemp = map(potValue, 0, 1023, 20, 40);


  /*
  Alarm Logic
  -----------

  If the measured temperature exceeds the threshold
  set by the potentiometer, activate the buzzer.
  */
  if (temperature > thresholdTemp) {
    digitalWrite(BUZZER, HIGH);
  }
  else {
    digitalWrite(BUZZER, LOW);
  }


  /*
  OLED Display Section
  --------------------

  The display is cleared before printing new values
  to prevent overlapping text.
  */

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0,15);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0,30);
  display.print("Pressure:");
  display.print(pressure);
  display.println(" hPa");

  display.setCursor(0,45);
  display.print("Alert Temp:");
  display.print(thresholdTemp);
  display.println("C");

  /*
  display.display() sends all buffered drawing
  commands to the OLED screen.
  */
  display.display();


  // Wait before next reading
  delay(2000);
}