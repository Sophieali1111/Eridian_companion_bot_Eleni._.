//==============================================================
//Project : Eleni - Eridian COmpanion robot 
//Author : sophia 
//Date : 13/06/2026

//what eleni does:
// moves around makes cute faces and outputs info from screen such as temp, humidity and maybe other things depending on how well the bluetooth and wifi part work 
// components: 
// - esp32 DEV board
// - OLED 
// - DHT 22 temp/ humidity sensor 
// - 10*9g servos 
// - Lipo batter + TP4056 charger + step up 
// - captive touchsensor 
// refrences 
//   Wiring inspiration:
//     https://github.com/EDISON-SCIENCE-CORNER/DESKBUDDY-1.0
//   Design inspiration:
//     https://www.youtube.com/watch?v=1UDsWkcQZhc
//   OLED eye animation reference:
//     https://github.com/playfultechnology/esp32-eyes
//   DHT22 library:
//     https://github.com/adafruit/dht-sensor-library
//   Servo library:
//     https://docs.arduino.cc/learn/built-in-libraries/software-serial/
//   ESP32 board package:
//     https://raw.githubusercontent.com/espressif/arduino-esp32/...
//   Adafruit SSD1306 library (via Arduino Library Manager)
//   ESP32 pinout reference:
//     https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
//   Project inspiration — Project Hail Mary (Andy Weir):
//     Rocky the Eridian companion
// ================================================================
// --LIBARIES--
#include <Wire.h>                // I2C 
#include <Adafruit_GFX.h>        // Graphics basic 
#include <Adafruit_SSD1306.h>    // Graphics oled
#include <DHT.h>                 // humidity sensor 
#include <ESP32Servo.h>          // servos
#include <WiFi.h>                // connect to wifi
#include <WebServer.h>           // servo
// --- Pin Definitions ---

// Touch sensor
#define TOUCH_PIN    T0   // GPIO4 - capacitive touch on head

// DHT22
#define DHT_PIN      15   // GPIO15 - temp/humidity data wire
#define DHT_TYPE     DHT22

// OLED
#define OLED_WIDTH   128
#define OLED_HEIGHT  64
#define OLED_ADDR    0x3C // fixed I2C address for SSD1306

// Servos - 5 legs x 2 servos each = 10 total
int SERVO_PINS[10] = {
  32,  4,   // Leg 1 - servos 1, 2
  16, 17,   // Leg 2 - servos 3, 4
  18, 19,   // Leg 3 - servos 5, 6
  25, 26,   // Leg 4 - servos 7, 8
  27, 23    // Leg 5 - servos 9, 10
};

// --- Objects ---

Adafruit_SSD1306 display(128, 64, &Wire, -1);// oled wide + lengh

DHT dht(15, DHT22);// sensor 

Servo servos[10];// def amout of servos legs 
// --- Variables ---
float temperature = 0.0;   // current temperature from DHT22
float humidity = 0.0;      // current humidity from DHT22
bool isWalking = false;      // is Eleni currently walking
unsigned long lastSensorTime = 0;  // last time sensor was read
unsigned long lastBlinkTime = 0;   // last time Eleni blinked
unsigned long lastTouchTime = 0;   // last time head was patted
void setup() {
  Serial.begin(115200); // starts communication with your computer for debugging

  // 1. Initialise OLED
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // 2. Start DHT22
  dht.begin();

  // 3. Attach all 10 servos
  for (int i = 0; i < 10; i++) {      
    servos[i].attach(SERVO_PINS[i]);
  }

  // 4. Sleep position - knees spread out
  for (int i = 0; i < 10; i++) {
    servos[i].write(130);               // what angle for spread knees?
  }
  delay(2000);                           

  // 5. Stand up - neutral position
  for (int i = 0; i < 10; i++) {
    servos[i].write(90);               // what angle for neutral standing?
  }

  // 6. Happy wake up face
  // [we will fill this in when we do the face functions]

  Serial.println("Eleni is awake!");
    // 7. Show temperature on OLED for 10 seconds
  display.clearDisplay();
  display.setTextSize(2);          // text size 2 = medium
  display.setCursor(0, 0);         // start at top left
  display.print("Temp:");
  display.print(temperature);              // what variable holds temperature?
  display.print("C");
  display.display();
  delay(10000);                      // how many milliseconds is 10 seconds?

  // 8. Show humidity for 10 seconds
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Humidity:"); 
  display.print(humidity);p
  display.print("%");
  display.display();
  delay(10000);                      // same as above
}

void loop() {
  
  server.handleClient();  //checks if handled client

  checkTouch();           // checks touch sensor if activated
  
  readSensors();          // checks sensors 
  
  updateDisplay();        // updated the disply 

  delay(20);              // small pause so ESP32 doesn't run too fast and crash
 void checkTouch() {
  
  // check if touch sensor is activated
  if (touchRead(TOUCH_PIN) < 40) {
    
    // 1. Happy face
    // [we will fill this in with face functions later]

    // 2. Dog bow stretch - front two legs forward and down
    servos[0].write(30);  // leg 1 hip - forward
    servos[1].write(60);  // leg 1 knee - down
    servos[2].write(30);  // leg 2 hip - forward
    servos[3].write(60);  // leg 2 knee - down
    delay(3000);            // hold the stretch - how long?
    
    // 3. Back to standing
    for (int i = 0; i < 10; i++) {
      servos[i].write(90); // neutral standing
    }

    // 4. Show time on OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("Time:");
    display.display();

    // update last touch time
    lastTouchTime = millis();
  }

  // if no headpat for 30 seconds → paranoid mode
  if (millis() - lastTouchTime > 30000) {  // 30 seconds in milliseconds?
    // [paranoid face + walk in circles]
  }

void readSensors() {

  // only read every 3 seconds so we dont overload the sensor
  if (millis() - lastSensorTime > 3000) {
    
    temperature = dht.readTemperature(); // read temp in celsius
    humidity = dht.readHumidity();       // read humidity %

    // update the timer
    lastSensorTime = millis(); // millis() gives current time in milliseconds

    // print to serial monitor for debugging
    Serial.print("Temp: ");
    Serial.println(temperature);   // which variable?
    Serial.print("Humidity: ");
    Serial.println(humidity);   // which variable?
  }
}
}
void updateDisplay() {

  if (temperature > 30) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("Too hot!");
    display.print(temperature);
    display.print("C");
    display.display();
    // [hot face - fill in with face functions later]

  } else if (temperature < 10) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("Brrr!");
    display.print(temperature);
    display.print("C");
    display.display();
    // [cold face - fill in with face functions later]

  } else {
    // [normal face - fill in with face functions later]
  }
}
void walkForward(int steps) {
  isWalking = true;

  for (int s = 0; s < steps; s++) {
    // Phase 1 - front group lifts and swings forward
    servos[0].write(110); // leg 1 hip forward
    servos[1].write(65);  // leg 1 knee lift
    servos[4].write(110); // leg 3 hip forward
    servos[5].write(65);  // leg 3 knee lift
    servos[8].write(110); // leg 5 hip forward
    servos[9].write(65);  // leg 5 knee lift
    delay(120);

    // Phase 2 - front group lands
    servos[0].write(110);
    servos[1].write(90);
    servos[4].write(110);
    servos[5].write(90);
    servos[8].write(110);
    servos[9].write(90);
    delay(60);

    // Phase 3 - back group lifts and swings
    servos[2].write(110);
    servos[3].write(65);
    servos[6].write(110);
    servos[7].write(65);
    delay(120);

    // Phase 4 - back group lands
    servos[2].write(110);
    servos[3].write(90);
    servos[6].write(110);
    servos[7].write(90);
    delay(60);
  }

  // back to standing
  for (int i = 0; i < 10; i++) {
    servos[i].write(90);
  }

  isWalking = false;
}
}
