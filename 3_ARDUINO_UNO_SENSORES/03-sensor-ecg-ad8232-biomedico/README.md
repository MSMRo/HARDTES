# Laboratorio 03: Front-End de Electrocardiografía (ECG AD8232) y Detección de Electrodos Desconectados

## 1. Introducción y Relevancia en Telesalud
El registro del **Electrocardiograma (ECG)** refleja la actividad eléctrica despolarizante y repolarizante del miocardio durante el ciclo cardíaco (ondas P, complejo QRS y onda T).

Debido a que la señal cutánea tiene amplitudes de apenas $\approx 1\text{ mV}$ inmersas en voltajes de interferencia electromagnética de modo común de varios voltios, se requiere un Front-End analógico especializado como el **AD8232**:
- Amplificador de instrumentación con altísimo rechazo de modo común (**CMRR > 80 dB**).
- Circuito de excitación de pierna derecha (**Right Leg Drive - RLD**) para cancelar el ruido de la red eléctrica de $60\text{ Hz}$.
- Detección automática de desconexión de electrodos (**Leads-Off Detection**).

En este laboratorio aprenderás a:
- Conectar e interpretar las señales analógicas y de diagnóstico digital del módulo AD8232.
- Supervisar los pines digitales **`LO+` (Lead-Off +)** y **`LO-` (Lead-Off -)** para alertar cuando un electrodo pierde contacto con la piel.
- Muestrear la onda electrocardiográfica y formatearla para visualización en el **Serial Plotter** o transmisión hacia Node-RED.

---

## 2. Mapa de Conexiones del Sensor AD8232

```
  Módulo AD8232                   Arduino Uno
  +---------------+               +-------------+
  |           3.3V| <-----------> | 3.3V        |
  |            GND| <-----------> | GND         |
  |         OUTPUT| ------------> | A0 (ADC ECG)|
  |            LO+| ------------> | D10 (GPIO)  |
  |            LO-| ------------> | D11 (GPIO)  |
  |            SDN| (No conectado o HIGH)       |
  +---------------+               +-------------+
```

---

## 3. Lógica de Detección de Desconexión (Leads-Off)
El AD8232 inyecta corrientes microscópicas de sondeo ($< 100\text{ nA}$) en los electrodos. Si un electrodo se despega:
- Si `LO+ == HIGH` o `LO- == HIGH`: Hay falla de contacto cutáneo; la señal analógica es inválida y se dispara la alerta: **"ELECTRODO DESCONECTADO"**.
- Si ambos están en `LOW`: El contacto es óptimo y la señal en `A0` corresponde al ECG real del paciente.

---

## 4. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Muestreando la señal analógica en `A0` e inspeccionando `D10` y `D11`.
2. **Generador de Señales:** Simula el biopotencial ECG centrado en $1.65\text{ V}$.
3. **Interruptores Lógicos:** Conectados a `D10` y `D11` para simular el desprendimiento accidental de un electrodo.
4. **LED de Alarma en D13:** Se enciende de inmediato cuando algún electrodo se desconecta.

---

## 5. Código Fuente del Firmware (`firmware/ad8232_ecg_sensor.ino`)
Realiza la lectura conjunta a $100\text{ Hz}$ e imprime los valores en formato compatible con el **Serial Plotter** de Arduino IDE (`Herramientas > Serial Plotter`).

---

## 6. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/03-sensor-ecg-ad8232-biomedico/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Activa uno de los interruptores de electrodo: observa cómo la alarma de Leads-Off se activa en el Monitor Serie y el LED se enciende.
