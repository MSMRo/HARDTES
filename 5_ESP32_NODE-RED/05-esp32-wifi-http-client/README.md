# Proyecto 05: ESP32 en Modo Estación y Cliente HTTP REST

## 1. Introducción y Relevancia en Telesalud
En este laboratorio el ESP32 abandona el modo AP para integrarse como un nodo cliente en la infraestructura de red local (**Modo Station - STA**). Una vez que el dispositivo obtiene una dirección IP del router hospitalario o residencial, actúa como un **Cliente HTTP REST**, capaz de reportar paquetes de constantes vitales serializados en formato JSON hacia servidores en la nube o pasarelas locales.

Aprenderás a:
- Conectar el ESP32 a una red Wi-Fi con reconexión automática en caso de pérdida de enlace.
- Utilizar la librería oficial `HTTPClient.h`.
- Construir y serializar objetos JSON biomédicos con la librería estándar **ArduinoJson** (v6 o v7).
- Realizar peticiones `HTTP POST` con cabeceras `Content-Type: application/json` y procesar el código de respuesta del servidor (200, 201, 500).

---

## 2. Arquitectura de Comunicación



---

## 3. Dependencias de Librerías Arduino
Asegúrate de tener instalada en el Arduino IDE la librería:
- **ArduinoJson** (por *Benoit Blanchon*). Disponible directamente en el menú *Herramientas > Administrar Bibliotecas*.

---

## 4. Estructura del JSON Transmitido por el ESP32
```json
{
  "nodo_id": "ESP32-UPCH-01",
  "paciente": "PAC-2026-A",
  "frecuencia_cardiaca": 74,
  "spo2": 98,
  "bateria_mv": 3950,
  "electrodos_conectados": true
}
```

---

## 5. Flujo Node-RED Receptor Acompañante (`nodered/flow.json`)

Contiene un endpoint HTTP `/api/telemetria` listo para recibir las peticiones POST del ESP32, validar la carga útil JSON y responder con un código `201 Created`.

---

## 6. Instrucciones de Compilación y Prueba

1. Abre [`firmware/wifi_http_client.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/05-esp32-wifi-http-client/firmware/wifi_http_client.ino) en Arduino IDE.
2. Modifica las credenciales de tu red Wi-Fi en el código:
   ```cpp
   const char* WIFI_SSID = "TU_RED_WIFI";
   const char* WIFI_PASSWORD = "TU_PASSWORD";
   ```
3. Configura la dirección IP de tu computadora (donde corre Node-RED o tu servidor de prueba):
   ```cpp
   const char* SERVER_URL = "http://192.168.1.50:1880/api/telemetria";
   ```
4. Sube el código y abre el Monitor Serie a `115200 baudios`.
5. Observa el proceso de negociación Wi-Fi, la obtención de IP y el envío periódico de telemetría HTTP cada 10 segundos.
