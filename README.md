# 🌫️ Air Quality Monitoring with ESP8266 + Firebase

Sistem ini memantau suhu, kelembaban, dan kualitas udara menggunakan sensor **DHT22** dan **MQ-135**, lalu menampilkan datanya di **LCD I2C** serta mengirimkan ke **Firebase Realtime Database** untuk monitoring online.

---

## 📦 Fitur

- 📡 Koneksi WiFi
- 🌡️ Pembacaan suhu dan kelembaban (DHT22)
- 🧪 Pembacaan kualitas udara (MQ-135)
- 📺 Tampilan data pada LCD I2C 20x4
- ☁️ Upload data ke Firebase Realtime Database
- 🔁 Update data otomatis setiap 5 detik

---

## 🛠️ Komponen

| Komponen     | Jumlah |
|--------------|--------|
| ESP8266 (NodeMCU) | 1 buah |
| DHT22 (atau DHT11) | 1 buah |
| MQ-135       | 1 buah |
| LCD I2C 20x4 (alamat: 0x27 / 0x3F) | 1 buah |
| Kabel jumper | secukupnya |

---

## 🧰 Instalasi Library

Pastikan kamu telah menginstal library berikut di Arduino IDE:

- [FirebaseESP8266](https://github.com/mobizt/Firebase-ESP8266)
- DHT sensor library by Adafruit
- LiquidCrystal_I2C by Frank de Brabander
- ESP8266 board packages (melalui Board Manager)

---

## ⚙️ Konfigurasi Awal

Edit bagian ini di file `main.ino`:

```cpp
#define WIFI_SSID     "masukkan_SSID_anda"
#define WIFI_PASSWORD "masukkan_PASSWORD_anda"

#define FIREBASE_HOST "masukkan_FIREBASE_HOST_anda"  // Tanpa https://
#define FIREBASE_AUTH "masukkan_FIREBASE_AUTH_anda"
