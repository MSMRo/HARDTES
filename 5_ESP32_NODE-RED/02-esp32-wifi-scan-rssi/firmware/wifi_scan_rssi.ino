/*
 * Proyecto 02: Escaneo de Redes Wi-Fi y Análisis de Calidad de Señal (RSSI)
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Este programa realiza un barrido de las redes inalámbricas disponibles en 2.4 GHz,
 * mide el nivel de potencia recibida (RSSI en dBm) y genera un reporte estructurado
 * para verificar si el entorno hospitalario o domiciliario es apto para telemetría continua.
 */

#include "WiFi.h"

// Intervalo entre escaneos de red (10 segundos)
const unsigned long INTERVALO_ESCANEO = 10000;
unsigned long previoEscaneo = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Proyecto 02: Scanner Wi-Fi y Auditoría RSSI");
  Serial.println("==================================================");

  // Configurar Wi-Fi en modo Estación y desconectar de cualquier AP previo
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Módulo RF inicializado en modo escucha.");
}

void loop() {
  unsigned long ahora = millis();

  if (ahora - previoEscaneo >= INTERVALO_ESCANEO || previoEscaneo == 0) {
    previoEscaneo = ahora;

    Serial.println("\n[INFO] Iniciando barrido de frecuencias Wi-Fi...");
    
    // WiFi.scanNetworks() devuelve el número de redes encontradas
    int numRedes = WiFi.scanNetworks();

    if (numRedes == 0) {
      Serial.println("[ALERTA] No se detectaron redes inalámbricas en el área.");
    } else {
      Serial.print("[OK] Redes detectadas: ");
      Serial.println(numRedes);

      // Cabecera de tabla de diagnóstico
      Serial.println("----------------------------------------------------------------------");
      Serial.printf("%-4s | %-24s | %-6s | %-8s | %-16s\n", "No.", "SSID", "Canal", "RSSI", "Aptitud Clínica");
      Serial.println("----------------------------------------------------------------------");

      for (int i = 0; i < numRedes; ++i) {
        int rssi = WiFi.RSSI(i);
        String calidad = "";

        // Clasificación biomédica según potencia de señal
        if (rssi >= -60) {
          calidad = "EXCELENTE (ECG Alta Frec)";
        } else if (rssi >= -75) {
          calidad = "BUENA (Telemetría Periódica)";
        } else if (rssi >= -85) {
          calidad = "REGULAR (Riesgo Latencia)";
        } else {
          calidad = "CRITICA (No Apto Soporte Vida)";
        }

        Serial.printf("%-4d | %-24s | %-6d | %-4d dBm | %-16s\n", 
                      i + 1, 
                      WiFi.SSID(i).c_str(), 
                      WiFi.channel(i), 
                      rssi, 
                      calidad.c_str());
      }
      Serial.println("----------------------------------------------------------------------");
    }

    // Liberar memoria ocupada por los resultados del escaneo
    WiFi.scanDelete();
  }
}
