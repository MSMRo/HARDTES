# Hardware para Telesalud
## Universidad Peruana Cayetano Heredia (UPCH)
### Facultad de Ciencias e Ingeniería — Ingeniería Biomédica

---

Bienvenido al repositorio oficial de prácticas y proyectos del curso **Hardware para Telesalud**. Este repositorio contiene una guía exhaustiva, modular y progresiva para el diseño, prototipado e integración de sistemas embebidos en el ámbito del **Internet of Medical Things (IoMT)** y la telemedicina.

El contenido está dividido en dos grandes bloques de aprendizaje:
1. **`4_NODE-RED/`**: Fundamentos, manipulación de flujos de datos, protocolos de red, bases de datos y dashboards de telemetría biomédica con Node-RED.
2. **`5_ESP32_NODE-RED/`**: Desde el dominio del microcontrolador ESP32 (periféricos, Wi-Fi, AP, servidores web embebidos) hasta su integración bidireccional con Node-RED mediante comunicaciones Serial (UART), HTTP REST y MQTT.

---

## 📚 Fundamentos Teóricos del Hardware y Software

### 1. El SoC ESP32 en el Ecosistema Biomédico

El **ESP32** es una familia de microcontroladores de tipo *System on a Chip* (SoC) de ultra bajo consumo y alto rendimiento desarrollada por **Espressif Systems**. Cuenta con conectividad inalámbrica nativa **Wi-Fi 802.11 b/g/n (2.4 GHz)** y **Bluetooth dual (v4.2 BR/EDR y BLE)**, lo que lo convierte en uno de los estándares de facto para el desarrollo de pasarelas médicas de bajo costo, dispositivos vestibles (*wearables*) y monitores de constantes vitales en telesalud.

```
                           +----------------------------------------+
                           |            ESP32 SoC Arquitectura      |
                           +----------------------------------------+
                               |               |               |
              +----------------+               |               +---------------+
              |                                |                               |
     [ CPU Xtensa LX6 ]            [ Memoria Interna ]               [ Conectividad RF ]
   Dual Core @ 160-240 MHz         - 520 KB SRAM                     - Wi-Fi 802.11 b/g/n
   Coprocesador ULP (Ultra         - 448 KB ROM                      - Bluetooth v4.2 + BLE
   Low Power) para Sleep Mode      - Flash Externa SPI (4-16 MB)     - Antena PCB / conector IPEX
              |                                |                               |
              +----------------+               |               +---------------+
                               |               |               |
                           +----------------------------------------+
                           |        Periféricos & Acondicionamiento |
                           +----------------------------------------+
                           - 2x ADC 12-bit SAR (18 canales)
                           - 2x DAC 8-bit
                           - 3x UART, 3x SPI, 2x I2C, 16x PWM (LEDC)
                           - 10x Entradas táctiles capacitivas
```

#### Familias y Variantes Principales del ESP32

| Modelo / Familia | Arquitectura de CPU | Frecuencia | Wi-Fi / Bluetooth | Memoria RAM | Periféricos Destacados | Caso de Uso en Telesalud |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **ESP32 Clásico** (WROOM-32 / WROVER) | Tensilica Xtensa Dual-Core 32-bit LX6 | Hasta 240 MHz | Wi-Fi 4 (2.4 GHz) + Bluetooth 4.2 / BLE | 520 KB SRAM (+ hasta 8MB PSRAM en WROVER) | 18x ADC (12-bit), 2x DAC (8-bit), 3x UART, 2x I2C, 10x Touch | Monitores multiparámetro, pasarelas de señales ECG/PPG con buffer PSRAM. |
| **ESP32-S2** | Xtensa Single-Core 32-bit LX7 | Hasta 240 MHz | Wi-Fi 4 (2.4 GHz) (Sin Bluetooth) | 320 KB SRAM | USB OTG nativo, 2x DAC (8-bit), Touch, Sensor de temperatura | Dispositivos USB de diagnóstico médico, sensores de bajo costo sin BLE. |
| **ESP32-S3** | Xtensa Dual-Core 32-bit LX7 con extensiones vectoriales | Hasta 240 MHz | Wi-Fi 4 + BLE 5.0 (Mesh, Long Range) | 512 KB SRAM (+ PSRAM opcional) | Aceleración por hardware para Machine Learning / TinyML, USB OTG, 45 GPIOs | Detección de arritmias con TinyML en el borde (*Edge AI*), cámaras médicas térmicas. |
| **ESP32-C3** | RISC-V Single-Core 32-bit | Hasta 160 MHz | Wi-Fi 4 + BLE 5.0 | 400 KB SRAM | Reemplazo pin a pin de ESP8266, bajo consumo, 22 GPIOs | Nodos sensores de temperatura axilar continua, pulsioxímetros vestibles ultracompactos. |
| **ESP32-C6** | RISC-V Single-Core 32-bit | Hasta 160 MHz | Wi-Fi 6 (802.11ax) + BLE 5.3 + Zigbee / Thread (802.15.4) | 512 KB SRAM | Redes malladas médicas de ultra bajo consumo (Matter / Thread) | Redes de telemetría hospitalaria distribuida en pabellones y residencias geriátricas. |

#### Consideraciones Críticas para Instrumentación Biomédica:
1. **Atenuación y Ruido del ADC SAR:** El conversor analógico-digital (ADC) interno del ESP32 presenta no linealidades en los extremos (0 V - 0.1 V y 3.1 V - 3.3 V). Para señales bioeléctricas de baja amplitud (ECG, EMG, EEG) se recomienda amplificación/acondicionamiento previo con front-ends biomédicos dedicados (como AD8232 o ADS1292R) o el uso de ADCs delta-sigma externos vía bus I2C/SPI (ADS1115).
2. **Conflicto Wi-Fi y ADC2:** El módulo ADC2 interno comparte circuitería con el controlador RF de Wi-Fi. Cuando el Wi-Fi está transmitiendo o conectado, la lectura analógica de los pines de ADC2 arroja errores o valores corruptos. **Regla de oro: En proyectos biomédicos conectados por Wi-Fi, utilizar exclusivamente los pines asociados al ADC1 (GPIOs 32 a 39).**
3. **Aislamiento Galvánico y Seguridad Eléctrica:** Todo equipo con electrodos en contacto con el paciente debe cumplir normativas internacionales de seguridad electromédica (como IEC 60601-1). Al conectar el ESP32 al PC por puerto USB, deben usarse aisladores galvánicos USB (tipo ADuM3160 / ADuM4160) o alimentar el microcontrolador con baterías aisladas.

---

### 2. Qué es Node-RED y su Rol en Telesalud

**Node-RED** es una plataforma de desarrollo basada en flujos (*Flow-Based Programming*) creada por IBM y gestionada bajo la OpenJS Foundation. Ejecutada sobre el entorno de ejecución **Node.js**, utiliza un modelo de concurrencia dirigido por eventos y no bloqueante (*Event-driven, Non-blocking I/O*).

```
                      +------------------------------------------+
                      |         Node-RED Runtime (Node.js)       |
                      +------------------------------------------+
                            |              |               |
              +-------------+              |               +-------------+
              |                            |                             |
      [ Input Nodes ]             [ Function Nodes ]            [ Output Nodes ]
      - MQTT In                   - Javascript Logic            - MQTT Out
      - HTTP In (REST Endpoints)  - Context Data (flow/global)  - HTTP Response
      - Serial In                 - Sequence (split/join/switch)- File / SQLite Storage
      - WebSockets In             - Parsers (JSON, CSV, XML)    - UI Dashboard Gauges/Charts
              \                            |                             /
               \                           |                            /
                +-----------------> [ msg Object ] <-------------------+
                                    - msg._msgid
                                    - msg.payload (datos de sensor)
                                    - msg.topic (origen/canal)
```

#### Ventajas en Sistemas de Salud:
- **Puente Multiprotocolo (IoT Gateway):** Convierte lecturas crudas del puerto Serie o paquetes MQTT en APIs REST seguras, sockets en tiempo real o registros compatibles con estándares de salud (HL7 / FHIR).
- **Procesamiento de Eventos Complejos (CEP):** Permite detectar umbrales críticos de signos vitales (ej. frecuencia cardíaca > 120 lpm acompañada de SpO2 < 90%) en milisegundos y disparar notificaciones inmediatas a personal clínico.
- **Abstracción Rápida:** Reduce drásticamente el tiempo de desarrollo de paneles de control clínicos mediante dashboards modulares sin necesidad de programar manualmente interfaces web complejas en React o Angular.

---

### 3. Protocolos de Comunicación: MQTT vs. REST

En proyectos de telemetría médica, la elección del protocolo de transporte impacta directamente en la autonomía energética de los nodos sensores, la latencia de entrega de alarmas críticas y la escalabilidad del sistema hospitalario.

#### A. Protocolo MQTT (Message Queuing Telemetry Transport)

MQTT es un protocolo de transporte de mensajería ligero, orientado al modelo **Publicador / Suscriptor (Pub/Sub)** sobre TCP/IP, optimizado para redes con ancho de banda reducido, alta latencia o conexiones inestables.

```
+----------------+                       +-------------------+                       +-----------------+
| Dispositivo    |                       |   BROKER MQTT     |                       |  Estación de    |
| Biomédico      | -- Publish(Vitals) -> | (Mosquitto/EMQX)  | -- Forward(Vitals) -> |  Monitoreo      |
| (ESP32)        |                       |                   |                       |  (Node-RED)     |
+----------------+                       +-------------------+                       +-----------------+
                                                   ^
                                                   | -- Subscribe(Vitals)
                                         +-------------------+
                                         | Panel Clínico / BD|
                                         +-------------------+
```

- **Broker:** Servidor central encargado de gestionar las conexiones de los clientes, recibir los mensajes y enrutar las publicaciones hacia aquellos clientes que se hayan suscrito a los tópicos correspondientes (ejemplos: Eclipse Mosquitto, EMQX, HiveMQ).
- **Estructura de Tópicos (Topic Taxonomy):** Nomenclatura jerárquica separada por barras diagonales. Ejemplo en telesalud:
  - `upch/pabellon_a/cama_03/signos_vitales/spo2`
  - `upch/pabellon_a/cama_03/comandos/calibracion`
  - Comodines: `+` (un solo nivel, ej. `upch/+/cama_03/signos_vitales/#`) y `#` (multinivel).
- **Niveles de Calidad de Servicio (QoS):**
  - **QoS 0 (At most once):** Entrega sin confirmación. Ideal para datos continuos de alta frecuencia donde la pérdida de una muestra puntual es irrelevante (ej. fotopletismografía cruda a 50 Hz).
  - **QoS 1 (At least once):** Garantiza que el mensaje llega al menos una vez (puede duplicarse). Ideal para mediciones periódicas de temperatura corporal o presión arterial.
  - **QoS 2 (Exactly once):** Garantiza una entrega única mediante un handshake de cuatro vías. Utilizado para registros de dosificación de fármacos o disparo de desfibrilador.
- **LWT (Last Will and Testament):** Mensaje preconfigurado que el broker publica automáticamente si el dispositivo de telemedicina pierde conexión abruptamente (corte de energía o falla de red), alertando de inmediato a la central médica sobre la desconexión del paciente.

#### B. Protocolo REST sobre HTTP/HTTPS

REST (*Representational State Transfer*) es un estilo de arquitectura cliente-servidor sin estado (*stateless*), que opera sobre el protocolo HTTP/HTTPS aprovechando los verbos estándar de la web:

- **`GET`:** Consulta o lectura de información (ej. obtener el historial de signos vitales de un paciente).
- **`POST`:** Creación de un nuevo recurso o envío de telemetría (ej. registrar un nuevo conjunto de mediciones `{"bpm": 74, "spo2": 98}`).
- **`PUT` / `PATCH`:** Actualización total o parcial de registros médicos.
- **`DELETE`:** Eliminación de recursos.

#### Tabla Comparativa: MQTT vs. REST en Telesalud

| Criterio | MQTT | HTTP / REST |
| :--- | :--- | :--- |
| **Modelo de Comunicación** | Publicador / Suscriptor (Asíncrono, bidireccional) | Cliente / Servidor (Petición - Respuesta, Síncrono) |
| **Sobrecarga de Encabezados** | Muy baja (2 bytes en encabezado fijo) | Alta (cientos de bytes por cabeceras HTTP) |
| **Consumo de Energía** | Mínimo; conexión TCP persistente con paquetes Keep-Alive livianos | Mayor; negociación de conexión en cada petición (handshake TCP/TLS) |
| **Consumo de Ancho de Banda** | Ideal para redes móviles 3G/4G/Satélite | Requiere mayor ancho de banda |
| **Detección de Caída de Red** | Automática e instantánea vía LWT (*Last Will*) | Solo detectable tras tiempo de espera (*timeout*) de la petición |
| **Facilidad de Integración Web** | Requiere WebSockets o pasarela MQTT en frontend | Nativo en cualquier navegador web, REST API y bases de datos |

---

## 🗂️ Estructura Completa del Repositorio

El repositorio está organizado en dos directorios principales estructurados de forma didáctica y progresiva:

```
HARDTES/
├── Readme.md                       # Documentación global del curso (este archivo)
│
├── 4_NODE-RED/                     # Módulo de aprendizaje integral de Node-RED
│   ├── 01-hello-world-debug/       # Inyección de datos, tipos de mensajes y panel Debug
│   ├── 02-function-and-context/    # Programación JS, scopes de memoria (context, flow, global)
│   ├── 03-parsers-json-csv-xml/    # Transformación de formatos heterogéneos de telemetría
│   ├── 04-sequences-and-flow-ctrl/ # Enrutamiento lógico (switch), alertas médicas y batch
│   ├── 05-mqtt-communication/     # Clientes MQTT Pub/Sub con brokers públicos y locales
│   ├── 06-http-rest-api/           # Servidor REST API propio y consumo de APIs de salud
│   ├── 07-storage-and-databases/   # Persistencia en archivos rotativos y base de datos
│   └── 08-dashboard-telehealth/    # Interfaz visual clínica (gráficos ECG, gauges, alertas)
│
└── 5_ESP32_NODE-RED/               # Módulo de integración física ESP32 y Node-RED
    ├── 01-esp32-blink-led-timer/   # Temporización no bloqueante con millis() para biosensores
    ├── 02-esp32-wifi-scan-rssi/    # Monitoreo de calidad de enlace inalámbrico (RSSI)
    ├── 03-esp32-wifi-access-point/ # Modo SoftAP para aprovisionamiento de equipos médicos
    ├── 04-esp32-ap-webserver/      # Servidor web HTTP embebido con interfaz de diagnóstico
    ├── 05-esp32-wifi-http-client/  # Modo Station y cliente REST enviando JSON
    ├── 06-esp32-nodered-serial/    # Telemetría bidireccional por puerto Serial USB
    ├── 07-esp32-nodered-http-rest/ # Publicación periódica hacia API REST en Node-RED
    └── 08-esp32-nodered-mqtt/      # Arquitectura IoMT definitiva con MQTT bidireccional
```

---

## 🚀 Requisitos e Instalación

### 1. Entorno Node-RED
Para ejecutar los flujos de Node-RED en tu computadora o pasarela (Raspberry Pi / servidor local):
```bash
# Instalar Node.js (versión LTS recomendada: 18 o 20)
# Verificar instalación:
node -v
npm -v

# Instalar Node-RED globalmente
npm install -g --unsafe-perm node-red

# Iniciar Node-RED
node-red
```
El entorno estará accesible en el navegador en `http://localhost:1880`.

Para los laboratorios que utilizan el panel de visualización, instala el módulo dashboard desde el menú *Manage Palette* o por terminal:
```bash
cd ~/.node-red
npm install node-red-dashboard
```

### 2. Entorno Arduino / PlatformIO para ESP32
1. Instalar el software **Arduino IDE** (v2.x) o la extensión **PlatformIO** en VS Code.
2. Añadir la URL de tarjetas ESP32 en *Preferencias > Gestor de URLs adicionales de tarjetas*:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. En el *Gestor de Tarjetas*, buscar e instalar el paquete **esp32** por *Espressif Systems*.
4. Instalar las siguientes librerías desde el Gestor de Librerías:
   - **ArduinoJson** (por Benoit Blanchon) — versión 6 o 7.
   - **PubSubClient** (por Nick O'Leary) — para conectividad MQTT.

---

## 👨‍🏫 Autoría y Derechos
- **Asignatura:** Hardware para Telesalud
- **Institución:** Universidad Peruana Cayetano Heredia (UPCH)
- **Año Académico:** 2026
- **Licencia:** Material académico de acceso abierto para fines educativos y de investigación.
