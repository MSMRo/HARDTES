/*
 * Laboratorio 02: Tipos de Datos de Ancho Fijo, Structs y Punteros en C/C++
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Optimización de memoria SRAM en el ATmega328P para empaquetar y evaluar
 * constantes vitales de pacientes mediante estructuras, punteros y paso por referencia.
 */

#include <stdint.h>

// Definición de enum fuertemente tipado a 1 byte
enum EstadoTriage : uint8_t {
  TRIAGE_VERDE = 0,    // Paciente estable
  TRIAGE_AMARILLO = 1, // Requiere observación
  TRIAGE_ROJO = 2      // Emergencia crítica
};

// Estructura empaquetada contigua de constantes vitales
struct __attribute__((packed)) SignosVitales {
  uint8_t id_cama;
  uint8_t bpm;
  uint8_t spo2;
  float temperatura_c;
  EstadoTriage triage;
};

// Función que modifica el estado usando un puntero directo a la memoria
void evaluarEstadoClinico(SignosVitales* p) {
  if (p->spo2 < 90 || p->bpm > 115) {
    p->triage = TRIAGE_ROJO;
  } else if (p->spo2 <= 94 || p->bpm > 95) {
    p->triage = TRIAGE_AMARILLO;
  } else {
    p->triage = TRIAGE_VERDE;
  }
}

// Función de reporte pasando la estructura por referencia constante (sin copiar en pila)
void imprimirReporte(const SignosVitales &ref) {
  Serial.println("--------------------------------------------------");
  Serial.print("Cama No.:         "); Serial.println(ref.id_cama);
  Serial.print("Frec. Cardíaca:   "); Serial.print(ref.bpm); Serial.println(" lpm");
  Serial.print("Saturación SpO2:  "); Serial.print(ref.spo2); Serial.println(" %");
  Serial.print("Temperatura:      "); Serial.print(ref.temperatura_c, 1); Serial.println(" °C");
  Serial.print("Nivel de Triage:  ");
  switch (ref.triage) {
    case TRIAGE_VERDE:    Serial.println("VERDE (Estable)"); break;
    case TRIAGE_AMARILLO: Serial.println("AMARILLO (Observación)"); break;
    case TRIAGE_ROJO:     Serial.println("ROJO (¡EMERGENCIA CRÍTICA!)"); break;
  }
  Serial.print("Tamaño en SRAM:   ");
  Serial.print(sizeof(ref));
  Serial.println(" bytes");
}

// Instancia global en memoria estática
SignosVitales pacienteCama04;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 02: C/C++ Embebido - Structs y Referencias");
  Serial.println("==================================================");

  // Inicialización de la estructura
  pacienteCama04.id_cama = 4;
  pacienteCama04.bpm = 72;
  pacienteCama04.spo2 = 98;
  pacienteCama04.temperatura_c = 36.6;
  pacienteCama04.triage = TRIAGE_VERDE;
}

void loop() {
  // Simular deterioro clínico aleatorio
  pacienteCama04.bpm = 65 + random(0, 55);       // Entre 65 y 120 lpm
  pacienteCama04.spo2 = 86 + random(0, 14);      // Entre 86 y 99 %
  pacienteCama04.temperatura_c = 36.4 + (random(0, 25) * 0.1);

  // Evaluar pasando la dirección de memoria (&) mediante puntero
  evaluarEstadoClinico(&pacienteCama04);

  // Imprimir pasando por referencia constante
  imprimirReporte(pacienteCama04);

  delay(3000);
}
