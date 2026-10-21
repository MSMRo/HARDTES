/*
 * Proyecto 03: ESP32 en Modo Punto de Acceso (SoftAP) Autónomo
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Este programa configura el ESP32 como un Access Point (AP) independiente,
 * permitiendo la conexión directa de tablets o computadoras de campo sin necesidad
 * de infraestructura de red hospitalaria o acceso a internet.
 */

#include "WiFi.h"

// Credenciales del Punto de Acceso generado por el equipo biomédico
const char* AP_SSID = "ESP32-UPCH-TeleSalud";
const char* AP_PASSWORD = "Biomedica2026"; // Mínimo 8 caracteres para WPA2

// Configuración opcional de canal y visibilidad
const int AP_CHANNEL = 1;
const int AP_HIDDEN = 0;       // 0: Visible, 1: Oculto
const int MAX_CLIENTES = 4;    // Limitar conexiones por seguridad médica

unsigned long tiempoPrevio = 0;
int clientesPrevios = -1;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Proyecto 03: Configuración de Punto de Acceso (SoftAP)");
  Serial.println("==================================================");

  // Iniciar el módulo en modo exclusivamente AP
  WiFi.mode(WIFI_AP);

  // Iniciar el SoftAP con SSID y clave WPA2
  bool iniciado = WiFi.softAP(AP_SSID, AP_PASSWORD, AP_CHANNEL, AP_HIDDEN, MAX_CLIENTES);

  if (iniciado) {
    Serial.println("[OK] Red SoftAP levantada exitosamente.");
    Serial.print(" > Nombre de Red (SSID): ");
    Serial.println(AP_SSID);
    Serial.print(" > Clave de Acceso:      ");
    Serial.println(AP_PASSWORD);
    Serial.print(" > IP del Dispositivo:   ");
    Serial.println(WiFi.softAPIP());
    Serial.print(" > Dirección MAC SoftAP: ");
    Serial.println(WiFi.softAPmacAddress());
  } else {
    Serial.println("[ERROR] Falla al iniciar el modo SoftAP.");
  }
}

void loop() {
  unsigned long actual = millis();

  // Verificar cada 1 segundo si hay cambios en los dispositivos conectados
  if (actual - tiempoPrevio >= 1000) {
    tiempoPrevio = actual;

    int clientesActuales = WiFi.softAPgetStationNum();

    if (clientesActuales != clientesPrevios) {
      clientesPrevios = clientesActuales;
      Serial.printf("[ESTADO DE RED] Dispositivos médicos/clientes conectados: %d\n", clientesActuales);
    }
  }
}
