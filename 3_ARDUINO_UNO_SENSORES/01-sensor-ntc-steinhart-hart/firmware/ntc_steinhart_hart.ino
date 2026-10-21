/*
 * Laboratorio 01: Sensor de Temperatura Corporal NTC con Ecuación Beta / Steinhart-Hart
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Adquisición analógica en A0 y linealización mediante el modelo Beta (B=3950)
 * para termistor NTC de 10k en configuración de divisor de tensión.
 */

#include <math.h>

const int PIN_NTC = A0;

// Parámetros característicos del termistor NTC 10k 3950
const float R_NOMINAL = 10000.0;    // Resistencia a 25°C (10 kΩ)
const float T_NOMINAL = 298.15;     // 25°C en Kelvin (25 + 273.15)
const float BETA = 3950.0;          // Coeficiente Beta del fabricante
const float R_PULLDOWN = 10000.0;   // Resistencia fija del divisor (10 kΩ)

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 01 (Sensores): Termistor NTC y Ecuación Beta");
  Serial.println(" Sensor en Pin A0 (R0=10k, Beta=3950)");
  Serial.println("==================================================");
}

void loop() {
  // 1. Sobremuestreo de 10 lecturas para atenuar ruido térmico
  long sumaRaw = 0;
  for (int i = 0; i < 10; i++) {
    sumaRaw += analogRead(PIN_NTC);
    delay(5);
  }
  float adcPromedio = sumaRaw / 10.0;

  // Evitar divisiones por cero en caso de corto o desconexión
  if (adcPromedio < 1.0) adcPromedio = 1.0;
  if (adcPromedio > 1022.0) adcPromedio = 1022.0;

  // 2. Calcular resistencia del NTC a partir del divisor de voltaje
  // Vout = Vcc * (R_pulldown / (R_ntc + R_pulldown))
  // R_ntc = R_pulldown * ((1023 / ADC) - 1)
  float rNtc = R_PULLDOWN * ((1023.0 / adcPromedio) - 1.0);

  // 3. Aplicar Ecuación Beta de Steinhart-Hart
  // 1/T = 1/T0 + (1/Beta) * ln(R / R0)
  float steinhart;
  steinhart = rNtc / R_NOMINAL;          // (R / R0)
  steinhart = log(steinhart);            // ln(R / R0)
  steinhart /= BETA;                     // (1 / Beta) * ln(R / R0)
  steinhart += 1.0 / T_NOMINAL;          // + (1 / T0)
  steinhart = 1.0 / steinhart;           // Invertir para obtener Kelvin
  float tempCelsius = steinhart - 273.15; // Convertir a Celsius

  // 4. Reporte estructurado
  Serial.print("ADC: ");
  Serial.print((int)adcPromedio);
  Serial.print(" | R_NTC: ");
  Serial.print(rNtc, 0);
  Serial.print(" Ω | Temp: ");
  Serial.print(tempCelsius, 2);
  Serial.print(" °C | Diagnóstico: ");

  if (tempCelsius >= 38.0) {
    Serial.println("FIEBRE (Notificar a Médico) 🚨");
  } else if (tempCelsius <= 35.0) {
    Serial.println("HIPOTERMIA CRÍTICA ⚠️");
  } else {
    Serial.println("NORMAL ✅");
  }

  delay(1000);
}
