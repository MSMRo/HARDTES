# Laboratorio 03: Diodos Semiconductores, Rectificación y Protección Zener

## 1. Introducción y Relevancia en Telesalud
Todo dispositivo médico portátil o de cabecera requiere una fuente de alimentación DC limpia y estable. Los biosensores y microcontroladores son extremadamente vulnerables a:
1. **Inversión de polaridad de la batería:** Puede destruir instantáneamente el circuito integrado.
2. **Sobretensiones y descargas electrostáticas (ESD):** Ocurren frecuentemente cuando el personal o el paciente tocan los electrodos.

En este laboratorio aprenderás a:
- Analizar la curva característica del diodo de silicio (caída en conducción directa de $\approx 0.7\text{ V}$).
- Simular un circuito de **rectificación con condensador de filtrado** para convertir señal alterna a corriente continua limpia.
- Implementar un circuito de **protección por supresión con Diodo Zener** que limite el voltaje a un máximo seguro ($3.3\text{ V}$ o $5.1\text{ V}$) antes de ingresar al pin del microcontrolador.

---

## 2. Fundamento Teórico

### A. Rectificación y Filtrado
El diodo solo permite el flujo de corriente en sentido ánodo $\to$ cátodo cuando $V_{ánodo} - V_{cátodo} \ge V_D$ ($\approx 0.7\text{ V}$):
$$V_{pico\_DC} = V_{pico\_AC} - V_D$$
Al colocar un condensador electrolítico en paralelo con la carga, este almacena carga durante el pico y la entrega en el valle, reduciendo el rizado (*ripple*):
$$V_{rizado\ (p-p)} \approx \frac{I_{carga}}{f \cdot C}$$

### B. Regulador y Limitador con Diodo Zener
El diodo Zener opera en la región de ruptura inversa ($V_Z$). Cuando el voltaje de entrada supera $V_Z$, el diodo conduce corriente manteniendo la tensión de salida clavada en $V_Z$, protegiendo las entradas del ADC del Arduino o ESP32.

```
            R_limitadora (1 kΩ)
  Vin >--------/\/\/\/\--------+---------> Vout_protegido (Máx 3.3V)
                               |
                              ---  Diodo Zener (3.3V)
                              / \  (Cátodo hacia arriba)
                              ---
                               |
                              GND
```

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Generador AC:** Onda senoidal de $10\text{ V}$ pico a $50\text{ Hz}$.
2. **Diodo 1N4007:** Para rectificación de media onda.
3. **Condensador de Filtro:** $100\ \mu\text{F}$.
4. **Resistencia de Carga:** $1\text{ k}\Omega$.
5. **Diodo Zener de Protección:** $3.3\text{ V}$ (para líneas de telemetría de 3.3V del ESP32).
6. **Osciloscopio / Probes:** Muestra la onda AC antes del diodo, la onda rectificada y la tensión estabilizada de salida.

---

## 4. Procedimiento de Simulación en SimulIDE

1. Carga [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/03-diodos-rectificacion-fuente/circuito.sim1) en SimulIDE.
2. Presiona **Power Circuit**.
3. Abre el **Osciloscopio**:
   - Observa cómo la onda senoidal negativa es bloqueada por el diodo rectificador.
   - Observa el efecto del condensador suavizando el rizado.
   - En la sección del Zener, observa cómo tensiones de entrada que alcanzan $8\text{ V}$ son recortadas limpiamente a $3.3\text{ V}$, garantizando que el ADC del microcontrolador jamás sufra sobretensión destructiva.
