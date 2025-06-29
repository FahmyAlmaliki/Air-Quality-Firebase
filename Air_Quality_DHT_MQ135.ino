/*
  ==========================================
  🌫️ Air Quality Monitoring System (ESP8266)
  ==========================================
  Author     : FahmyAlmaliki
  Hardware   : NodeMCU ESP8266, DHT22, MQ-135, LCD I2C
  Features   :
    - Monitoring suhu, kelembaban, dan kualitas udara
    - Display data via LCD I2C
    - Pengiriman data ke Firebase Realtime Database
  ==========================================
*/

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ========== Konfigurasi ==========
#define WIFI_SSID     "masukkan_SSID_anda"
#define WIFI_PASSWORD "masukkan_PASSWORD_anda"

#define FIREBASE_HOST "masukkan_FIREBASE_HOST_anda"  // Contoh: your-project-id.firebaseio.com
#define FIREBASE_AUTH "masukkan_FIREBASE_AUTH_anda"

#define DHTPIN   D5
#define DHTTYPE  DHT22
#define MQ135_PIN A0
// ================================

// Inisialisasi objek
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();

  // Koneksi WiFi
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  Serial.print("Connecting to WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  Serial.println("\n✅ WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Firebase setup
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  lcd.setCursor(0, 1);
  if (Firebase.ready()) {
    lcd.print("Firebase: OK");
    Serial.println("✅ Firebase ready");
  } else {
    lcd.print("Firebase: FAIL");
    Serial.println("❌ Firebase connection failed");
  }

  delay(2000);
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();
  int kualitas_udara = analogRead(MQ135_PIN);

  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("❌ Gagal membaca DHT!");
    lcd.clear();
    lcd.print("DHT Error!");
    delay(1000);
    return;
  }

  // Tampilkan ke Serial
  Serial.printf("Suhu: %.2f C | Kelembaban: %.2f %% | MQ-135: %d\n", suhu, kelembaban, kualitas_udara);

  // Tampilkan ke LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("S:%.1fC H:%.1f%%", suhu, kelembaban);
  lcd.setCursor(0, 1);
  lcd.print("Udara: ");
  lcd.print(kualitas_udara);

  // Kirim ke Firebase
  if (Firebase.ready()) {
    if (!Firebase.setFloat(firebaseData, "/Data_Sensor/suhu", suhu)) {
      Serial.println("Gagal kirim suhu: " + firebaseData.errorReason());
    }

    if (!Firebase.setFloat(firebaseData, "/Data_Sensor/kelembaban", kelembaban)) {
      Serial.println("Gagal kirim kelembaban: " + firebaseData.errorReason());
    }

    if (!Firebase.setInt(firebaseData, "/Data_Sensor/kualitas_udara", kualitas_udara)) {
      Serial.println("Gagal kirim kualitas udara: " + firebaseData.errorReason());
    }
  } else {
    Serial.println("❌ Firebase tidak terhubung");
  }

  delay(5000); // Kirim setiap 5 detik
}
