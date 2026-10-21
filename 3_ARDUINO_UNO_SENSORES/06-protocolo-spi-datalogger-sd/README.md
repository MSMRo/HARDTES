# Laboratorio 06: Protocolo Hardware SPI y Registro Continuo en Tarjeta MicroSD (Holter Clínico)

## 1. Introducción y Relevancia en Telesalud
Cuando se diseñan dispositivos biomédicos ambulatorios —como un **Monitor Holter ECG de 24 o 48 horas**, un oxímetro de sueño para apnea o una grabadora electroencefalográfica—, no siempre es viable o seguro transmitir cada muestra por Wi-Fi o Bluetooth:
1. El consumo de energía de la radio agotaría la batería del paciente en pocas horas.
2. Si el paciente camina fuera de cobertura o ingresa a un ascensor, los datos se perderían.

Por ello, la arquitectura de referencia en telesalud incluye un **Data Logger de alta velocidad en memoria MicroSD** conectado mediante el **Protocolo Hardware SPI**.

En este laboratorio aprenderás a:
- Conocer la arquitectura del bus sincrónico **SPI (Serial Peripheral Interface)** y sus cuatro líneas (**MOSI, MISO, SCK, CS**).
- Comprender los modos de reloj (`CPOL`, `CPHA`) y velocidades de transferencia de hasta $8\text{ MHz}$.
- Utilizar las librerías oficiales `SPI.h` y `SD.h` para inicializar tarjetas SD en formato FAT16/FAT32.
- Registrar series temporales clínicas continuas en un archivo estructurado `HOLTER.CSV`.

---

## 2. Fundamento Físico y Líneas del Bus SPI

A diferencia de I2C (que usa 2 líneas semidúplex con arbitraje y direcciones), SPI es un bus **Full-Dúplex de 4 líneas dedicadas**:

```
  Arduino Uno (Maestro SPI)                   Módulo MicroSD / ADC Rápido
  +-------------------------+                 +-------------------------+
  |    MOSI (Pin D11)       | --------------> | MOSI / DI (Data In)     |
  |    MISO (Pin D12)       | <-------------- | MISO / DO (Data Out)    |
  |    SCK  (Pin D13)       | --------------> | CLK (Serial Clock)      |
  |    CS   (Pin D10 / SS)  | --------------> | CS / SS (Chip Select)   |
  +-------------------------+                 +-------------------------+
```

### Líneas del Bus:
1. **MOSI (Master Out Slave In):** Datos emitidos por el Arduino hacia el periférico.
2. **MISO (Master In Slave Out):** Datos devueltos por el periférico al Arduino.
3. **SCK (Serial Clock):** Señal de reloj generada por el maestro para sincronizar cada bit transmitido.
4. **CS / SS (Chip Select):** Línea activa en nivel **BAJO (`LOW`)** que selecciona de forma exclusiva con qué dispositivo se desea conversar.

---

## 3. Estructura del Archivo de Auditoría (`HOLTER.CSV`)

Cada segundo, el firmware anexa una línea con las muestras adquiridas:
```csv
timestamp_ms,paciente_id,bpm,spo2,temp_c,estado
1000,PAC_HOLTER_01,74,98,36.6,OK
2000,PAC_HOLTER_01,75,98,36.6,OK
3000,PAC_HOLTER_01,118,89,37.1,ARRITMIA_DETECTADA
```

---

## 4. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Pines SPI por hardware (`D10` CS, `D11` MOSI, `D12` MISO, `D13` SCK).
2. **Lector de Tarjeta SD / Periférico SPI:** Módulo conectado al bus SPI.
3. **Osciloscopio Virtual de 2 Canales:**
   - **Canal 1:** Reloj SPI de alta velocidad (`SCK`).
   - **Canal 2:** Línea de datos (`MOSI`).

---

## 5. Código Fuente del Firmware (`firmware/spi_sd_datalogger.ino`)
Inicializa la tarjeta en el pin `CS = 10`, verifica la presencia de la tarjeta, escribe la cabecera CSV si es nueva y anexa periódicamente las constantes vitales con volcado seguro a disco (`flush()`).

---

## 6. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/06-protocolo-spi-datalogger-sd/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Observa en el Monitor Serie la inicialización de la tarjeta SD y el registro progresivo de filas de telemetría médica en el archivo de registro.
