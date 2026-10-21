# Proyecto 06: Comunicación Bidireccional ESP32 con Node-RED vía Puerto Serie (UART / USB)

## 1. Introducción y Relevancia en Telesalud
Aunque las tecnologías inalámbricas son ideales para monitores vestibles, en unidades de cuidados intensivos (UCI), quirófanos o estaciones fijas de hemodiálisis, la conexión física directa por **cable Serial USB (UART)** sigue siendo la preferida por tres motivos fundamentales:
1. **Inmunidad Electromagnética:** Cero interferencia por señales RF o saturación del espectro Wi-Fi.
2. **Latencia Determinística:** Adquisición de señales biológicas a tasas elevadas (ej. 250 a 500 Hz para ECG) sin paquetes perdidos por contención de canal.
3. **Ciberseguridad y Aislamiento:** El dispositivo no está expuesto a la red corporativa o a Internet.

En este proyecto aprenderás a:
- Implementar un protocolo de comunicación bidireccional serial en el ESP32 a **115200 baudios**.
- Emitir tramas de telemetría estructuradas en formato JSON delimitadas por salto de línea (`\n`).
- Recibir y parsear comandos en tiempo real provenientes de Node-RED (encender LED de alarma, solicitar calibración de sensor).
- Utilizar los nodos **Serial In** y **Serial Out** en Node-RED (`node-red-node-serialport`).

---

## 2. Arquitectura de Interconexión

```
 +-------------------------------------+                  +-----------------------------------+
 |             ESP32 SoC               |                  |             Node-RED              |
 |                                     |                  |          (PC / Raspberry Pi)      |
 |  [ Emisión ]                        |   Cable USB/UART |                                   |
 |  Telemetría JSON ---> UART TX (GPIO1)| ---------------> | [ Serial In ] ---> [ JSON Parser ]|
 |                                     |   (115200 baud)  |                           |       |
 |  [ Recepción ]                      |                  |                           v       |
 |  Comandos <---------- UART RX (GPIO3)| <--------------- | [ Serial Out ] <--- [ Dashboard ] |
 +-------------------------------------+                  +-----------------------------------+
```

---

## 3. Protocolo de Comandos Soportados por el Firmware

El ESP32 escucha continuamente por su puerto serie. Al recibir un paquete JSON terminado en `\n`, ejecuta las siguientes acciones:

| Comando JSON | Acción en el Hardware |
| :--- | :--- |
| `{"comando":"ALARMA_ON"}` | Activa el LED / Buzzer del GPIO 2. |
| `{"comando":"ALARMA_OFF"}` | Apaga el LED / Buzzer. |
| `{"comando":"CALIBRAR"}` | Reinicia las medias de referencia de los sensores biomédicos. |

---

## 4. Instalación del Nodo Serial en Node-RED
Para utilizar el puerto serie físico en Node-RED:
1. Dirígete a `☰` > **Manage palette** > **Install**.
2. Busca: `node-red-node-serialport` y presiona **Install**.
3. En la configuración del nodo en Node-RED, selecciona el puerto COM de tu ESP32 (ej. `COM3` en Windows o `/dev/ttyUSB0` en Linux) y fija la velocidad en `115200` baudios, dividido por `\n`.

---

## 5. Código del Firmware (`firmware/serial_telemetry.ino`)

El firmware ejecuta el bucle de envío no bloqueante cada 1 segundo mientras procesa de forma asíncrona cualquier byte disponible en el buffer serie `Serial.available()`.

---

## 6. Flujo Node-RED Acompañante (`nodered/flow.json`)

Contiene el flujo completo con nodos Serial In y Serial Out configurados, parsing automático de la telemetría recibida e inyectores de comandos interactivos.

---

## 7. Instrucciones de Compilación y Puesta en Marcha

1. Sube [`firmware/serial_telemetry.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/06-esp32-nodered-serial/firmware/serial_telemetry.ino) al ESP32.
2. **Cierra el Monitor Serie de Arduino IDE** (obligatorio, ya que Windows no permite que dos aplicaciones abran el mismo puerto COM simultáneamente).
3. Importa [`nodered/flow.json`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/06-esp32-nodered-serial/nodered/flow.json) en Node-RED.
4. Haz doble clic en el nodo **Serial In**, edita la configuración del puerto seleccionando tu puerto COM real a `115200` y pulsa **Deploy**.
5. Observa las constantes vitales llegando en tiempo real al panel Debug y presiona los botones para controlar el LED del ESP32 a través del cable USB.
