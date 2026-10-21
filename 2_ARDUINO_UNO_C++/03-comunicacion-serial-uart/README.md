# Laboratorio 03: Comunicación Serial UART y Servidor de Comandos Clínicos

## 1. Introducción y Relevancia en Telesalud
El puerto **UART (Universal Asynchronous Receiver-Transmitter)** por hardware en los pines `D0` (RX) y `D1` (TX) del Arduino Uno es el canal principal para enlazar el microcontrolador con:
- Pasarelas IoT locales (como Raspberry Pi con Node-RED).
- Módulos de telemetría inalámbrica (Bluetooth HC-05/BLE, módems 4G/LTE hospitalarios).
- Monitores de cabecera que requieren control por comandos de texto o tramas binarias.

En este laboratorio aprenderás a:
- Comprender el funcionamiento del hardware UART del ATmega328P.
- Evitar el desbordamiento del buffer de recepción serie de 64 bytes.
- Programar un **analizador sintáctico (*parser*) de comandos de texto en C++** que responda a peticiones del personal de enfermería o software de telemetría.

---

## 2. Parámetros de la Trama UART Asíncrona (8N1)
- **Baud Rate:** 115200 bps ($8.68\ \mu\text{s}$ por bit).
- **Bits de Datos:** 8 bits.
- **Paridad:** Ninguna (None).
- **Bits de Parada:** 1 bit (Stop Bit).

```
   IDLE (1)      START (0)      D0      D1      D2      D3      D4      D5      D6      D7     STOP (1)
  -----------+             +-------+-------+-------+-------+-------+-------+-------+-------+-------------
             |_____________|                                                               |
              1 bit time                                                                    1 bit time
```

---

## 3. Protocolo de Comandos Soportados por el Firmware

| Comando Enviado | Respuesta del Arduino Uno | Acción Clínica en Hardware |
| :--- | :--- | :--- |
| `GET_STATUS` | `{"uptime":..., "estado":"OK"}` | Reporta estado del hardware médico. |
| `GET_VITALS` | `{"bpm":74, "spo2":98, "temp":36.6}` | Envía muestra instantánea de constantes. |
| `ALARM_ON` | `ACK: ALARMA ACTIVADA` | Enciende LED de emergencia en pin D13. |
| `ALARM_OFF` | `ACK: ALARMA APAGADA` | Silencia el indicador de alarma. |
| `HELP` | Lista de comandos disponibles | Menú de ayuda para el operador. |

---

## 4. Código Fuente del Firmware (`firmware/serial_uart_commands.ino`)

El firmware implementa un receptor asíncrono no bloqueante basado en `Serial.available()` y lectura de strings delimitados por salto de línea (`\n`), evitando llamadas bloqueantes que congelen el ciclo de ejecución.

---

## 5. Simulación en SimulIDE (`circuito.sim1`)
El circuito incluye un Arduino Uno y un terminal serial conectado a los pines RX/TX para enviar comandos interactivos y visualizar las respuestas en tiempo real.
