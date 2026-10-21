# Laboratorio 07: Lógica Digital Combinacional y Sistemas de Interbloqueo Médico

## 1. Introducción y Relevancia en Telesalud
En equipos médicos de soporte de vida y alto riesgo (desfibriladores, máquinas de diálisis, sistemas de radioterapia o incubadoras neonatales), la seguridad del paciente **nunca debe delegarse exclusivamente al software**. Un cuelgue de la CPU o un bug en el código podría disparar una descarga eléctrica accidental letal.

Por este motivo, las normativas internacionales de diseño electromédico (IEC 60601) exigen **circuitos de interbloqueo por hardware (Hardware Interlocks)** implementados mediante compuertas lógicas digitales dedicadas.

En este laboratorio aprenderás a:
- Analizar y simular compuertas lógicas fundamentales: **AND, OR, NOT, NAND y XOR**.
- Construir tablas de verdad en SimulIDE con interruptores y sondas de nivel lógico.
- Diseñar un circuito de seguridad de enclavamiento para autorización de descarga en un desfibrilador bifásico.

---

## 2. Fundamento Teórico y Tablas de Verdad

### A. Compuerta AND (Conjunción Lógica)
Salida en nivel alto ($1$) **únicamente** si todas sus entradas están en $1$:
$$Y = A \cdot B$$

| Entrada A | Entrada B | Salida Y (AND) | Salida Y (NAND) |
| :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | 1 |
| 0 | 1 | 0 | 1 |
| 1 | 0 | 0 | 1 |
| 1 | 1 | **1** | **0** |

---

### B. Compuerta OR (Disyunción Lógica)
Salida en nivel alto si **al menos una** de sus entradas está en $1$:
$$Y = A + B$$

---

### C. Compuerta NOT (Inversor Lógico)
Invierte el nivel lógico de la entrada:
$$Y = \overline{A}$$

---

## 3. Caso Clínico: Interbloqueo de Seguridad de Desfibrilador

### Requisitos de Seguridad Médica:
La descarga de $200\text{ Joules}$ solo puede autorizarse si se cumplen simultáneamente:
1. **Sensor de Contacto Firme ($A = 1$):** Impedancia torácica entre $25\ \Omega$ y $150\ \Omega$.
2. **Pulsador de Descarga del Médico ($B = 1$):** El operador presiona el gatillo.
3. **Sensor de Movimiento / Ritmo No Desfibrilable ($C = 0$):** No hay ritmo no choqueable (asistolia). Mediante un inversor NOT, $\overline{C} = 1$.

### Ecuación Booleana de Activación:
$$\text{DESCARGA} = A \cdot B \cdot \overline{C}$$

```
  Sensor Contacto Torácico (A) >----+
                                     +----[ AND-1 ]----+
  Pulsador Médico (B) --------->----+                 |
                                                       +----[ AND-2 ]----> DISPARO DESFIBRILADOR (LED Rojo)
  Ritmo No Choqueable (C) ---->[ NOT ]----------------+
```

---

## 4. Elementos en SimulIDE (`circuito.sim1`)
1. **Interruptores Lógicos (Switches con Pull-Down):** Entradas $A$, $B$, $C$.
2. **Compuerta NOT (Inversor 74HC04).**
3. **Compuertas AND (74HC08).**
4. **Sonda de Voltaje / Probador Lógico:** Indica el estado $0$ o $1$ en cada etapa.
5. **LED Indicador de Autorización:** Representa el relé de descarga de alta energía.

---

## 5. Procedimiento de Simulación en SimulIDE

1. Carga [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/07-compuertas-logicas-digitales/circuito.sim1) en SimulIDE.
2. Presiona **Power Circuit**.
3. Prueba todas las combinaciones de interruptores:
   - Si el médico presiona el pulsador ($B=1$) pero las palas no hacen buen contacto ($A=0$), la salida permanece en $0$ (Seguridad OK).
   - Si hay buen contacto ($A=1$) y se pulsa el botón ($B=1$), pero el sensor detecta ritmo no choqueable ($C=1$), el inversor produce un $0$ bloqueando la descarga.
   - Solo cuando $A=1$, $B=1$ y $C=0$, la salida conmuta a nivel lógico alto ($5\text{ V}$), encendiendo el LED de descarga autorizada.
