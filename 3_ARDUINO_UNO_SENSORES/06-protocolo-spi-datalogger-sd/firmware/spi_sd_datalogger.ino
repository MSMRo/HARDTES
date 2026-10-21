/*
 * Laboratorio 06: Protocolo Hardware SPI y Data Logger en Tarjeta MicroSD
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Demuestra el uso del bus SPI (MOSI=11, MISO=12, SCK=13, CS=10) para registrar
 * series temporales biomédicas continuas (Monitor Holter) en formato CSV.
 */

#include <SPI.h>
#include <SD.h>

const int PIN_CHIP_SELECT = 10;
const char* NOMBRE_ARCHIVO = "HOLTER.CSV";

unsigned long contadorRegistros = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 06: Bus SPI y Data Logger en Tarjeta SD");
  Serial.println(" Pines: MOSI=11, MISO=12, SCK=13, CS=10");
  Serial.println("==================================================");

  // El pin SS por defecto (10 en Arduino Uno) debe ser salida aunque no se use
  pinMode(PIN_CHIP_SELECT, OUTPUT);

  // Inicializar el bus SPI y el subsistema de archivos FAT
  Serial.print("[SD] Inicializando tarjeta MicroSD en bus SPI...");
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("\n[SD ERROR] Falla de inicialización o tarjeta no presente.");
    Serial.println("   (Verifique formato FAT16/FAT32 y conexiones SPI)");
    return;
  }
  Serial.println(" ¡Tarjeta inicializada con éxito!");

  // Crear o verificar encabezado CSV si el archivo no existe
  if (!SD.exists(NOMBRE_ARCHIVO)) {
    File archivo = SD.open(NOMBRE_ARCHIVO, FILE_WRITE);
    if (archivo) {
      archivo.println("timestamp_ms,paciente_id,bpm,spo2,temp_c,estado");
      archivo.close();
      Serial.println("[SD OK] Archivo HOLTER.CSV creado con cabeceras médicas.");
    }
  }
}

void loop() {
  // Simular adquisición de biopotenciales de paciente
  int bpm = 72 + random(-4, 6);
  int spo2 = 97 + random(0, 3);
  float temp = 36.5 + (random(-1, 3) * 0.1);
  const char* estado = (spo2 < 92 || bpm > 100) ? "ALERTA" : "NORMAL";

  // Abrir el archivo en modo append para escribir nueva muestra
  File logFile = SD.open(NOMBRE_ARCHIVO, FILE_WRITE);

  if (logFile) {
    contadorRegistros++;
    unsigned long ahora = millis();

    // Escribir fila CSV
    logFile.print(ahora);
    logFile.print(",PAC_HOLTER_01,");
    logFile.print(bpm);
    logFile.print(",");
    logFile.print(spo2);
    logFile.print(",");
    logFile.print(temp, 1);
    logFile.print(",");
    logFile.println(estado);

    // Forzar escritura física de los sectores flash de la SD
    logFile.flush();
    logFile.close();

    // Reporte serial
    Serial.print("[LOG #");
    Serial.print(contadorRegistros);
    Serial.print("] Registrado en SD: ");
    Serial.print(ahora);
    Serial.print(" ms | BPM: ");
    Serial.print(bpm);
    Serial.print(" | SpO2: ");
    Serial.print(spo2);
    Serial.println("%");
  } else {
    Serial.println("[SD ERROR] Imposible abrir HOLTER.CSV para escritura.");
  }

  delay(1000); // 1 registro por segundo
}
