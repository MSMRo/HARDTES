# Laboratorio 06: Timers por Hardware e Interrupciones (ISR) para Adquisición Determinística

## 1. Introducción y Relevancia en Telesalud
En el procesamiento de señales biomédicas (como el cálculo de la variabilidad del ritmo cardíaco - HRV o el filtrado digital de un ECG), el **tiempo entre muestras debe ser rigurosamente constante (isócrono)**. Si el muestreo se realiza dentro de la función `loop()`, cualquier retraso por procesamiento o comunicación introduce fluctuaciones temporales aleatorias conocidas como **Jitter**, invalidando el análisis espectral (FFT).

Además, eventos médicos críticos (como la pulsación del **botón de pánico del paciente** o la detección de asistolia) no pueden esperar a que la CPU termine sus tareas normales: requieren atención inmediata mediante **Interrupciones por Hardware (Interrupts)**.

En este laboratorio aprenderás a:
- Configurar **Interrupciones Externas (`INT0` / Pin D2)** para eventos de emergencia médica.
- Configurar el **Timer1 de 16 bits en modo CTC (Clear Timer on Compare Match)** para muestreo periódico a $100\text{ Hz}$ exactos ($10\text{ ms}$).
- Escribir **Rutinas de Servicio de Interrupción (ISR)** robustas y seguras en C/C++.
- Comprender el modificador de compilador `volatile` y la sincronización con secciones críticas atómicas.

---

## 2. Fundamento Teórico y Matemáticas del Timer1

El microcontrolador ATmega328P opera a una frecuencia de reloj principal:
$$f_{CPU} = 16\text{ MHz} = 16\,000\,000\text{ Hz}$$

### Modo CTC del Timer1 (16 bits):
El temporizador cuenta desde $0$ hasta el valor cargado en el registro de comparación `OCR1A`, momento en el cual genera la interrupción `TIMER1_COMPA_vect` y se reinicia automáticamente a cero.

$$\text{Frecuencia de Interrupción} = \frac{f_{CPU}}{\text{Prescaler} \cdot (1 + \text{OCR1A})}$$

Despejando el registro de comparación `OCR1A`:
$$\text{OCR1A} = \frac{f_{CPU}}{\text{Prescaler} \cdot f_{deseada}} - 1$$

#### Cálculo para Muestreo Biomédico a $100\text{ Hz}$ (cada $10\text{ ms}$):
Eligiendo un prescaler de $64$:
$$\text{OCR1A} = \frac{16\,000\,000}{64 \cdot 100} - 1 = \frac{250\,000}{100} - 1 = 2500 - 1 = \mathbf{2499}$$

---

## 3. Reglas Críticas de Programación en ISRs
1. **Brevedad:** Una ISR debe ejecutarse en microsegundos (activar una bandera o guardar un dato en buffer).
2. **Modificador `volatile`:** Toda variable compartida entre la ISR y el bucle principal `loop()` debe declararse como `volatile` para evitar que el optimizador del compilador la mantenga en un registro de CPU.
3. **Prohibiciones:** Jamás invocar `delay()`, `delayMicroseconds()` prolongados, ni operaciones extensas de `Serial.print()` dentro de la ISR.

---

## 4. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:**
   - **Pin D2 (INT0):** Conectado a un pulsador de emergencia médica hacia GND con pull-up.
   - **Pin D13:** LED indicador conmutado por la ISR del Timer1 a $100\text{ Hz}$ (o división a $1\text{ Hz}$).
2. **Pulsador de Emergencia:** Dispara la interrupción externa instantánea.
3. **Osciloscopio Virtual:** Verifica la precisión exacta del período de $10\text{ ms}$ sin jitter.

---

## 5. Código Fuente del Firmware (`firmware/timers_interrupts.ino`)
Combina la interrupción externa `attachInterrupt()` para el botón de paro cardiorrespiratorio con la interrupción por registros del Timer1 para adquisición de muestras periódicas.

---

## 6. Simulación en SimulIDE
1. Abre [`circuito.sim1`](./2_ARDUINO_UNO_C%2B%2B/06-timers-e-interrupciones/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Abre el Monitor Serie y presiona el pulsador en el pin D2:
   - Observa la respuesta inmediata con interrupción de emergencia independientemente de lo que esté ejecutando el programa principal.
