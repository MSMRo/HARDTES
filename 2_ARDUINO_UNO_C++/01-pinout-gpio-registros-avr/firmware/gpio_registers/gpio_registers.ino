/*
 * Laboratorio 01: Pinout de Arduino Uno y Manipulación de Registros AVR
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Demuestra la programación directa de los registros del microcontrolador ATmega328P
 * (DDRB, PORTB, DDRD, PIND) para lograr lecturas y conmutaciones en un solo ciclo de reloj
 * (62.5 ns), técnica fundamental para sistemas biomédicos de tiempo real crítico.
 */

#include <avr/io.h>
#include <util/delay.h>

void setup() {
  // 1. Configurar D13 (PB5) como SALIDA digital usando el registro DDRB
  // Equivalente de alta velocidad a: pinMode(13, OUTPUT);
  DDRB |= (1 << DDB5);

  // 2. Configurar D2 (PD2) como ENTRADA digital usando el registro DDRD
  // Equivalente a: pinMode(2, INPUT);
  DDRD &= ~(1 << DDD2);

  // 3. Activar la resistencia interna de Pull-up en D2 escribiendo 1 en PORTD
  // Equivalente a: pinMode(2, INPUT_PULLUP);
  PORTD |= (1 << PORTD2);

  // Iniciar puerto serial para telemetría
  Serial.begin(115200);
  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 01: Registros de Hardware AVR en C/C++");
  Serial.println(" D13 configurado como salida via DDRB");
  Serial.println(" D2 configurado con Pull-Up via PORTD");
  Serial.println("==================================================");
}

void loop() {
  // 4. Leer el estado del pin D2 directamente desde el registro PIND
  // Si el pulsador se presiona, el pin se conecta a GND -> PIND bit 2 es 0
  bool botonPresionado = !(PIND & (1 << PIND2));

  if (botonPresionado) {
    // Encender LED en D13 (PB5) poniendo el bit en 1
    PORTB |= (1 << PORTB5);
    Serial.println("[ALERTA] Pulsador presionado -> LED D13 ON (PORTB5 = 1)");
    delay(200); // Debounce básico
  } else {
    // Apagar LED en D13 (PB5) limpiando el bit a 0
    PORTB &= ~(1 << PORTB5);
  }
}
