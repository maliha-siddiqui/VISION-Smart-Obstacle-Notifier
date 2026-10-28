#define BLYNK_TEMPLATE_ID "TMPL3Pnrgd17k"
#define BLYNK_TEMPLATE_NAME "Obstacle Detector"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
#define DISTANCE_VIRTUAL_PIN V1

char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// Pin definitions
const int trigPin = 5;   // D1
const int echoPin = 4;   // D2
const int alertPin = 15; // D8 (buzzer)

// Variables
long duration;
long distanceCm;
const int ALERT_THRESHOLD = 200;
bool isAlertActive = false;
bool pulseState = false;
unsigned long lastPulseTime = 0;

// Speed detection
float previousDistance = 0;
unsigned long previousTime = 0;
const float SPEED_THRESHOLD = 25.0; // cm/sec

// Wi-Fi connection tracking
unsigned long wifiCheckTimer = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000;
bool wifiConnected = false;

// --------------------------------------------------
// 🔔 Buzzer pattern helpers
void beepShort() {
  digitalWrite(alertPin, HIGH);
  delay(150);
  digitalWrite(alertPin, LOW);
  delay(150);
}

void beepLong() {
  digitalWrite(alertPin, HIGH);
  delay(600);
  digitalWrite(alertPin, LOW);
}

void beepOfflinePattern() {
  for (int i = 0; i < 3; i++) beepShort();
}

void beepOnlinePattern() {
  beepLong();
}

void beepFastDanger() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(alertPin, HIGH);
    delay(100);
    digitalWrite(alertPin, LOW);
    delay(100);
  }
}
// --------------------------------------------------

// 🧠 Ultrasonic distance + speed measurement
void readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  distanceCm = duration / 58;
  delay(80);

  if (distanceCm == 0 || distanceCm > ALERT_THRESHOLD) {
    if (wifiConnected) {
      Blynk.virtualWrite(DISTANCE_VIRTUAL_PIN, "No Object Detected");
    }
    previousDistance = distanceCm;
    previousTime = millis();
    isAlertActive = false;
    return;
  }

  unsigned long currentTime = millis();
  float distanceChange = previousDistance - distanceCm;
  unsigned long timeDiff = currentTime - previousTime;
  previousTime = currentTime;

  float speed = 0;
  if (timeDiff > 0) {
    speed = (distanceChange / timeDiff) * 1000.0;
    if (abs(speed) < 1.0) speed = 0;
  }

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm | Speed: ");
  Serial.print(speed);
  Serial.println(" cm/s");

  if (wifiConnected) {
    Blynk.virtualWrite(DISTANCE_VIRTUAL_PIN,
      "Dist: " + String(distanceCm) + " cm | Spd: " + String(speed, 1) + " cm/s");
  }

  if (speed > SPEED_THRESHOLD && distanceCm < ALERT_THRESHOLD) {
    Serial.println("⚡ Fast approaching object detected!");
    beepFastDanger();

    if (wifiConnected && !isAlertActive) {
      String message = "⚡ Fast approaching object! Speed: " + String(speed, 1) + " cm/s";
      Blynk.logEvent("obstacle_alert", message);
      isAlertActive = true;
    }
  }
  else if (distanceCm <= ALERT_THRESHOLD) {
    int pulseDelay = map(distanceCm, 0, ALERT_THRESHOLD, 100, 400);
    if (millis() - lastPulseTime >= pulseDelay) {
      pulseState = !pulseState;
      digitalWrite(alertPin, pulseState);
      lastPulseTime = millis();
    }
  }

  previousDistance = distanceCm;
}

// 🌐 Wi-Fi connection checker with auto reconnect
void checkWiFiConnection() {
  if (millis() - wifiCheckTimer >= WIFI_CHECK_INTERVAL) {
    wifiCheckTimer = millis();

    if (WiFi.status() != WL_CONNECTED) {
      if (wifiConnected) {
        Serial.println("⚠ WiFi Lost! Switching to offline mode...");
        wifiConnected = false;
        beepOfflinePattern();
      } else {
        Serial.println("🔄 Attempting WiFi reconnection...");
        WiFi.begin(ssid, pass);
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000) {
          delay(500);
          Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED) {
          Serial.println("\n✅ WiFi Reconnected! Back to online mode.");
          wifiConnected = true;
          Blynk.config(auth);
          Blynk.connect();
          beepOnlinePattern();
        }
      }
    }
  }
}

// --------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(alertPin, OUTPUT);
  digitalWrite(alertPin, LOW);

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.begin(ssid, pass);
  Serial.println("🔄 Connecting to WiFi...");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) {
    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    wifiConnected = true;
    Blynk.config(auth);
    Blynk.connect();
    beepOnlinePattern();
  } else {
    Serial.println("\n❌ WiFi Not Connected — Running in Offline Mode.");
    wifiConnected = false;
    beepOfflinePattern();
  }

  timer.setInterval(300L, readUltrasonic);
}

void loop() {
  checkWiFiConnection();

  if (wifiConnected) {
    Blynk.run();
  }

  timer.run();
}
