 /*
 * Laboratorio 04: Muestreo ADC de 10 bits y Conversión de Biosensor
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Lectura del conversor analógico-digital en el pin A0 con filtrado de media móvil,
 * cálculo de tensión en milivoltios y conversión a temperatura clínica (°C).
 */

const int PIN_SENSOR_A0 = A0;
const float VREF = 5.0; // Voltaje de referencia estándar (5V)
const int NUM_MUESTRAS_FILTRO = 8;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Configurar referencia de voltaje (DEFAULT = 5V en Arduino Uno)
  analogReference(DEFAULT);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 04: Conversión ADC de 10 bits con Filtro");
  Serial.println(" Muestreando canal A0...");
  Serial.println("==================================================");
}

void loop() {
  // 1. Sobremuestreo y promedio móvil para filtrar ruido de 60Hz
  long sumaRaw = 0;
  for (int i = 0; i < NUM_MUESTRAS_FILTRO; i++) {
    sumaRaw += analogRead(PIN_SENSOR_A0);
    delay(5); // Pequeño intervalo de estabilización del condensador Sample & Hold
  }
  int valorAdcPromedio = sumaRaw / NUM_MUESTRAS_FILTRO;

  // 2. Conversión a Milivoltios: (ADC / 1023.0) * 5000 mV
  float milivoltios = (valorAdcPromedio / 1023.0) * (VREF * 1000.0);

  // 3. Conversión a Temperatura (°C) considerando escala TMP36 (10mV/°C, 500mV a 0°C)
  float temperaturaC = (milivoltios - 500.0) / 10.0;

  // 4. Reporte estructurado por Serial
  Serial.print("ADC Crudo: ");
  Serial.print(valorAdcPromedio);
  Serial.print(" / 1023 | ");
  Serial.print("Voltaje: ");
  Serial.print(milivoltios, 1);
  Serial.print(" mV | ");
  Serial.print("Temp Calculada: ");
  Serial.print(temperaturaC, 2);
  Serial.print(" °C | Estado: ");

  if (temperaturaC >= 38.0) {
    Serial.println("FIEBRE ALTA 🚨");
  } else if (temperaturaC < 35.5) {
    Serial.println("HIPOTERMIA ⚠️");
  } else {
    Serial.println("NORMOTERMIA ✅");
  }

  delay(1000);
}
