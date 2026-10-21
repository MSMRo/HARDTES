/*
 * Laboratorio 03: Front-End de ECG AD8232 y Detección de Desconexión de Electrodos
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Adquisición de biopotencial cardíaco en pin A0 y supervisión de pines de seguridad
 * LO+ (D10) y LO- (D11) para alertar sobre desprendimiento de electrodos del paciente.
 */

const int PIN_ECG_OUT = A0;
const int PIN_LO_PLUS = 10;
const int PIN_LO_MINUS = 11;
const int PIN_LED_ALARMA = 13;

void setup() {
  pinMode(PIN_LO_PLUS, INPUT);
  pinMode(PIN_LO_MINUS, INPUT);
  pinMode(PIN_LED_ALARMA, OUTPUT);
  digitalWrite(PIN_LED_ALARMA, LOW);

  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 03: Monitor ECG AD8232 y Leads-Off Detection");
  Serial.println(" ECG en A0 | LO+ en D10 | LO- en D11");
  Serial.println("==================================================");
}

void loop() {
  // 1. Verificar si los electrodos están en contacto con el paciente
  bool electrodoSuelto = (digitalRead(PIN_LO_PLUS) == HIGH) || (digitalRead(PIN_LO_MINUS) == HIGH);

  if (electrodoSuelto) {
    // Alerta visual de electrodo desconectado
    digitalWrite(PIN_LED_ALARMA, HIGH);
    Serial.println("!ALERTA: Electrodo Desconectado (Leads-Off)! | Valor_ECG: 0");
  } else {
    // Electrodos en contacto óptimo: leer biopotencial
    digitalWrite(PIN_LED_ALARMA, LOW);
    int valorEcg = analogRead(PIN_ECG_OUT);

    // Formato listo para el Serial Plotter de Arduino IDE
    Serial.print("ECG_Muestra:");
    Serial.println(valorEcg);
  }

  // Tasa de muestreo de telemetría (~100 Hz = 10 ms)
  delay(10);
}
