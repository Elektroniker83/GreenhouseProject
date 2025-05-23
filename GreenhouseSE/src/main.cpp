// Einbindung der Arduino-Bibliothek
#include <Arduino.h>

// Blynk-Projektkonfiguration (Template-ID, Name, Auth-Token) für Blynk IoT App und Konsole
#define BLYNK_TEMPLATE_ID "TMPL4njukVQyG"
#define BLYNK_TEMPLATE_NAME "GreenhouseSE"
#define BLYNK_AUTH_TOKEN "7I-0DjUqby9pxDic7TQcgwbFnkKapOkB"

// Einbindung Bibliotheken für WLAN, Blynk, DHT-Sensoren und Servos
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <DHT.h>
#include <Servo.h>

// WLAN-Zugangsdaten (ändern bei anderem WiFi-Netz)
char ssid[] = "UPC6669241";
char pass[] = "kt8nczNGdbrn";

// Pinbelegung für die beiden DHT11 Temperatursensoren definieren
#define DHTPIN1 7
#define DHTPIN2 6
#define DHTTYPE DHT11

// Pinbelegung für Servos und Lüfter definieren
#define SERVO_LEFT_PIN 5
#define SERVO_RIGHT_PIN 9
#define FAN_PWM_PIN 2

// Initialisierung der DHT11 Temperatursensoren
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);


// Initialisierung der Servo-Motoren
Servo servoLeft;
Servo servoRight;

// Blynk-Timer für regelmäßige Aufgaben
BlynkTimer timer;

// Anfangswerte initialisieren
uint8_t fanPWMValue = 0;        // PWM Startwert Lüfter
float targetTemp = 25.0;        // Solltemperatur in °C
const float hysterese = 2.0;    // Hysterese für Temperatursteuerung

// Callback: Empfängt Solltemperatur von Blynk App (Virtueller Pin V4)
BLYNK_WRITE(V4) {
        targetTemp = param.asFloat();       // Neue Solltemperatur auslesen
        Serial.print("Neue Solltemperatur: ");
        Serial.println(targetTemp);
        Blynk.virtualWrite(V4, targetTemp); // Bestätigung an App zurücksenden
}

// Funktion zum Auslesen der Temperaturensoren und Steuerung der Servos
void sendSensorData() {
    float temp1 = dht1.readTemperature();
    float temp2 = dht2.readTemperature();

    // Fehlerbehandlung beim Lesen der Sensoren
    if (isnan(temp1) || isnan(temp2)) {
        Serial.println("Fehler beim Lesen von DHT11");
        return;
    }

    // Ausgabe der aktuellen Temperaturen auf Terminal
    Serial.print("Temp1: ");
    Serial.print(temp1);
    Serial.print(" °C | Temp2: ");
    Serial.print(temp2);
    Serial.print(" °C | Soll: ");
    Serial.print(targetTemp);
    Serial.println(" °C");

    // Senden der Temperatursensordaten an Blynk App
    Blynk.virtualWrite(V0, temp1);
    Blynk.virtualWrite(V1, temp2);

    // Steuerungslogik für Lüfter und Servo
    int servoAngle = 0;
    if (temp1 > targetTemp + 8) {
        fanPWMValue = 204;  // 80 % PWM (Lüftergeschwindigkeit)
        servoAngle = 90;    //Belüftungsklappen ganz offen
        Serial.println("‼️ Hohe Temperatur: 80 % PWM");
    }
    else if (temp1 > targetTemp + hysterese) {
        fanPWMValue = 128;  // 50 % PWM (Lüftergeschwindigkeit)
        servoAngle = 90;    //Belüftungsklappen ganz offen
        Serial.println("⚠️ Moderat: 50 % PWM");
    }
    else {
        fanPWMValue = 0;    // 0% PWM (Lüfter aus)
        servoAngle = 0;     // Belüftungsklappen schliessen
        Serial.println("✅ Temperatur OK: Luefter AUS");
    }

    // Servos auf den berechneten Winkel setzen
    servoLeft.write(servoAngle);
    servoRight.write(servoAngle);

    // PWM-Wert für Lüfter setzen
    analogWrite(FAN_PWM_PIN, fanPWMValue);

    // Werte an Blynk senden
    Blynk.virtualWrite(V3, fanPWMValue);         // Lüfter PWM
    Blynk.virtualWrite(V5, servoRight.read());   // Rechter Servo Winkel
    Blynk.virtualWrite(V6, servoLeft.read());    // Linker Servo Winkel

    // Ausgabe des PWM Wertes ses Lüfters
    Serial.print("Lüfter PWM: ");
    Serial.println(fanPWMValue);
}

// Initialisierung
void setup() {
    Serial.begin(115200);   // Serielle Schnittstelle starten
    delay(1000);

    pinMode(FAN_PWM_PIN, OUTPUT);
    analogWrite(FAN_PWM_PIN, 0);    // Lüfter ausschalten

    servoLeft.attach(SERVO_LEFT_PIN);
    servoRight.attach(SERVO_RIGHT_PIN);

    servoLeft.write(0);         //Belüftungsklappe links schliessen
    servoRight.write(0);        //Belüftungsklappe rechts schliessen

    // Blynk starten (mit WLAN und Token)
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    dht1.begin();
    dht2.begin();

    // Alle 5 Sekunden Sensordaten senden
    timer.setInterval(5000L, sendSensorData);

    // Initialwerte von Lüfter (PWM) und Solltemperatur an App senden
    Blynk.virtualWrite(V3, fanPWMValue);
    Blynk.virtualWrite(V4, targetTemp);
}

// Programmhauptschleife
void loop() {
    Blynk.run();    // Blynk-Dienste verarbeiten
    timer.run();    // Timer ausführen
}