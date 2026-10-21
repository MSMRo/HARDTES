/*
 * Proyecto 08: Integración Completa ESP32 <-> Node-RED mediante MQTT
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * El ESP32 se conecta a Wi-Fi y a un broker MQTT (broker.emqx.io).
 * - Publica telemetría médica en: upch/telesalud/paciente_01/vitales
 * - Se suscribe a comandos en:   upch/telesalud/paciente_01/comandos
 * - Configura LWT en:            upch/telesalud/paciente_01/estado
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Credenciales Wi-Fi (Actualizar con tu red)
const char* WIFI_SSID = "TU_WIFI_SSID";
const char* WIFI_PASS = "TU_WIFI_PASSWORD";

// Servidor Broker MQTT Público de Pruebas
const char* MQTT_BROKER = "broker.emqx.io";
const int MQTT_PORT = 1883;

// Definición de Tópicos
const char* TOPIC_VITALS   = "upch/telesalud/paciente_01/vitales";
const char* TOPIC_COMMANDS = "upch/telesalud/paciente_01/comandos";
const char* TOPIC_STATUS   = "upch/telesalud/paciente_01/estado";

// Hardware
const int PIN_ALARMA = 2; // LED de alarma en GPIO 2

// Clientes de red
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Temporización de telemetría (cada 3 segundos)
unsigned long ultimoEnvio = 0;
const unsigned long INTERVALO_ENVIO = 3000;

// Callback: Ejecutado al recibir un mensaje suscrito desde Node-RED
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("\n[MQTT] Mensaje recibido en tópico: ");
  Serial.println(topic);

  String mensajeStr;
  for (unsigned int i = 0; i < length; i++) {
    mensajeStr += (char)message[i];
  }
  Serial.print("[MQTT] Payload: ");
  Serial.println(mensajeStr);

  // Deserializar comando JSON
  StaticJsonDocument<200> doc;
  DeserializationError err = deserializeJson(doc, mensajeStr);
  if (!err) {
    const char* accion = doc["accion"];
    if (accion != nullptr) {
      if (strcmp(accion, "ACTIVAR_ALARMA") == 0) {
        digitalWrite(PIN_ALARMA, HIGH);
        Serial.println("[ACTUADOR] Alarma luminosa encendida por Node-RED");
      } else if (strcmp(accion, "SILENCIAR_ALARMA") == 0) {
        digitalWrite(PIN_ALARMA, LOW);
        Serial.println("[ACTUADOR] Alarma luminosa apagada por Node-RED");
      }
    }
  }
}

void conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.print("[WIFI] Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 25) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] Conectado. IP: " + WiFi.localIP().toString());
  }
}

void reconectarMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("[MQTT] Conectando al broker: ");
    Serial.println(MQTT_BROKER);

    // ID de cliente único basado en la MAC del ESP32
    String clientId = "ESP32-UPCH-" + String(random(0xffff), HEX);

    // Conectar con LWT (Last Will and Testament) para detectar desconexión de paciente
    if (mqttClient.connect(clientId.c_str(), TOPIC_STATUS, 1, true, "DESCONECTADO_INESPERADO")) {
      Serial.println("[MQTT] ¡Conectado exitosamente!");

      // Publicar presencia ONLINE con flag retain
      mqttClient.publish(TOPIC_STATUS, "CONECTADO_ONLINE", true);

      // Suscribirse al tópico de comandos
      mqttClient.subscribe(TOPIC_COMMANDS, 1);
      Serial.print("[MQTT] Suscrito a: ");
      Serial.println(TOPIC_COMMANDS);
    } else {
      Serial.print("[MQTT ERROR] Fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(". Reintentando en 3 segundos...");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_ALARMA, OUTPUT);
  digitalWrite(PIN_ALARMA, LOW);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Proyecto 08: Integración Completa IoMT con MQTT");
  Serial.println("==================================================");

  conectarWiFi();
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  if (!mqttClient.connected()) {
    reconectarMQTT();
  }

  // Mantener vivo el stack MQTT y escuchar mensajes entrantes
  mqttClient.loop();

  // Publicar telemetría biomédica periódica
  unsigned long ahora = millis();
  if (ahora - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = ahora;

    int bpm = 70 + random(-3, 8);
    int spo2 = 96 + random(0, 4);
    float temp = 36.4 + (random(0, 6) * 0.1);

    StaticJsonDocument<256> doc;
    doc["paciente_id"] = "PAC-2026-UPCH";
    doc["bpm"] = bpm;
    doc["spo2"] = spo2;
    doc["temp_c"] = temp;
    doc["uptime_s"] = ahora / 1000;

    char buffer[256];
    serializeJson(doc, buffer);

    Serial.print("[MQTT PUB] ");
    Serial.println(buffer);

    mqttClient.publish(TOPIC_VITALS, buffer);
  }
}
