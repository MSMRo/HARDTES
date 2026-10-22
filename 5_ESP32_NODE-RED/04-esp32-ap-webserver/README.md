# Proyecto 04: ESP32 en Modo SoftAP con Servidor Web HTTP Embebido

## 1. Introducción y Relevancia en Telesalud
Un monitor de signos vitales no siempre tiene acceso a un router o una pasarela de datos. En emergencias prehospitalarias (ambulancias o visitas domiciliarias), es imprescindible que el médico pueda visualizar los datos de inmediato en su propio teléfono móvil sin instalar ninguna aplicación adicional.

En este proyecto aprenderás a:
- Implementar la librería `WebServer.h` en el ESP32 en el puerto estándar `80`.
- Servir una página web HTML5 responsiva con estilos CSS modernos almacenada directamente en la memoria Flash del microcontrolador.
- Enlazar rutas HTTP (`/`, `/alarma/on`, `/alarma/off`, `/api/vitales`) a funciones en C++.
- Controlar actuadores de alarma (LED del GPIO 2) directamente desde la interfaz web del paciente.

---

## 2. Arquitectura de Navegación



---

## 3. Endpoints Servidos por el ESP32
- **`GET /`**: Retorna el documento HTML principal con la tarjeta clínica y los botones de acción.
- **`GET /alarma/on`**: Enciende el LED de alerta en el GPIO 2 y redirige a `/`.
- **`GET /alarma/off`**: Apaga el LED y redirige a `/`.
- **`GET /api/vitales`**: Retorna un payload JSON con las mediciones instantáneas para polling o integración.

---

## 4. Código Fuente del Firmware (`firmware/ap_webserver.ino`)

El firmware combina `WiFi.softAP()` con los manejadores de ruta `server.on()` y `server.handleClient()` dentro del bucle principal `loop()`.

---

## 5. Flujo Node-RED Acompañante (`nodered/flow.json`)

Contiene un flujo de consulta periódica mediante peticiones HTTP que recupera el JSON del endpoint `/api/vitales` servido por el ESP32 para verificar la disponibilidad del servidor web embebido.

---

## 6. Instrucciones de Compilación y Prueba

1. Sube [`firmware/ap_webserver.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/04-esp32-ap-webserver/firmware/ap_webserver.ino) al ESP32.
2. Abre el Monitor Serie a `115200 baudios`.
3. Desde tu teléfono inteligente o PC, conéctate a la red Wi-Fi:
   - **SSID:** `ESP32-Monitor-Web`
   - **Clave:** `Salud2026`
4. Abre cualquier navegador (Chrome, Safari, Firefox) e ingresa la dirección:
   ```
   http://192.168.4.1
   ```
5. Podrás ver la consola web clínica y presionar los botones para encender y apagar el LED indicador de alarma del ESP32 en tiempo real.
