/*
 * Laboratorio 05: Modulación PWM de 8 bits para Actuador de Bomba de Infusión
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Controla el ciclo de trabajo de una señal PWM en el Pin D9 para dosificar
 * la tasa de infusión de un actuador médico regulado desde una entrada analógica.
 */

const int PIN_POT_CONTROL = A0;
const int PIN_PWM_ACTUADOR = 9; // Pin con Timer1 PWM hardware

void setup() {
  pinMode(PIN_PWM_ACTUADOR, OUTPUT);

  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 05: Modulación PWM (Control de Bomba de Infusión)");
  Serial.println(" Actuador conectado a Pin D9 (Timer1 - 490 Hz)");
  Serial.println("==================================================");
}

void loop() {
  // 1. Leer potenciómetro de ajuste de dosis (0 - 1023)
  int valorAdc = analogRead(PIN_POT_CONTROL);

  // 2. Mapear a rango de ciclo de trabajo PWM de 8 bits (0 - 255)
  int valorPwm = map(valorAdc, 0, 1023, 0, 255);

  // 3. Escribir valor al temporizador PWM por hardware
  analogWrite(PIN_PWM_ACTUADOR, valorPwm);

  // 4. Cálculos teóricos para telemetría clínica
  float dutyCyclePct = (valorPwm / 255.0) * 100.0;
  float voltajeEquivalente = (valorPwm / 255.0) * 5.0;
  float tasaInfusionMlH = (dutyCyclePct / 100.0) * 250.0; // Escala máx: 250 mL/h

  // 5. Reporte periódico por Serial
  Serial.print("[BOMBA INFUSIÓN] PWM: ");
  Serial.print(valorPwm);
  Serial.print("/255 | Duty: ");
  Serial.print(dutyCyclePct, 1);
  Serial.print(" % | Tensión Promedio: ");
  Serial.print(voltajeEquivalente, 2);
  Serial.print(" V | Tasa Dosificación: ");
  Serial.print(tasaInfusionMlH, 1);
  Serial.println(" mL/h");

  delay(500);
}
