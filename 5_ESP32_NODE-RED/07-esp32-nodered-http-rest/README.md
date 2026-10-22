# Proyecto 07: Integración ESP32 con Node-RED mediante API REST (HTTP POST)

## 1. Introducción y Relevancia en Telesalud
La arquitectura REST sobre HTTP/HTTPS es el estándar más extendido en aplicaciones web y registros de salud electrónicos. En este laboratorio se implementa un enlace completo punto a punto donde **Node-RED funciona como el Servidor Receptor Central** y el **ESP32 actúa como el Dispositivo Médico Emisor**.

Aprenderás a:
- Implementar un servicio web REST en Node-RED que valide esquemas clínicos JSON.
- Programar el ESP32 para conectarse a la red Wi-Fi y transmitir paquetes de signos vitales mediante `HTTP POST` con cabecera `Content-Type: application/json`.
- Procesar la respuesta del servidor en el microcontrolador (deserializando el payload de respuesta enviado por Node-RED).
- Gestionar códigos de estado HTTP (201 Created vs 400 Bad Request).

---

## 2. Diagrama de Secuencia de la Comunicación



---

## 3. Estructura de los Paquetes JSON

### Petición del ESP32:
```json
{
  "device_mac": "24:6F:28:AB:CD:EF",
  "paciente_id": "PAC-2026-UPCH",
  "frecuencia_cardiaca": 75,
  "spo2": 98,
  "temperatura_c": 36.6,
  "bateria_pct": 92
}
```

### Respuesta de Node-RED:
```json
{
  "estado": "REGISTRADO",
  "codigo": 201,
  "alerta_clinica": "NINGUNA",
  "servidor_timestamp": "2026-10-20T17:45:00.000Z"
}
```

---

## 4. Código Fuente del Firmware (`firmware/esp32_rest_client.ino`)

El firmware gestiona la reconexión Wi-Fi periódica, empaqueta las lecturas cada 5 segundos y efectúa el `POST` procesando el resultado.

---

## 5. Flujo Node-RED Acompañante (`nodered/flow.json`)

Incluye el endpoint `POST /api/v1/telesalud/vitales`, la función de validación de campos, la respuesta HTTP 201 y una visualización en el panel de Debug.

---

## 6. Instrucciones de Compilación y Prueba

1. Averigua la dirección IP local de la computadora donde corre Node-RED (en Windows ejecuta en PowerShell: `ipconfig` y busca la `Dirección IPv4`, por ejemplo `192.168.1.15`).
2. Abre [`firmware/esp32_rest_client.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/07-esp32-nodered-http-rest/firmware/esp32_rest_client.ino) en Arduino IDE.
3. Actualiza el SSID, la contraseña Wi-Fi y la URL de destino:
   ```cpp
   const char* WIFI_SSID = "TU_WIFI";
   const char* WIFI_PASS = "TU_PASSWORD";
   const char* SERVER_ENDPOINT = "http://192.168.1.15:1880/api/v1/telesalud/vitales";
   ```
4. Importa [`nodered/flow.json`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/07-esp32-nodered-http-rest/nodered/flow.json) en Node-RED y pulsa **Deploy**.
5. Carga el firmware en el ESP32 y abre el Monitor Serie a `115200 baudios`.
6. Verifica que cada 5 segundos el ESP32 envíe la telemetría y reciba la confirmación del servidor con el timestamp de Node-RED.
