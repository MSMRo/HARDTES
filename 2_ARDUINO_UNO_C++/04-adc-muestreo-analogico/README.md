# Laboratorio 04: Conversor Analógico-Digital (ADC) y Muestreo de Biosensores

## 1. Introducción y Relevancia en Telesalud
La inmensa mayoría de las señales del cuerpo humano —temperatura corporal, presión arterial, actividad muscular (EMG) y ritmo cardíaco (PPG/ECG)— son por naturaleza **analógicas y continuas en el tiempo**. Para que el microcontrolador pueda procesarlas, filtrarlas y transmitirlas vía telemedicina, deben ser convertidas a números digitales mediante el **Conversor Analógico-Digital (ADC)**.

En este laboratorio aprenderás a:
- Conocer la arquitectura interna del ADC de aproximaciones sucesivas (SAR) de 10 bits del ATmega328P.
- Comprender el concepto de **Resolución de Cuantización y Voltaje de Referencia ($V_{ref}$)**.
- Alternar entre referencias de voltaje (`DEFAULT` a $5\text{ V}$ e `INTERNAL` a $1.1\text{ V}$) para maximizar la resolución en bioseñales de baja amplitud.
- Muestrear un sensor de temperatura analógico y convertir el código digital crudo a milivoltios y grados Celsius.

---

## 2. Fundamento Teórico y Matemáticas del ADC

El ADC de 10 bits mapea una tensión analógica entre $0\text{ V}$ y $V_{ref}$ a un entero entre $0$ y $1023$:

$$\text{Valor Digital} = \text{floor}\left( \frac{V_{in}}{V_{ref}} \cdot 1023 \right)$$

### Resolución de Voltaje por Cuenta (LSB):
$$\Delta V = \frac{V_{ref}}{1024}$$

- **Con $V_{ref} = 5.0\text{ V}$ (`DEFAULT`):**
  $$\Delta V = \frac{5000\text{ mV}}{1024} \approx 4.88\text{ mV por cuenta}$$
- **Con $V_{ref} = 1.1\text{ V}$ (`INTERNAL`):**
  $$\Delta V = \frac{1100\text{ mV}}{1024} \approx 1.07\text{ mV por cuenta}$$
  *(¡Casi 5 veces más sensibilidad para biosensores sin amplificación externa!)*

### Conversión a Milivoltios y Temperatura (Sensor TMP36):
El sensor analógico lineal TMP36 entrega $10\text{ mV}$ por cada grado Celsius con un offset de $500\text{ mV}$ a $0^\circ\text{C}$:
$$V_{out} = 500\text{ mV} + 10\text{ mV} \cdot T(^\circ\text{C})$$
$$T(^\circ\text{C}) = \frac{V_{medido}(\text{mV}) - 500\text{ mV}}{10\text{ mV}/^\circ\text{C}}$$

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Ejecutando muestreo en el pin analógico `A0`.
2. **Potenciómetro / Sensor de Temperatura Analógico:** Conectado a $5\text{ V}$, GND y el cursor al pin `A0`.
3. **Voltímetro Digital:** Mide el voltaje analógico real presente en el pin `A0` para cotejarlo con la conversión del microcontrolador.
4. **Terminal Serial:** Muestra el valor crudo del ADC ($0-1023$), los milivoltios calculados y la temperatura resultante.

---

## 4. Código Fuente del Firmware (`firmware/adc_sampling.ino`)
Implementa un filtro de promedio móvil sobre 8 muestras consecutivas del ADC para eliminar el ruido eléctrico antes de calcular el valor final.

---

## 5. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/04-adc-muestreo-analogico/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado.
3. Presiona **Power Circuit** y gira la perilla del potenciómetro/sensor conectado a `A0`.
4. Observa cómo el Monitor Serie reporta con alta fidelidad el cambio de voltaje y temperatura en tiempo real.
