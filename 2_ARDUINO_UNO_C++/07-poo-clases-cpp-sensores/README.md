# Laboratorio 07: Programación Orientada a Objetos (POO) en C++ para Instrumentación Biomédica

## 1. Introducción y Relevancia en Telesalud
A medida que un sistema de telesalud crece —incorporando electrocardiógrafos, sensores de temperatura dérmica, transductores de flujo respiratorio y pulsioxímetros en un mismo dispositivo—, el código estructurado tradicional en un único archivo se vuelve inmanejable y propenso a errores.

La **Programación Orientada a Objetos (POO) en C++ embebido** permite:
- **Encapsulamiento:** Ocultar la complejidad de los registros del ADC y buffers de calibración dentro del objeto sensor.
- **Modularidad y Reutilización:** Crear múltiples instancias independientes (ej. `BioSensor sensorBrazoIzquierdo(A0)` y `BioSensor sensorBrazoDerecho(A1)`).
- **Mantenibilidad:** Separar la lógica de adquisición física del reporte de telemetría a la nube o Node-RED.

---

## 2. Arquitectura de la Clase C++ `MonitorCardiaco`

```cpp
class MonitorCardiaco {
private:
    uint8_t pinAdc;
    uint8_t pinLedAlarma;
    uint16_t umbralPicoR;
    uint16_t bufferMuestras[4];
    uint8_t indiceBuffer;
    uint8_t ultimoBpm;
    bool alarmaActiva;

public:
    // Constructor con lista de inicialización
    MonitorCardiaco(uint8_t pinSensor, uint8_t pinAlarma, uint16_t umbral);

    void begin();
    void actualizar();
    uint8_t getBPM() const;
    bool getEstadoAlarma() const;
    void imprimirTelemetria() const;
};
```

---

## 3. Ventajas de la POO en Microcontroladores
- **Cero sobrecosto de ejecución:** Métodos no virtuales en C++ se compilan directamente a llamadas a función planas idénticas al código en C clásico.
- **Uso estático de memoria:** Declarar las instancias globales evita el uso peligroso de memoria dinámica en el *Heap* (`new`/`delete` o `malloc`), previniendo la fragmentación de la SRAM.

---

## 4. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Ejecutando dos objetos `MonitorCardiaco` y `SensorTemperatura`.
2. **Entrada Analógica A0:** Simula la entrada de fotopletismografía (PPG).
3. **LED Indicador en D13:** Activado por el método `actualizar()` del objeto cuando se superan los umbrales fisiológicos.

---

## 5. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/07-poo-clases-cpp-sensores/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Observa en el Monitor Serie cómo el objeto C++ gestiona de forma autónoma el filtrado, cálculo de frecuencia cardíaca y emisión de paquetes JSON de telemetría.
