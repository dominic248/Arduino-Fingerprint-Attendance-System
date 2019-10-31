/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial fingerprintsensor(2, 3);
SoftwareSerial Nodemcu(10,11);//RX,TX

int ledPin = 9;
int Buzzer = 8; 
Adafruit_SSD1306 display(-1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerprintsensor);



void setup()  
{
  Serial.begin(9600);
  Nodemcu.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  delay(2000);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("");
  display.display(); 
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
}

void displayscreen(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Please place your finger!");
  display.println("To get attendance");
  display.display(); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    displayscreen();
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // Display static text
    display.println("Invalid ID");
    display.println("Please try again!");
    display.display();
    
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // Display static text
    display.println("Invalid ID!");
    display.println("Please try again!");
    display.display();
    return -1;
  }

  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // Display static text
    display.println("Invalid ID");
    display.println("Please try again!");
    display.display();
    digitalWrite(Buzzer,HIGH); //activate buzzer
    delay(1000); 
    digitalWrite(Buzzer,LOW); //deactivate buzzer 
  Nodemcu.println(String(finger.fingerID)+","+String(finger.confidence)+"");
  
  return finger.fingerID; 
}
