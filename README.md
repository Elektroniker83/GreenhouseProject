# 🌱 Smartes Minigewächshaus mit Temperaturregelung und Blynk

Dieses Projekt steuert ein kleines Gewächshaus mithilfe von Temperatursensoren, Servomotoren und einem PWM-gesteuerten Lüfter. Es nutzt die Blynk IoT-Plattform, um Temperaturdaten anzuzeigen und die Zieltemperatur per PC-Konsole und Smartphone-App einstellbar zu machen. Das System hält die Temperatur durch automatisches Öffnen von Lüftungsklappen (Servos) und Einschalten eines Lüfters im gewünschten Bereich.

---

## 📦 Komponentenliste

| Komponente         | Beschreibung |
|--------------------|--------------|
| Mikrocontroller     | MKR WiFi 1010 mit WiFiNINA |
| 2x DHT11 Sensoren   | Temperatur-/Luftfeuchtigkeitssensor |
| 2x Servomotoren     | Z. B. SG90 (Belüftungsklappenöffnung) |
| 1x Lüfter (PWM)     | Gesteuert über PWM (z. B. PC-Lüfter) |
| WLAN-Modul          | z. B. integriert im MKR WiFi 1010 |
| Externe Stromversorgung | Für Servos und Lüfter empfohlen |
| Breadboard, Kabel   | Für Verdrahtung |

---

## 🖥️ Voraussetzungen

### Software

- CLION (PlatformIO)
- Bibliotheken:
  - `Blynk` oder `BlynkLegacy`
  - `WiFiNINA`
  - `DHT sensor library` von Adafruit
  - `Servo`

---

## 🔌 Verkabelung

| Gerät            | Arduino-Pin |
|------------------|-------------|
| DHT11 #1         | D7          |
| DHT11 #2         | D6          |
| Servo links      | D5          |
| Servo rechts     | D9          |
| Lüfter (PWM)     | D2          |

> **Hinweis:** Gemeinsame Masse (GND) verwenden. Servos und Lüfter benötigen ggf. externe 5V.

---

## 🧪 Installation & Kompilierung

1. **CLiON starten**
2. **Board auswählen:** z. B. Arduino MKR WiFi 1010
3. **WLAN-Zugangsdaten eintragen:**

   ```cpp
   char ssid[] = "DEIN_WIFI";
   char pass[] = "DEIN_PASSWORT";
