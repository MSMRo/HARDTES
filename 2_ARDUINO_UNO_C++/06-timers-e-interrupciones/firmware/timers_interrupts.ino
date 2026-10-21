/*
 * Laboratorio 06: Timers por Hardware (Timer1 CTC 100Hz) e Interrupciones Externas (INT0)
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Demuestra la adquisición biomédica determinística libre de jitter mediante Timer1
 * a exactamente 100 Hz (10 ms) y atención inmediata de paro/emergencia con INT0 (Pin D2).
 */

#include <avr/io.h>
#include <avr/interrupt.h>

const int PIN_BOTON_EMERGENCIA = 2; // INT0 por hardware
const int PIN_LED_MUESTREO = 13;

// Variables compartidas con ISRs (obligatorio declarar como volatile)
volatile bool flagEmergencia = false;
volatile unsigned long contadorMuestras = 0;
volatile bool estadoLedMuestreo = false;

// 1. Rutina de Servicio de Interrupción Externa (INT0) en Pin D2
void isrBotonEmergencia() {
  flagEmergencia = true; // Solo activa bandera; procesar fuera de la ISR
}

// 2. Rutina de Servicio de Interrupción de Timer1 (100 Hz - cada 10 ms)
ISR(TIMER1_COMPA_vect) {
  contadorMuestras++;

  // Cada 50 muestras (500 ms) alternar LED como demostración visual
  if (contadorMuestras % 50 == 0) {
    estadoLedMuestreo = !estadoLedMuestreo;
    if (estadoLedMuestreo) {
      PORTB |= (1 << PORTB5);  // D13 HIGH
    } else {
      PORTB &= ~(1 << PORTB5); // D13 LOW
    }
  }

  // Aquí se ejecutaría la lectura analógica de la bioseñal (ECG/PPG a 100 Hz)
}

void configurarTimer1_100Hz() {
  cli(); // Deshabilitar interrupciones globales durante la configuración

  TCCR1A = 0; // Modo normal de pines de salida del timer
  TCCR1B = 0; // Limpiar registros de control
  TCNT1  = 0; // Reiniciar contador de 16 bits

  // OCR1A = (16MHz / (64 * 100Hz)) - 1 = 2499
  OCR1A = 2499;

  // Activar modo CTC (Clear Timer on Compare Match) bit WGM12 en TCCR1B
  // Configurar Prescaler a 64 (bits CS11 y CS10 en 1)
  TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);

  // Habilitar la interrupción por comparación en canal A (OCIE1A)
  TIMSK1 |= (1 << OCIE1A);

  sei(); // Habilitar interrupciones globales
}

void setup() {
  pinMode(PIN_LED_MUESTREO, OUTPUT);
  pinMode(PIN_BOTON_EMERGENCIA, INPUT_PULLUP);

  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 06: Timers Hardware e Interrupciones ISR");
  Serial.println(" Timer1: 100 Hz (Muestreo isócrono cada 10 ms)");
  Serial.println(" INT0: Pin D2 activo para botón de pánico");
  Serial.println("==================================================");

  // Configurar interrupción externa en Pin 2 por flanco de bajada (FALLING)
  attachInterrupt(digitalPinToInterrupt(PIN_BOTON_EMERGENCIA), isrBotonEmergencia, FALLING);

  // Iniciar temporizador periódico
  configurarTimer1_100Hz();
}

void loop() {
  // Atender bandera de emergencia médica disparada por la ISR externa
  if (flagEmergencia) {
    flagEmergencia = false; // Limpiar bandera

    Serial.println("\n🚨 [INTERRUPCION CRITICA] ¡BOTÓN DE PÁNICO DEL PACIENTE PULSADO!");
    Serial.print("   Muestras biológicas recolectadas hasta el evento: ");
    Serial.println(contadorMuestras);
    Serial.println("   Despachando alerta a central de enfermería...");
  }

  // Tareas no críticas en segundo plano
  delay(200);
}
