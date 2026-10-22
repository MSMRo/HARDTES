# Laboratorio 01: Sensor de Temperatura Corporal con Termistor NTC y Ecuación de Steinhart-Hart

## 1. Introducción y Relevancia en Telesalud
La temperatura corporal central es una de las cuatro constantes vitales primarias. En la telemonitorización de pacientes postquirúrgicos, febriles o neonatos en incubadoras, el sensor más utilizado en la práctica clínica por su bajo costo, diminuto tamaño y excelente sensibilidad es el **Termistor NTC (Negative Temperature Coefficient)**.

A diferencia de los sensores de silicio integrados, un termistor es un resistor semiconductor cuya resistencia eléctrica **decrece de forma no lineal (exponencial)** al aumentar la temperatura.

En este laboratorio aprenderás a:
- Conectar un termistor NTC de $10\text{ k}\Omega$ en configuración de divisor de tensión.
- Linealizar la respuesta térmica mediante el **Modelo Beta ($\beta$)** y la **Ecuación de Steinhart-Hart**.
- Calcular temperaturas clínicas con resolución de centésimas de grado Celsius ($^\circ\text{C}$).

---

## 2. Fundamento Matemático

### A. Ecuación del Divisor de Voltaje:
Con el termistor $R_{NTC}$ conectado a $V_{CC} = 5.0\text{ V}$ y una resistencia de referencia $R_{ref} = 10\text{ k}\Omega$ a GND:
$$V_{out} = V_{CC} \cdot \left( \frac{R_{ref}}{R_{NTC} + R_{ref}} \right)$$

Despejando la resistencia del termistor a partir del valor digital del ADC ($0-1023$):
$$R_{NTC} = R_{ref} \cdot \left( \frac{1023}{\text{ADC}} - 1 \right)$$

### B. Ecuación del Parámetro Beta ($\beta$):
La relación entre la resistencia a una temperatura $T$ (en Kelvin) y la resistencia nominal $R_0$ a $T_0 = 25^\circ\text{C}$ ($298.15\text{ K}$) es:
$$\frac{1}{T} = \frac{1}{T_0} + \frac{1}{\beta} \cdot \ln\left( \frac{R_{NTC}}{R_0} \right)$$

Donde:
- $R_0 = 10\,000\ \Omega$ a $T_0 = 298.15\text{ K}$ ($25^\circ\text{C}$).
- $\beta = 3950\text{ K}$ (constante típica del material semiconductor).
- Conversión a Celsius: $T(^\circ\text{C}) = T(\text{K}) - 273.15$.

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Muestrea la tensión en el pin analógico `A0`.
2. **Termistor NTC:** Modelo de $10\text{ k}\Omega$ con $\beta = 3950$.
3. **Resistencia de Precisión:** $10\text{ k}\Omega$ al $1\%$.
4. **Condensador de Desacoplo:** $100\text{ nF}$ en paralelo con la entrada analógica para derivar a tierra ruidos de alta frecuencia.
5. **Voltímetro Digital:** Monitorea la tensión de salida del divisor.

---

## 4. Código Fuente del Firmware (`firmware/ntc_steinhart_hart.ino`)
Realiza la adquisición con sobremuestreo, resuelve la ecuación matemática en coma flotante y clasifica estados de hipotermia, normotermia o fiebre.

---

## 5. Simulación en SimulIDE
1. Abre [`circuito.sim1`](./3_ARDUINO_UNO_SENSORES/01-sensor-ntc-steinhart-hart/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Modifica la temperatura del componente termistor en SimulIDE (o desliza el potenciómetro equivalente).
4. Verifica en el Monitor Serie cómo el logaritmo natural resuelve la no linealidad del sensor, entregando lecturas precisas de temperatura clínica en tiempo real.
