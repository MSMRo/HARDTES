/*
 * Proyecto 01: Blink LED con Temporización No Bloqueante (millis)
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Este programa demuestra cómo controlar un LED indicador de estado del sistema (Heartbeat)
 * sin usar la función bloqueante delay(). Esto es indispensable en telemedicina
 * para no interferir con la adquisición de señales biomédicas (ECG, PPG, EMG).
 */

// Definición de pines
// En la mayoría de placas ESP32 WROOM-32, el LED onboard está en el GPIO 2
const int LED_PIN = 2;

// Constantes de temporización
const unsigned long INTERVALO_PARPADEO = 500;  // 500 ms (1 Hz de frecuencia)
const unsigned long INTERVALO_TELEMETRIA = 2000; // Imprimir estado cada 2 segundos

// Variables para control del tiempo (unsigned long obligatorio para millis())
unsigned long previousMillisLed = 0;
unsigned long previousMillisSerial = 0;
bool estadoLed = LOW;
unsigned long contadorCiclos = 0;

void setup() {
  // Inicialización del puerto serie a alta velocidad (115200 baudios)
  Serial.begin(115200);
  delay(100); // Pequeña pausa de estabilización de la UART

  // Configuración del GPIO como salida digital
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Proyecto 01: Temporización No Bloqueante con millis()");
  Serial.println(" Sistema iniciado y listo para adquisición continua.");
  Serial.println("==================================================");
}

void loop() {
  // Obtener el tiempo actual desde que arrancó el ESP32
  unsigned long currentMillis = millis();

  // -------------------------------------------------------------
  // Tarea 1: Conmutar el LED de estado (Heartbeat del equipo)
  // -------------------------------------------------------------
  if (currentMillis - previousMillisLed >= INTERVALO_PARPADEO) {
    previousMillisLed = currentMillis;

    // Invertir el estado lógico del LED
    estadoLed = !estadoLed;
    digitalWrite(LED_PIN, estadoLed);
  }

  // -------------------------------------------------------------
  // Tarea 2: Reporte de salud del microcontrolador por Serial
  // -------------------------------------------------------------
  if (currentMillis - previousMillisSerial >= INTERVALO_TELEMETRIA) {
    previousMillisSerial = currentMillis;
    contadorCiclos++;

    Serial.print("[HEARTBEAT] Uptime: ");
    Serial.print(currentMillis / 1000);
    Serial.print("s | Ciclos de reporte: ");
    Serial.print(contadorCiclos);
    Serial.print(" | Estado CPU: OK | Core: ");
    Serial.println(xPortGetCoreID()); // Muestra en qué núcleo de ESP32 corre la tarea
  }

  // Aquí se podrían ejecutar filtros digitales o lecturas ADC sin interrupciones
}
