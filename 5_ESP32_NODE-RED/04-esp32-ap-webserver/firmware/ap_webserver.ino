/*
 * Proyecto 04: ESP32 en Modo SoftAP con Servidor Web HTTP Embebido
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * El ESP32 crea su propio Access Point y aloja un servidor web en el puerto 80.
 * Cualquier dispositivo conectado a la red local puede ingresar a http://192.168.4.1
 * para observar los signos vitales del paciente y activar/desactivar alarmas.
 */

#include "WiFi.h"
#include <WebServer.h>

// Credenciales del punto de acceso
const char* AP_SSID = "ESP32-Monitor-Web";
const char* AP_PASSWORD = "Salud2026";

// Instancia del servidor HTTP en el puerto estándar 80
WebServer server(80);

// Pin del indicador de alarma (LED en GPIO 2)
const int LED_ALARMA = 2;
bool estadoAlarma = false;

// Variables simuladas de signos vitales
int bpmActual = 75;
int spo2Actual = 98;
float tempActual = 36.7;

// Generador de la página web HTML5
String generarHTML() {
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>UPCH Telesalud - Monitor Embebido</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background: #0f172a; color: #f8fafc; margin: 0; padding: 20px; text-align: center; }";
  html += ".card { background: #1e293b; border-radius: 12px; padding: 24px; max-width: 450px; margin: 0 auto; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }";
  html += ".header { color: #38bdf8; font-size: 22px; font-weight: bold; margin-bottom: 20px; }";
  html += ".metric { background: #334155; padding: 12px; border-radius: 8px; margin: 10px 0; font-size: 18px; }";
  html += ".val { font-size: 26px; font-weight: bold; color: #4ade80; }";
  html += ".btn { display: inline-block; padding: 12px 24px; margin: 10px 5px; border-radius: 6px; text-decoration: none; font-weight: bold; color: white; }";
  html += ".btn-on { background: #ef4444; }";
  html += ".btn-off { background: #64748b; }";
  html += ".badge { display: inline-block; padding: 4px 12px; border-radius: 9999px; font-size: 14px; font-weight: bold; }";
  html += "</style></head><body>";
  
  html += "<div class='card'>";
  html += "<div class='header'>🏥 UPCH - Telesalud Rural</div>";
  html += "<p>Monitor Clínico Portátil en Modo SoftAP</p>";
  
  html += "<div class='metric'>Frecuencia Cardíaca: <div class='val'>" + String(bpmActual) + " <span style='font-size:16px;'>lpm</span></div></div>";
  html += "<div class='metric'>Saturación Oxígeno: <div class='val'>" + String(spo2Actual) + " <span style='font-size:16px;'>% SpO2</span></div></div>";
  html += "<div class='metric'>Temperatura Corporal: <div class='val'>" + String(tempActual, 1) + " <span style='font-size:16px;'>°C</span></div></div>";
  
  html += "<div style='margin-top: 20px;'>Estado Alarma: ";
  if (estadoAlarma) {
    html += "<span class='badge' style='background:#ef4444;'>ACTIVA 🚨</span>";
  } else {
    html += "<span class='badge' style='background:#22c55e;'>SILENCIADA ✅</span>";
  }
  html += "</div>";

  html += "<div style='margin-top: 25px;'>";
  html += "<a href='/alarma/on' class='btn btn-on'>Activar Alarma</a>";
  html += "<a href='/alarma/off' class='btn btn-off'>Silenciar</a>";
  html += "</div>";

  html += "<p style='margin-top: 25px; font-size: 12px; color: #94a3b8;'>Actualizar página para refrescar lecturas</p>";
  html += "</div></body></html>";
  return html;
}

// Manejador de la página principal
void handleRoot() {
  // Simular variación fisiológica leve
  bpmActual = 72 + random(-4, 5);
  spo2Actual = 97 + random(0, 3);
  server.send(200, "text/html", generarHTML());
}

// Manejador para encender alarma
void handleAlarmaOn() {
  estadoAlarma = true;
  digitalWrite(LED_ALARMA, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

// Manejador para apagar alarma
void handleAlarmaOff() {
  estadoAlarma = false;
  digitalWrite(LED_ALARMA, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

// Endpoint de API JSON crudo
void handleApiVitales() {
  String json = "{";
  json += "\"dispositivo\":\"ESP32-AP-SERVER\",";
  json += "\"bpm\":" + String(bpmActual) + ",";
  json += "\"spo2\":" + String(spo2Actual) + ",";
  json += "\"temp_c\":" + String(tempActual, 1) + ",";
  json += "\"alarma\":" + String(estadoAlarma ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_ALARMA, OUTPUT);
  digitalWrite(LED_ALARMA, LOW);

  // Iniciar SoftAP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println("==================================================");
  Serial.println(" Servidor Web Embebido de Telesalud UPCH Listo");
  Serial.print(" Conéctate a Wi-Fi: ");
  Serial.println(AP_SSID);
  Serial.print(" Navega a: http://");
  Serial.println(WiFi.softAPIP());
  Serial.println("==================================================");

  // Mapeo de rutas HTTP
  server.on("/", handleRoot);
  server.on("/alarma/on", handleAlarmaOn);
  server.on("/alarma/off", handleAlarmaOff);
  server.on("/api/vitales", handleApiVitales);

  // Iniciar servidor web
  server.begin();
}

void loop() {
  // Atender peticiones de clientes HTTP en cada ciclo
  server.handleClient();
}
