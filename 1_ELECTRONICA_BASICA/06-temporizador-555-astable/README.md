# Laboratorio 06: Temporizador NE555 en Modo Astable (Generador de Pulsos / Marcapasos)

## 1. Introducción y Relevancia en Telesalud
El circuito integrado **NE555** es uno de los temporizadores y osciladores más icónicos y confiables de la electrónica. En ingeniería biomédica y telesalud se utiliza para:
- Generar señales de reloj periódicas para sincronizar convertidores analógico-digitales.
- Simular el pulso rítmico de un **nodo sinoauricular (marcapasos biológico)** a una frecuencia cardíaca fija (ej. $1\text{ Hz} = 60\text{ lpm}$ o $1.2\text{ Hz} = 72\text{ lpm}$).
- Generar tonos de alerta acústica intermitente para monitores de signos vitales.

En este laboratorio aprenderás a:
- Conectar y dimensionar el temporizador 555 en modo **Astable** (oscilador libre continuo).
- Calcular los tiempos en estado alto ($T_H$), bajo ($T_L$), el período total ($T$) y el ciclo de trabajo (*Duty Cycle*).
- Medir la frecuencia resultante con el osciloscopio virtual en SimulIDE.

---

## 2. Fundamento Teórico y Ecuaciones de Diseño

En el modo astable, el condensador $C$ se carga a través de $(R_A + R_B)$ hasta que su tensión alcanza $\frac{2}{3} V_{CC}$, activando el comparador interno que conmuta la salida a nivel bajo y descarga $C$ a través de $R_B$ hacia el pin de descarga (pin 7) hasta llegar a $\frac{1}{3} V_{CC}$.

```
                     +--- VCC (5V)
                     |
                    [ ] RA (10 kΩ)
                     |
                     +------- Pin 7 (Discharge)
                     |
                    [ ] RB (47 kΩ)
                     |
                     +------- Pin 6 (Threshold) & Pin 2 (Trigger)
                     |
                    --- C (10 µF)
                    ---
                     |
                    GND
```

### Ecuaciones Matemáticas:
1. **Tiempo en nivel Alto ($T_H$):**
   $$T_H = \ln(2) \cdot (R_A + R_B) \cdot C \approx 0.693 \cdot (R_A + R_B) \cdot C$$

2. **Tiempo en nivel Bajo ($T_L$):**
   $$T_L = \ln(2) \cdot R_B \cdot C \approx 0.693 \cdot R_B \cdot C$$

3. **Período Total ($T$):**
   $$T = T_H + T_L = 0.693 \cdot (R_A + 2 R_B) \cdot C$$

4. **Frecuencia de Oscilación ($f$):**
   $$f = \frac{1}{T} = \frac{1.44}{(R_A + 2 R_B) \cdot C}$$

#### Dimensionamiento para Ritmo Cardíaco Fisiológico ($72\text{ lpm} = 1.2\text{ Hz}$):
- $C = 10\ \mu\text{F}$
- $R_A = 10\text{ k}\Omega$
- $R_B = 47\text{ k}\Omega$
$$T = 0.693 \cdot (10\text{ k} + 2 \cdot 47\text{ k}) \cdot 10\ \mu\text{F} \approx 0.693 \cdot 104000 \cdot 10^{-5} \approx 0.72\text{ s}$$
$$f \approx \frac{1}{0.72\text{ s}} \approx 1.38\text{ Hz} \quad (\approx 83\text{ lpm})$$

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Temporizador 555:** Bloque integrado.
2. **Resistencias:** $R_A = 10\text{ k}\Omega$, $R_B = 47\text{ k}\Omega$, $R_{LED} = 330\ \Omega$.
3. **Condensadores:** $C = 10\ \mu\text{F}$ (temporización) y $C_2 = 10\text{ nF}$ (desacoplo pin 5 control).
4. **LED Indicador de Pulso:** Visualiza los latidos simulados.
5. **Osciloscopio Virtual:** Conectado a la salida (Pin 3) y al condensador (Pin 2/6) para visualizar la forma de onda cuadrada y la carga/descarga exponencial.

---

## 4. Procedimiento de Simulación en SimulIDE

1. Carga [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/06-temporizador-555-astable/circuito.sim1) en SimulIDE.
2. Presiona **Power Circuit**.
3. Observa cómo el LED parpadea periódicamente como el pulso de un monitor cardíaco.
4. Abre el **Osciloscopio**:
   - Observa la onda de salida cuadrada de $0\text{ a }5\text{ V}$.
   - Mide el ancho del pulso alto y bajo para confirmar los cálculos teóricos.
