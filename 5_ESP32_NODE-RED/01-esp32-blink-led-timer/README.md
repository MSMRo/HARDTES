# Proyecto 01: Blink LED y Temporización No Bloqueante con `millis()`

## 1. Introducción y Relevancia en Telesalud
En el desarrollo de instrumentación biomédica (como electrocardiógrafos o pulsioxímetros), el microcontrolador no puede detener su ejecución. El uso de la función tradicional `delay()` congela el núcleo del procesador, lo que provocaría pérdida irrecuperable de muestras analógicas de señales fisiológicas de alta frecuencia.

En este proyecto aprenderás a:
- Controlar los pines de propósito general (**GPIO**) del ESP32 en modo salida.
- Implementar una máquina de estados temporizada utilizando **`millis()`** (temporización no bloqueante).
- Comprender la arquitectura de núcleos del ESP32 y el pinout del LED integrado (*Onboard LED*).
- Establecer un patrón de *Heartbeat* (latido de vida) para verificar que el firmware médico sigue respondiendo en tiempo real.

---

## 2. Diagrama de Conexiones de Hardware

En la mayoría de placas de desarrollo ESP32 (como la ESP32 WROOM-32 DevKit v1 de 30 o 38 pines), el LED integrado está conectado al **GPIO 2** (o GPIO 1 en algunos clones).

```
   ESP32 DevKit v1
  +---------------+
  |        [ESP32]|
  |               |
  | GPIO 2 >------+-----[ R = 330 Ω ]-----(+) [ LED Externo ]
  |               |                            |
  | GND    >------+----------------------------(-)
  +---------------+
```

> **Nota:** Puedes utilizar directamente el LED azul integrado en la tarjeta conectada a `GPIO 2`, o conectar un LED externo en serie con un resistor limitador de 220 Ω a 330 Ω hacia tierra (GND).

---

## 3. Código Fuente del Firmware (`firmware/blink_timer.ino`)

El código implementa dos tareas concurrentes en un solo hilo sin interferirse:
1. Conmutar el LED a una frecuencia fija (500 ms) como indicador de salud del sistema.
2. Imprimir por el puerto Serial el tiempo transcurrido en milisegundos y el estado de operación.

---

## 4. Flujo Node-RED Acompañante (`nodered/flow.json`)

Para familiarizarse con el concepto de latido (*heartbeat*) en la nube, el flujo de Node-RED implementa un detector de presencia de pulso que vigila los mensajes periódicos y activa una alerta si transcurren más de 3 segundos sin recibir un latido del microcontrolador.

---

## 5. Instrucciones de Compilación y Carga

1. Abre el archivo [`firmware/blink_timer.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/01-esp32-blink-led-timer/firmware/blink_timer.ino) en **Arduino IDE**.
2. En el menú **Herramientas > Placa**, selecciona: `ESP32 Dev Module` (o tu modelo específico).
3. Selecciona el **Puerto COM** asignado por Windows al conectar tu cable micro-USB o USB-C.
4. Presiona el botón **Subir** (Upload). *(Si tu placa requiere presionar el botón `BOOT`, mantenlo presionado cuando aparezca `Connecting........___` hasta que inicie la escritura).*
5. Abre el **Monitor Serie** a `115200 baudios` para verificar los mensajes de diagnóstico.
