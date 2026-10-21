# Laboratorio 04: Transistor BJT en Modo Conmutación (Switch) y Control de Actuadores

## 1. Introducción y Relevancia en Telesalud
Los pines de salida digital de microcontroladores biomédicos (Arduino Uno o ESP32) entregan típicamente un máximo de $20\text{ a }40\text{ mA}$ a $3.3\text{ V}$ o $5\text{ V}$. Sin embargo, en un entorno hospitalario se requiere activar actuadores de potencia:
- **Zumbadores/Sirenas de alarma UCI:** Consumen más de $150\text{ mA}$ a $12\text{ V}$.
- **Válvulas solenoides para ventiladores mecánicos:** Requieren picos de corriente inductiva elevados.
- **Relés electromecánicos:** Para aislamiento galvánico de alta tensión.

El **Transistor BJT (Bipolar Junction Transistor) NPN** operando en regiones de **Corte** y **Saturación** actúa como un interruptor electrónico de alta velocidad controlado por una corriente de base diminuta.

---

## 2. Fundamento Teórico

### A. Región de Corte (Switch Abierto - OFF)
Cuando la tensión de control en la base es nula ($V_{BE} < 0.6\text{ V}$):
$$I_B = 0 \implies I_C \approx 0 \implies V_{CE} \approx V_{CC}$$
El actuador médico permanece completamente desenergizado.

### B. Región de Saturación (Switch Cerrado - ON)
Cuando se inyecta suficiente corriente de base ($I_B$) desde el pin digital del microcontrolador:
$$I_B \ge \frac{I_{C(sat)}}{\beta_{min}}$$
$$V_{CE(sat)} \approx 0.2\text{ V} \implies V_{carga} \approx V_{CC} - 0.2\text{ V}$$
El actuador recibe prácticamente la totalidad del voltaje de la fuente externa de potencia.

### C. Diodo Flyback (Rueda Libre) para Cargas Inductivas
Al desconectar una bobina (relé, motor, solenoide), la ley de Faraday-Lenz genera un pulso de alto voltaje inductivo inverso:
$$V_L = -L \frac{di}{dt}$$
Este pico (a menudo $> 100\text{ V}$) destruiría el transistor al instante. Un diodo conectado en antiparalelo con la carga absorbe esta energía recirculante con total seguridad.

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Interruptor Lógico / Fuente de Control:** Señal de $5\text{ V}$ simulando el pin `GPIO` de Arduino.
2. **Resistencia de Base ($R_B$):** $1\text{ k}\Omega$ (limita la corriente del pin digital a $\approx 4.3\text{ mA}$).
3. **Transistor BJT NPN:** Modelo equivalente 2N2222.
4. **Resistencia de Colector / Carga Médica ($R_C$):** $100\ \Omega$ alimentada desde fuente de $12\text{ V}$ (o relé con LED indicador).
5. **Diodo de Protección / Flyback:** 1N4148 / 1N4007.
6. **Sonda / Amperímetro:** Para verificar la saturación del transistor y la corriente de carga.

---

## 4. Procedimiento de Simulación en SimulIDE

1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/04-transistor-bjt-switch-rele/circuito.sim1) en SimulIDE.
2. Enciende el circuito (**Power Circuit**).
3. Con el interruptor abierto ($0\text{ V}$ en base):
   - Observa que la corriente de colector es $0\text{ mA}$ y el LED/alarma está apagado.
4. Cierra el interruptor ($5\text{ V}$ en base):
   - Fluye corriente a la base saturando el BJT.
   - La tensión $V_{CE}$ cae a $\approx 0.2\text{ V}$ y la corriente principal fluye por la carga activando el actuador médico.
