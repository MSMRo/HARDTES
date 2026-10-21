# Proyecto 08: Integración Definitiva ESP32 y Node-RED mediante MQTT

## 1. Introducción y Relevancia en Telesalud
Este proyecto representa la arquitectura estándar de la industria en **Internet of Medical Things (IoMT)**. A diferencia del modelo HTTP (donde el microcontrolador debe abrir y cerrar conexiones TCP constantemente), **MQTT** mantiene un canal abierto y bidireccional de ultra bajo consumo energético.

En este proyecto aprenderás a:
- Implementar la librería **PubSubClient** en el ESP32.
- Publicar paquetes de signos vitales (SpO2, FC, Temperatura) en el tópico:
  `upch/telesalud/paciente_01/vitales`.
- Suscribir el ESP32 al canal de telecontrol y comandos remotos:
  `upch/telesalud/paciente_01/comandos`.
- Configurar **LWT** (*Last Will and Testament*) en el tópico de presencia:
  `upch/telesalud/paciente_01/estado` con mensaje `"OFFLINE_INESPERADO"`.
- Orquestar en Node-RED la recepción de telemetría, el disparo de alarmas y el envío interactivo de órdenes al hardware médico.

---

## 2. Arquitectura de Conectividad IoMT

```
                     +---------------------------------------+
                     |        BROKER MQTT (EMQX / Mosquitto) |
                     +---------------------------------------+
                       ^                                   ^
       Publica Vitals  |                                   |  Recibe Vitals
       Suscrito a Cmds |                                   |  Publica Cmds
                       v                                   v
             +--------------------+               +------------------+
             | ESP32 DevKit       |               | Servidor Node-RED|
             | Monitor Biomédico  |               | Dashboard UPCH   |
             +--------------------+               +------------------+
```

---

## 3. Matriz de Tópicos MQTT del Proyecto

| Tópico | Dirección | QoS | Propósito |
| :--- | :--- | :--- | :--- |
| `upch/telesalud/paciente_01/vitales` | ESP32 -> Node-RED | 1 | Telemetría periódica de constantes vitales en formato JSON. |
| `upch/telesalud/paciente_01/comandos`| Node-RED -> ESP32 | 1 | Órdenes de control: encendido de buzzer, cambio de tasa de muestreo. |
| `upch/telesalud/paciente_01/estado`  | Broker / ESP32    | 1 (Retain) | Mensaje de presencia ("ONLINE" al arrancar, "OFFLINE" al caer vía LWT). |

---

## 4. Dependencias de Librerías Arduino
En Arduino IDE (*Herramientas > Administrar Bibliotecas*):
1. **PubSubClient** (por Nick O'Leary).
2. **ArduinoJson** (por Benoit Blanchon).

---

## 5. Código Fuente del Firmware (`firmware/esp32_mqtt_telehealth.ino`)

El firmware implementa un bucle de reconexión automática sin bloqueo (*non-blocking reconnect*), atiende el callback de mensajes entrantes y despacha publicaciones periódicas.

---

## 6. Flujo Node-RED Acompañante (`nodered/flow.json`)

Contiene:
- Nodos `mqtt in` para monitoreo continuo de signos vitales y estado de conexión (LWT).
- Procesador de umbrales clínicos.
- Nodos `inject` y `mqtt out` para enviar comandos de activación de alarma remota directamente al ESP32.

---

## 7. Instrucciones de Compilación y Prueba

1. Abre [`firmware/esp32_mqtt_telehealth.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/08-esp32-nodered-mqtt/firmware/esp32_mqtt_telehealth.ino) en Arduino IDE.
2. Configura tu SSID y contraseña de Wi-Fi:
   ```cpp
   const char* WIFI_SSID = "TU_WIFI";
   const char* WIFI_PASS = "TU_PASSWORD";
   ```
3. Carga el firmware en el ESP32.
4. Abre [`nodered/flow.json`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/08-esp32-nodered-mqtt/nodered/flow.json) en Node-RED y haz clic en **Deploy**.
5. Observa en Node-RED cómo se reciben los signos vitales del ESP32 a través del broker en la nube.
6. En Node-RED, haz clic en el botón **Encender Alarma en ESP32**:
   - El mensaje viajará al broker MQTT y el ESP32 encenderá su LED físico en menos de 50 milisegundos.
7. Desconecta el cable USB del ESP32 para simular una falla de energía:
   - Observa cómo el Broker MQTT dispara de inmediato el mensaje de última voluntad (**LWT**) alertando a Node-RED sobre la desconexión del paciente.
