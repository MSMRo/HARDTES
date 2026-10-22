# Laboratorio 02: Sensor Óptico de Pulso Cardíaco (PPG) y Algoritmo de Detección de Latidos

## 1. Introducción y Relevancia en Telesalud
La **Fotopletismografía (PPG)** es la tecnología óptica no invasiva que sustenta a todos los pulsioxímetros clínicos y relojes inteligentes modernos.

Se basa en emitir luz (comúnmente verde a $530\text{ nm}$ o infrarroja a $940\text{ nm}$) sobre la piel y medir la luz reflejada o transmitida mediante un fotodetector:
- Durante la **sístole cardíaca**, el ventrículo izquierdo bombea sangre oxigenada aumentando el volumen sanguíneo en el lecho capilar del dedo, lo que incrementa la absorción de luz.
- Durante la **diástole**, el volumen cae y la luz detectada aumenta.

En este laboratorio aprenderás a:
- Comprender la diferencia entre la **componente estática (DC)** y la **onda pulsátil arterial (AC)** de la señal PPG.
- Implementar un **filtro digital paso-altas IIR** en Arduino para desacoplar la componente continua sin capacitores externos.
- Programar un algoritmo de detección de picos sistólicos, cálculo del intervalo entre latidos (**IBI**) y cómputo de **latidos por minuto (BPM)**.

---

## 2. Morfología de la Señal PPG y Procesamiento Digital



### A. Filtro Paso-Altas Digital IIR (DC Removal):
Para eliminar la deriva de línea base y retener solo la pulsación:
$$y[n] = \alpha \cdot (y[n-1] + x[n] - x[n-1])$$
Con $\alpha \approx 0.95$, este filtro atenúa todo cambio lento por debajo de $0.5\text{ Hz}$.

### B. Cálculo de BPM e Intervalo Inter-Latido (IBI):
$$\text{IBI} = t_{\text{pico actual}} - t_{\text{pico anterior}}\quad (\text{en milisegundos})$$
$$\text{BPM} = \frac{60\,000}{\text{IBI (ms)}}$$

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Muestreando la señal PPG en el pin analógico `A0`.
2. **Generador de Señales:** Simula la pulsación biológica de un paciente a $1.2\text{ Hz}$ ($72\text{ lpm}$) con offset continuo.
3. **LED Indicador en D13:** Parpadea sincronizado con cada latido detectado.
4. **Osciloscopio Virtual:** Visualiza la señal analógica cruda y la detección de los picos.

---

## 4. Código Fuente del Firmware (`firmware/ppg_pulse_sensor.ino`)
Procesa las muestras a intervalos de $10\text{ ms}$ ($100\text{ Hz}$), actualiza el umbral adaptativo y calcula la frecuencia cardíaca promedio de los últimos 5 latidos.

---

## 5. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/02-sensor-pulso-ppg-analogico/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Observa cómo el LED parpadea al ritmo del corazón y el Monitor Serie reporta el IBI en milisegundos y los BPM calculados.
