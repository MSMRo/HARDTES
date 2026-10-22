# Laboratorio 05: Modulación por Ancho de Pulsos (PWM) y Control de Actuadores

## 1. Introducción y Relevancia en Telesalud
Un microcontrolador digital solo puede entregar dos estados nativos: $0\text{ V}$ o $5\text{ V}$. Sin embargo, en equipos de soporte de vida se requiere modular gradualmente la energía entregada a actuadores:
- **Bombas de Infusión Intravenosa:** Regulación precisa de la velocidad del motor para dosificar microgotas de fármacos por hora.
- **Incubadoras y Cunas de Fototerapia Neonatal:** Ajuste fino de la intensidad de radiación lumínica y calefactores térmicos.
- **Estimuladores Eléctricos Neuromusculares (TENS):** Generación de trenes de pulsos de intensidad graduable.

La técnica de **Modulación por Ancho de Pulsos (PWM - Pulse Width Modulation)** permite generar un efecto analógico equivalente variando el porcentaje de tiempo que la señal permanece en estado alto durante cada período fijo.

---

## 2. Fundamento Teórico del PWM

### A. Ciclo de Trabajo (Duty Cycle)
El ciclo de trabajo ($D$) es la fracción de tiempo en que la señal digital está en $5\text{ V}$ ($T_{ON}$) respecto al período total ($T$):
$$D = \frac{T_{ON}}{T} \cdot 100\%$$

El voltaje medio o efectivo entregado a la carga es:
$$V_{promedio} = V_{CC} \cdot \frac{D}{100\%} = 5.0\text{ V} \cdot \left( \frac{\text{Valor PWM}}{255} \right)$$



### B. Pines PWM en el Arduino Uno (ATmega328P):
Los pines marcados con el símbolo `~` disponen de temporizadores por hardware para PWM:
- **Pines 5 y 6:** Controlados por el **Timer0** ($\approx 976.56\text{ Hz}$).
- **Pines 9 y 10:** Controlados por el **Timer1** ($\approx 490.20\text{ Hz}$).
- **Pines 3 y 11:** Controlados por el **Timer2** ($\approx 490.20\text{ Hz}$).

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Generando PWM en el pin digital `D9`.
2. **Potenciómetro de Ajuste:** Conectado al pin analógico `A0` para calibrar el flujo de la bomba de infusión ($0-100\%$).
3. **LED Actuador / Indicador de Flujo:** Con resistencia limitadora conectado a `D9`.
4. **Osciloscopio Virtual (`Oscope`):** Conectado a `D9` para observar la variación milimétrica del ancho del pulso $T_{ON}$.
5. **Voltímetro con Filtro:** Mide el voltaje promedio efectivo entregado al actuador.

---

## 4. Código Fuente del Firmware (`firmware/pwm_control.ino`)
Lee el valor analógico del potenciómetro ($0-1023$), lo escala con la función `map()` al rango PWM de 8 bits ($0-255$) y calcula la tasa de infusión equivalente en mililitros por hora ($\text{mL/h}$).

---

## 5. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/05-pwm-control-actuadores/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Abre el **Osciloscopio** y gira la perilla del potenciómetro:
   - Observa cómo la onda cuadrada en pantalla ensancha o adelgaza su cresta en tiempo real mientras el LED cambia suavemente su brillo.
