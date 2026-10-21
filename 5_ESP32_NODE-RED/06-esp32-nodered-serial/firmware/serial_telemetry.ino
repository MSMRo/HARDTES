/*
 * Proyecto 06: Telemetría Bidireccional ESP32 <-> Node-RED por Puerto Serie
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Este programa emite constantemente tramas de telemetría de signos vitales en
 * formato JSON por el puerto serie USB, y simultáneamente procesa comandos
 * entrantes desde Node-RED para encender o apagar alarmas y ejecutar calibraciones.
 */

#include <ArduinoJson.h>

const int PIN_ALARMA = 2; // LED indicador de alarma física

// Variables de estado
bool estadoAlarma = false;
unsigned long ultimoEnvio = 0;
const unsigned long INTERVALO_ENVIO = 1000; // 1 muestra por segundo

// Variables biomédicas simuladas
int bpmBase = 72;
int spo2Base = 98;
float tempBase = 36.6;

void procesarComando(String jsonComando) {
  StaticJsonDocument<200> doc;
  DeserializationError err = deserializeJson(doc, jsonComando);

  if (err) {
    Serial.print("{\"error\":\"JSON invalido en comando\"}\n");
    return;
  }

  const char* comando = doc["comando"];
  if (comando != nullptr) {
    if (strcmp(comando, "ALARMA_ON") == 0) {
      estadoAlarma = true;
      digitalWrite(PIN_ALARMA, HIGH);
      Serial.println("{\"ack\":\"ALARMA_ACTIVADA\"}");
    } else if (strcmp(comando, "ALARMA_OFF") == 0) {
      estadoAlarma = false;
      digitalWrite(PIN_ALARMA, LOW);
      Serial.println("{\"ack\":\"ALARMA_DESACTIVADA\"}");
    } else if (strcmp(comando, "CALIBRAR") == 0) {
      bpmBase = 70;
      spo2Base = 99;
      Serial.println("{\"ack\":\"SENSORES_RECALIBRADOS\"}");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_ALARMA, OUTPUT);
  digitalWrite(PIN_ALARMA, LOW);

  // Mensaje de arranque
  Serial.println("{\"status\":\"ESP32_TELEMED_BOOT_OK\",\"baudrate\":115200}");
}

void loop() {
  // 1. Escuchar comandos entrantes desde Node-RED
  if (Serial.available() > 0) {
    String lineaEntrante = Serial.readStringUntil('\n');
    lineaEntrante.trim();
    if (lineaEntrante.length() > 0) {
      procesarComando(lineaEntrante);
    }
  }

  // 2. Enviar telemetría periódica no bloqueante
  unsigned long ahora = millis();
  if (ahora - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = ahora;

    int bpm = bpmBase + random(-3, 4);
    int spo2 = spo2Base + random(-1, 2);
    if (spo2 > 100) spo2 = 100;
    float temp = tempBase + (random(-2, 3) * 0.1);

    StaticJsonDocument<256> doc;
    doc["dispositivo"] = "ESP32-SERIAL-NODE";
    doc["uptime_ms"] = ahora;
    doc["bpm"] = bpm;
    doc["spo2"] = spo2;
    doc["temp"] = temp;
    doc["alarma_activa"] = estadoAlarma;

    // Serializar e imprimir terminando obligatoriamente con \n
    serializeJson(doc, Serial);
    Serial.println();
  }
}
