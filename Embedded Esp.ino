#include "LiquidCrystal_I2C.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "ElZawawy";
const char* password = "00035459070";
#define BOTtoken "1888009873:AAG38SGCB7_aVr7aMGq4pmlQT5TOTPTAmCY"  // your Bot Token (Get from Botfather)
#define CHAT_ID "1079889164"

// Define Trig and Echo pins:
#define trigPin1 2
#define echoPin1 4
#define trigPin2 18
#define echoPin2 19
#define LEDpin 25
#define SWpin 34

// Define variables:
long duration;
int distance1;
int distance2;
int sw;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void getDistance1();
void getDistance2();


void setup() {

  Serial.begin(9600);

  Serial.println("");
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Bot started up", "");


  // put your setup code here, to run once:



  lcd.init();
  lcd.backlight();

  pinMode(SWpin, INPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);


}

void loop() {

  // put your main code here, to run repeatedly:
  int people = 0;
  while (1) {

    sw = digitalRead(SWpin);

    getDistance1();

    getDistance2();
    lcd.clear();
    lcd.setCursor(0, 0);

    if (sw == 1)
      lcd.print("Secure Mode");
    else if (sw == 0 ) {
      lcd.print("Normal Mode");
      lcd.setCursor(0, 1);
      lcd.print(people);

    }
    if (sw == 0) {

      if (distance1 <= 20 && distance2 > 20) {

        while (distance2 > 20)
          getDistance2();

        people++;
        digitalWrite(LEDpin, HIGH);
      }

      else if (distance1 > 20 && distance2 <= 20 && people > 0) {

        while (distance1 > 20)
          getDistance1();

        people--;
        if (people == 0 )
        {
          digitalWrite(LEDpin, LOW);
        }
      }

      while (distance2 <= 20 || distance1 <= 20) {
        getDistance1();
        getDistance2();
      }
    }
    else if (sw == 1 && ( distance2 <= 20 || distance1 <= 20) ) {
      people = 0;
      digitalWrite(LEDpin, LOW);
      bot.sendMessage(CHAT_ID, "Motion detected!!", "");
      while (distance2 <= 20 || distance1 <= 20) {
        getDistance1();
        getDistance2();
      }
    }


    Serial.print("Distance1 = ");
    Serial.print(distance1);
    Serial.println(" cm");

    Serial.print("Distance2 = ");
    Serial.print(distance2);
    Serial.println(" cm");

    Serial.print("switch = ");
    Serial.println(sw);

    Serial.print("people = ");
    Serial.println(people);

    delay(1000);

  }




}

void getDistance1() {


  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin1, HIGH);
  // Calculate the distance:
  distance1 = duration * 0.034 / 2;

}

void getDistance2() {

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin2, HIGH);
  // Calculate the distance:
  distance2 = duration * 0.034 / 2;

}
