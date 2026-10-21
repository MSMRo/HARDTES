/*
 * Laboratorio 04: Protocolo UART y Decodificador NMEA de GPS para Teleasistencia
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Recepción UART en pines D4/D5 vía SoftwareSerial para decodificar sentencias
 * $GPRMC de geolocalización de ambulancias o pacientes en telemonitoreo móvil.
 */

#include <SoftwareSerial.h>

// Pines para el puerto serie del GPS
const int PIN_GPS_RX = 4; // Conectar al pin TX del módulo GPS
const int PIN_GPS_TX = 5; // Conectar al pin RX del módulo GPS

SoftwareSerial gpsSerial(PIN_GPS_RX, PIN_GPS_TX);

String tramaNmea = "";

// Función para convertir formato NMEA (DDMM.MMMM) a Grados Decimales
float convertirNmeaAGrados(String val, char hemisferio) {
  if (val.length() == 0) return 0.0;
  
  float raw = val.toFloat();
  int grados = (int)(raw / 100);
  float minutos = raw - (grados * 100);
  float decimal = grados + (minutos / 60.0);

  if (hemisferio == 'S' || hemisferio == 'W') {
    decimal = -decimal;
  }
  return decimal;
}

void procesarSentenciaGprmc(String linea) {
  // Ejemplo: $GPRMC,123519.00,A,1205.3820,S,07701.8420,W,...
  int comaIndices[12];
  int count = 0;
  for (unsigned int i = 0; i < linea.length() && count < 12; i++) {
    if (linea.charAt(i) == ',') {
      comaIndices[count++] = i;
    }
  }

  if (count >= 6) {
    String status = linea.substring(comaIndices[1] + 1, comaIndices[2]);

    if (status == "A") { // Fix Satelital Válido
      String latRaw = linea.substring(comaIndices[2] + 1, comaIndices[3]);
      char latHem = linea.charAt(comaIndices[3] + 1);

      String lonRaw = linea.substring(comaIndices[4] + 1, comaIndices[5]);
      char lonHem = linea.charAt(comaIndices[5] + 1);

      float latitud = convertirNmeaAGrados(latRaw, latHem);
      float longitud = convertirNmeaAGrados(lonRaw, lonHem);

      // JSON listo para plataformas de mapas y Node-RED
      Serial.print("{\"ambulancia_id\":\"UPCH-SAMU-01\",\"fix\":true,\"lat\":");
      Serial.print(latitud, 6);
      Serial.print(",\"lon\":");
      Serial.print(longitud, 6);
      Serial.println(",\"estado\":\"EN_RUTA_HOSPITAL\"}");
    } else {
      Serial.println("[GPS] Buscando satélites (Fix Inválido)...");
    }
  }
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600); // La mayoría de módulos GPS operan a 9600 bps

  delay(100);
  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 04: Decodificador UART de GPS (NMEA $GPRMC)");
  Serial.println(" Escuchando GPS en Pines D4(RX) y D5(TX)");
  Serial.println("==================================================");
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = (char)gpsSerial.read();

    if (c == '\n') {
      tramaNmea.trim();
      if (tramaNmea.startsWith("$GPRMC")) {
        procesarSentenciaGprmc(tramaNmea);
      }
      tramaNmea = "";
    } else if (c != '\r') {
      tramaNmea += c;
    }
  }
}
