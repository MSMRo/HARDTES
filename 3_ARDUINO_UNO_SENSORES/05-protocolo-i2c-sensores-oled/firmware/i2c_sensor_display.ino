/*
 * Laboratorio 05: Protocolo Hardware I2C (Wire.h), Escáner y Telemetría
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Implementación del protocolo I2C / TWI en los pines A4 (SDA) y A5 (SCL).
 * Escanea el bus para detectar biosensores y pantallas conectadas,
 * y transmite tramas periódicas de constantes vitales del paciente.
 */

#include <Wire.h>

void escanearBusI2C() {
  byte error, direccion;
  int dispositivosEncontrados = 0;

  Serial.println("[I2C SCAN] Iniciando barrido de direcciones (0x01 a 0x7F)...");

  for (direccion = 1; direccion < 127; direccion++) {
    // La librería Wire utiliza el valor de retorno para saber si un dispositivo respondió con ACK
    Wire.beginTransmission(direccion);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("  [DISPOSITIVO HALLADO] Dirección I2C: 0x");
      if (direccion < 16) Serial.print("0");
      Serial.print(direccion, HEX);

      // Identificación de dispositivos biomédicos habituales
      if (direccion == 0x3C || direccion == 0x3D) {
        Serial.println(" -> Pantalla OLED SSD1306 (Monitor de Cabecera)");
      } else if (direccion == 0x57) {
        Serial.println(" -> Sensor Pulsioxímetro MAX30102");
      } else if (direccion == 0x68) {
        Serial.println(" -> Sensor de Caídas MPU6050");
      } else if (direccion == 0x27 || direccion == 0x3F) {
        Serial.println(" -> Adaptador I2C para Display LCD Clínico");
      } else {
        Serial.println(" -> Sensor / Periférico I2C Genérico");
      }
      dispositivosEncontrados++;
    }
  }

  if (dispositivosEncontrados == 0) {
    Serial.println("[I2C ALERTA] No se detectaron dispositivos con respuesta ACK.");
  } else {
    Serial.printf("[I2C OK] Total de periféricos detectados en el bus: %d\n", dispositivosEncontrados);
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Inicializar hardware I2C como Maestro (Master)
  Wire.begin();

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 05: Protocolo Hardware I2C (Master)");
  Serial.println(" Pines: SDA = A4 | SCL = A5 (Requiere Pull-Ups)");
  Serial.println("==================================================");

  // Ejecutar diagnóstico inicial del bus
  escanearBusI2C();
}

void loop() {
  // Transmisión periódica a display I2C (dirección típica 0x3C o 0x27)
  const byte DIR_TARGET = 0x3C;

  int bpm = 74 + random(-3, 5);
  int spo2 = 98;

  // Iniciar paquete I2C hacia el esclavo
  Wire.beginTransmission(DIR_TARGET);
  Wire.write(0x40); // Byte de control / comando de datos
  Wire.write(bpm);  // Byte 1: Frecuencia cardíaca
  Wire.write(spo2); // Byte 2: SpO2
  byte resultado = Wire.endTransmission();

  Serial.print("[I2C TX] Enviado a 0x3C -> BPM: ");
  Serial.print(bpm);
  Serial.print(" | SpO2: ");
  Serial.print(spo2);
  Serial.print("% | Estado ACK: ");
  Serial.println(resultado == 0 ? "EXITO (ACK) ✅" : "SIN RESPUESTA (NACK) ❌");

  delay(2000);
}
