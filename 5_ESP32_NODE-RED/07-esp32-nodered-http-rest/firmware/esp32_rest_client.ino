/*
 * Proyecto 07: Cliente ESP32 REST HTTP enviando telemetría a Servidor Node-RED
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * El ESP32 envía paquetes JSON mediante HTTP POST a un endpoint definido en Node-RED.
 * Tras la respuesta, lee el JSON devuelto por Node-RED para confirmar el registro.
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Credenciales de la red Wi-Fi
const char* WIFI_SSID = "TU_WIFI_SSID";
const char* WIFI_PASS = "TU_WIFI_PASS";

// Dirección del servidor Node-RED (Reemplazar con la IP de tu PC)
const char* SERVER_ENDPOINT = "http://192.168.1.100:1880/api/v1/telesalud/vitales";

// Intervalo de transmisión (5 segundos)
const unsigned long INTERVALO_ENVIO = 5000;
unsigned long ultimoEnvio = 0;

void verificarConexionWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("[WIFI] Conectando a red: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 30) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] Conectado exitosamente.");
    Serial.print("[WIFI] IP asignada: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[WIFI ERROR] Imposible conectar. Reintentará.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Proyecto 07: Integración ESP32 -> Node-RED (REST)");
  Serial.println("==================================================");

  verificarConexionWiFi();
}

void loop() {
  verificarConexionWiFi();
  if (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    return;
  }

  unsigned long ahora = millis();
  if (ahora - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = ahora;

    // 1. Simulación de constantes vitales
    int bpm = 72 + random(-4, 6);
    int spo2 = 97 + random(0, 3);
    float temp = 36.5 + (random(-1, 4) * 0.1);
    int bateria = 90 + random(-5, 5);

    // 2. Serializar JSON con ArduinoJson
    StaticJsonDocument<256> reqDoc;
    reqDoc["device_mac"] = WiFi.macAddress();
    reqDoc["paciente_id"] = "PAC-2026-UPCH";
    reqDoc["frecuencia_cardiaca"] = bpm;
    reqDoc["spo2"] = spo2;
    reqDoc["temperatura_c"] = temp;
    reqDoc["bateria_pct"] = bateria;

    String jsonPayload;
    serializeJson(reqDoc, jsonPayload);

    Serial.println("\n--------------------------------------------------");
    Serial.println("[REST] Enviando datos a Node-RED:");
    Serial.println(jsonPayload);

    // 3. Petición HTTP POST
    HTTPClient http;
    http.begin(SERVER_ENDPOINT);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    // 4. Procesar respuesta del servidor Node-RED
    if (httpResponseCode > 0) {
      Serial.printf("[REST] Código HTTP recibido: %d\n", httpResponseCode);
      String responseBody = http.getString();
      Serial.print("[REST] Respuesta del servidor Node-RED: ");
      Serial.println(responseBody);

      // Deserializar respuesta del servidor
      StaticJsonDocument<200> resDoc;
      DeserializationError err = deserializeJson(resDoc, responseBody);
      if (!err) {
        const char* estado = resDoc["estado"];
        const char* alerta = resDoc["alerta_clinica"];
        Serial.printf("[DIAGNÓSTICO NUBE] Estado: %s | Alerta: %s\n", estado, alerta);
      }
    } else {
      Serial.printf("[REST ERROR] Fallo al conectar con Node-RED: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  }
}
