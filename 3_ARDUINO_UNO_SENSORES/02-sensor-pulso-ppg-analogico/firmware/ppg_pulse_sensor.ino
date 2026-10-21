/*
 * Laboratorio 02: Procesamiento Digital de Señal PPG y Detección de Latidos
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Muestreo de señal de fotopletismografía en A0, filtro DC-removal y cálculo
 * de Intervalo Inter-Latido (IBI) y Frecuencia Cardíaca (BPM) con LED sístole en D13.
 */

const int PIN_PPG = A0;
const int PIN_LED_LATIDO = 13;

// Variables del filtro IIR paso-altas (DC removal)
float x_prev = 0;
float y_prev = 0;
const float ALPHA = 0.95;

// Variables de detección de picos y temporización
unsigned long tiempoUltimoLatido = 0;
unsigned long ultimoMuestreo = 0;
const unsigned long PERIODO_MUESTREO = 10; // 100 Hz (10 ms)
const int UMBRAL_PICO = 50;                // Amplitud AC mínima para validar sístole
bool enPulso = false;

// Buffer de promedio móvil para BPM
int bufferBpm[5] = {72, 72, 72, 72, 72};
int idxBpm = 0;

void setup() {
  pinMode(PIN_LED_LATIDO, OUTPUT);
  digitalWrite(PIN_LED_LATIDO, LOW);

  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 02: Procesamiento PPG y Detector de Sístole");
  Serial.println(" Muestreo a 100 Hz en Pin A0");
  Serial.println("==================================================");
}

void loop() {
  unsigned long ahora = millis();

  // Muestreo determinístico cada 10 ms
  if (ahora - ultimoMuestreo >= PERIODO_MUESTREO) {
    ultimoMuestreo = ahora;

    // 1. Leer ADC (0 a 1023)
    int rawVal = analogRead(PIN_PPG);

    // 2. Filtro digital IIR paso-altas: y[n] = alpha * (y[n-1] + x[n] - x[n-1])
    float acSignal = ALPHA * (y_prev + rawVal - x_prev);
    x_prev = rawVal;
    y_prev = acSignal;

    // 3. Algoritmo detector de pico sistólico
    if (acSignal > UMBRAL_PICO && !enPulso) {
      unsigned long ibi = ahora - tiempoUltimoLatido; // Intervalo en ms

      // Ventana fisiológica válida: entre 300 ms (200 lpm) y 2000 ms (30 lpm)
      if (ibi >= 300 && ibi <= 2000) {
        tiempoUltimoLatido = ahora;
        enPulso = true;
        digitalWrite(PIN_LED_LATIDO, HIGH); // Encender LED con la sístole

        int bpmInstantaneo = 60000 / ibi;

        // Promedio móvil de 5 latidos
        bufferBpm[idxBpm] = bpmInstantaneo;
        idxBpm = (idxBpm + 1) % 5;

        int sumaBpm = 0;
        for (int i = 0; i < 5; i++) sumaBpm += bufferBpm[i];
        int bpmFiltrado = sumaBpm / 5;

        // Telemetría serial
        Serial.print("❤️ LATIDO! IBI: ");
        Serial.print(ibi);
        Serial.print(" ms | BPM Instantáneo: ");
        Serial.print(bpmInstantaneo);
        Serial.print(" | BPM Promedio: ");
        Serial.print(bpmFiltrado);
        Serial.println(" lpm");
      }
    }

    // Apagar LED cuando la onda pulsátil desciende
    if (acSignal < (UMBRAL_PICO / 2) && enPulso) {
      enPulso = false;
      digitalWrite(PIN_LED_LATIDO, LOW);
    }
  }
}
