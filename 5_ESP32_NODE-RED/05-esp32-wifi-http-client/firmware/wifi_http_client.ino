/*
 * Proyecto 05: ESP32 en Modo Estación (STA) y Cliente HTTP REST
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * El ESP32 se conecta a la red Wi-Fi local y envía periódicamente un paquete
 * de constantes vitales serializado en JSON mediante una petición HTTP POST
 * hacia el servidor de telemetría.
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Credenciales Wi-Fi (Actualizar con tu red de laboratorio o casa)
const char* WIFI_SSID = "TU_WIFI_SSID";
const char* WIFI_PASSWORD = "TU_WIFI_PASSWORD";

// URL del servidor de destino (Node-RED o backend REST)
// Reemplazar con la IP local de la computadora donde corre Node-RED
const char* SERVER_URL = "http://192.168.1.100:1880/api/telemetria";

// Intervalo de transmisión (10 segundos)
const unsigned long INTERVALO_ENVIO = 10000;
unsigned long ultimoEnvio = 0;

void conectarWiFi() {
  Serial.print("[WIFI] Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 25) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ¡Conectado exitosamente!");
    Serial.print("[WIFI] Dirección IP asignada: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WIFI] Potencia RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n[WIFI] Falla de conexión. Reintentando en el próximo ciclo...");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Proyecto 05: Cliente HTTP REST con ArduinoJson");
  Serial.println("==================================================");

  conectarWiFi();
}

void loop() {
  // Verificar si la conexión Wi-Fi se mantiene activa
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
    return;
  }

  unsigned long ahora = millis();
  if (ahora - ultimoEnvio >= INTERVALO_ENVIO || ultimoEnvio == 0) {
    ultimoEnvio = ahora;

    // 1. Simulación de adquisición de sensores biomédicos
    int bpm = 70 + random(-5, 8);
    int spo2 = 96 + random(0, 4);
    int bateriaMv = 3800 + random(-100, 100);

    // 2. Construcción del JSON con ArduinoJson
    StaticJsonDocument<256> doc;
    doc["nodo_id"] = "ESP32-UPCH-01";
    doc["paciente"] = "PAC-2026-A";
    doc["frecuencia_cardiaca"] = bpm;
    doc["spo2"] = spo2;
    doc["bateria_mv"] = bateriaMv;
    doc["electrodos_conectados"] = true;

    String jsonString;
    serializeJson(doc, jsonString);

    Serial.println("\n[HTTP] Preparando envío de paquete biomédico:");
    Serial.println(jsonString);

    // 3. Inicialización del cliente HTTP
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    // 4. Ejecución del POST
    int httpCode = http.POST(jsonString);

    // 5. Evaluación de respuesta del servidor
    if (httpCode > 0) {
      Serial.printf("[HTTP] Código de respuesta: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
        String payloadRespuesta = http.getString();
        Serial.print("[HTTP] Respuesta servidor: ");
        Serial.println(payloadRespuesta);
      }
    } else {
      Serial.printf("[HTTP ERROR] Falla en la petición: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end(); // Liberar recursos de conexión
  }
}
