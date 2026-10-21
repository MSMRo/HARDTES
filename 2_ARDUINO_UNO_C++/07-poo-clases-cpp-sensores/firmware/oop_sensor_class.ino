/*
 * Laboratorio 07: Programación Orientada a Objetos en C++ Embebido
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Encapsulamiento de la lógica de adquisición, filtrado digital y detección
 * de arritmias dentro de una clase C++ modular (MonitorCardiaco).
 */

#include <stdint.h>

class MonitorCardiaco {
private:
  uint8_t _pinAdc;
  uint8_t _pinAlarma;
  uint16_t _umbralPico;
  uint8_t _ultimoBpm;
  bool _alarmaTaquicardia;
  uint16_t _historial[4];
  uint8_t _idx;

public:
  // Constructor con lista de inicialización
  MonitorCardiaco(uint8_t pinAdc, uint8_t pinAlarma, uint16_t umbral)
    : _pinAdc(pinAdc), _pinAlarma(pinAlarma), _umbralPico(umbral), 
      _ultimoBpm(72), _alarmaTaquicardia(false), _idx(0) {
    for (int i = 0; i < 4; i++) _historial[i] = 0;
  }

  // Inicialización de hardware
  void begin() {
    pinMode(_pinAlarma, OUTPUT);
    digitalWrite(_pinAlarma, LOW);
  }

  // Método de adquisición y procesamiento en tiempo real
  void actualizar() {
    // 1. Lectura del ADC
    uint16_t lectura = analogRead(_pinAdc);

    // 2. Filtro de promedio móvil interno
    _historial[_idx] = lectura;
    _idx = (_idx + 1) % 4;

    uint32_t suma = 0;
    for (int i = 0; i < 4; i++) suma += _historial[i];
    uint16_t filtrado = suma / 4;

    // 3. Estimación de BPM proporcional a la señal
    _ultimoBpm = map(filtrado, 0, 1023, 50, 130);

    // 4. Clasificación de taquicardia clínica (> 100 lpm)
    if (_ultimoBpm > 100) {
      _alarmaTaquicardia = true;
      digitalWrite(_pinAlarma, HIGH);
    } else {
      _alarmaTaquicardia = false;
      digitalWrite(_pinAlarma, LOW);
    }
  }

  // Métodos getters (constantes para garantizar inmutabilidad)
  uint8_t getBPM() const { return _ultimoBpm; }
  bool getAlarma() const { return _alarmaTaquicardia; }

  // Serialización a JSON para telemedicina
  void imprimirTelemetria(const char* idPaciente) const {
    Serial.print("{\"paciente\":\"");
    Serial.print(idPaciente);
    Serial.print("\",\"bpm\":");
    Serial.print(_ultimoBpm);
    Serial.print(",\"taquicardia\":");
    Serial.print(_alarmaTaquicardia ? "true" : "false");
    Serial.print(",\"estado\":\"");
    Serial.print(_alarmaTaquicardia ? "ALERTA_UCI" : "NORMAL");
    Serial.println("\"}");
  }
};

// Instanciación estática global (sin fragmentación de memoria en Heap)
MonitorCardiaco monitorPaciente1(A0, 13, 600);

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 07: Programación Orientada a Objetos en C++");
  Serial.println(" Instancia MonitorCardiaco inicializada");
  Serial.println("==================================================");

  monitorPaciente1.begin();
}

void loop() {
  // El objeto procesa sus sensores internamente
  monitorPaciente1.actualizar();

  // Emisión periódica de telemetría médica
  monitorPaciente1.imprimirTelemetria("PAC-UPCH-007");

  delay(1000);
}
