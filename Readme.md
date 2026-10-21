# Hardware para Telesalud
## Universidad Peruana Cayetano Heredia (UPCH)
### Facultad de Ciencias e Ingeniería — Ingeniería Biomédica

---

Bienvenido al repositorio oficial del curso **Hardware para Telesalud**. Este repositorio contiene una guía exhaustiva, modular y progresiva para el diseño, prototipado, simulación e integración de sistemas embebidos en el ámbito del **Internet of Medical Things (IoMT)** y la telemedicina.

El currículo práctico está organizado en 5 módulos formativos integrados:

```
HARDTES/
├── 1_ELECTRONICA_BASICA/     # Electrónica analógica y digital para instrumentación médica en SimulIDE
├── 2_ARDUINO_UNO_C++/        # C y C++ embebido, registros AVR, ADC, PWM, Timers e interrupciones en SimulIDE
├── 3_ARDUINO_UNO_SENSORES/   # Sensores biomédicos y protocolos de hardware (UART, I2C, SPI) en SimulIDE
├── 4_NODE-RED/               # Programación por flujos, parsers, persistencia, APIs REST y Dashboards
└── 5_ESP32_NODE-RED/         # Microcontrolador ESP32 (Wi-Fi, AP, Web Server) e integración física con Node-RED
```

---

## 📚 Mapa Curricular y Contenido de los Módulos

### ⚡ Módulo 1: Electrónica Básica y Digital (`1_ELECTRONICA_BASICA/`)
Simulación en **SimulIDE** de circuitos fundamentales para el acondicionamiento de señales y seguridad médica:
- [`01-ley-ohm-divisor-voltaje`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/01-ley-ohm-divisor-voltaje/README.md): Ley de Ohm, divisor de tensión resistivo, efecto de carga e instrumentación de sensores resistivos.
- [`02-filtro-rc-pasabajas-pasoaltos`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/02-filtro-rc-pasabajas-pasoaltos/README.md): Filtros pasivos RC, frecuencia de corte $f_c = \frac{1}{2\pi RC}$, atenuación de ruido de 60 Hz y deriva de línea base en ECG/EMG.
- [`03-diodos-rectificacion-fuente`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/03-diodos-rectificacion-fuente/README.md): Rectificación con filtro capacitivo para fuentes DC médicas y protección de entradas de microcontroladores con diodo Zener.
- [`04-transistor-bjt-switch-rele`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/04-transistor-bjt-switch-rele/README.md): Transistor BJT NPN (2N2222) en corte y saturación, driver de actuadores de potencia (alarmas UCI, solenoides de infusión) y diodo flyback.
- [`05-amplificador-operacional-bio`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/05-amplificador-operacional-bio/README.md): OpAmp LM358 en configuraciones seguidor de voltaje (buffer de altísima impedancia de entrada) y amplificador no inversor para biopotenciales.
- [`06-temporizador-555-astable`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/06-temporizador-555-astable/README.md): Oscilador astable NE555, cálculo de frecuencia y Duty Cycle, simulador de marcapasos y reloj biológico.
- [`07-compuertas-logicas-digitales`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/1_ELECTRONICA_BASICA/07-compuertas-logicas-digitales/README.md): Lógica combinacional (AND, OR, NOT) aplicada a un sistema de interbloqueo y seguridad (*Hardware Interlock*) para desfibriladores médicos.

---

### 💻 Módulo 2: C y C++ Embebido para Arduino Uno (`2_ARDUINO_UNO_C++/`)
Dominio de la arquitectura del ATmega328P y programación de bajo nivel simulada en **SimulIDE**:
- [`01-pinout-gpio-registros-avr`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/01-pinout-gpio-registros-avr/README.md): Pinout del ATmega328P, registros directos `DDRB`, `PORTB`, `DDRD`, `PIND` en 1 ciclo de reloj ($62.5\text{ ns}$) y operadores de bits (`&`, `|`, `^`, `~`, `<<`).
- [`02-c-cpp-tipos-estructuras`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/02-c-cpp-tipos-estructuras/README.md): Tipos de datos de ancho fijo (`<stdint.h>`), empaquetamiento de signos vitales en `struct`, punteros y paso por referencia para optimizar los 2 KB de SRAM.
- [`03-comunicacion-serial-uart`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/03-comunicacion-serial-uart/README.md): UART hardware (pines D0/D1 a 115200 bps), buffer circular de recepción e intérprete interactivo de comandos clínicos en C++.
- [`04-adc-muestreo-analogico`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/04-adc-muestreo-analogico/README.md): ADC de 10 bits ($0-1023$), voltajes de referencia (`DEFAULT` 5V vs `INTERNAL` 1.1V), resolución LSB, cálculo de milivoltios y muestreo de temperatura.
- [`05-pwm-control-actuadores`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/05-pwm-control-actuadores/README.md): Modulación por ancho de pulsos en Timer1 (Pin D9), ciclo de trabajo ($0-100\%$) y dosificación de actuador para bomba de infusión.
- [`06-timers-e-interrupciones`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/06-timers-e-interrupciones/README.md): Adquisición isócrona a $100\text{ Hz}$ exactos con Timer1 en modo CTC (`TIMER1_COMPA_vect`) e interrupción externa inmediata (`INT0` / Pin D2) para botón de pánico del paciente.
- [`07-poo-clases-cpp-sensores`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/2_ARDUINO_UNO_C%2B%2B/07-poo-clases-cpp-sensores/README.md): Programación Orientada a Objetos en C++ embebido (`class MonitorCardiaco`), constructores, métodos de filtrado y detección de taquicardia.

---

### 🩺 Módulo 3: Sensores y Protocolos Hardware (`3_ARDUINO_UNO_SENSORES/`)
Integración de transductores biomédicos y buses de comunicación industriales en **SimulIDE**:
- [`01-sensor-ntc-steinhart-hart`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/01-sensor-ntc-steinhart-hart/README.md): Termistor NTC de $10\text{ k}\Omega$ en divisor de tensión, linealización matemática con la Ecuación de Steinhart-Hart / Coeficiente $\beta$ y diagnóstico térmico.
- [`02-sensor-pulso-ppg-analogico`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/02-sensor-pulso-ppg-analogico/README.md): Fotopletismografía óptica (PPG), filtro digital IIR paso-altas para remoción de componente DC, cálculo de intervalo inter-latido (IBI) y BPM.
- [`03-sensor-ecg-ad8232-biomedico`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/03-sensor-ecg-ad8232-biomedico/README.md): Front-end de biopotenciales AD8232 para electrocardiografía y supervisión de pines de seguridad de electrodos desconectados (`LO+` y `LO-`).
- [`04-protocolo-uart-gps-telesalud`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/04-protocolo-uart-gps-telesalud/README.md): Protocolo UART serie a fondo, `SoftwareSerial` en pines D4/D5 y decodificación de sentencias NMEA 0183 (`$GPRMC`) para geolocalización de ambulancias.
- [`05-protocolo-i2c-sensores-oled`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/05-protocolo-i2c-sensores-oled/README.md): Bus I2C / TWI a fondo (líneas SDA/SCL, resistencias pull-up de $4.7\text{ k}\Omega$, direccionamiento de 7 bits, condiciones Start/Stop/ACK) con escáner I2C y display de cabecera.
- [`06-protocolo-spi-datalogger-sd`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/06-protocolo-spi-datalogger-sd/README.md): Bus SPI a fondo (MOSI, MISO, SCK, CS hasta $8\text{ MHz}$) y registro continuo en memoria MicroSD (`HOLTER.CSV`) en formato FAT16/FAT32 para monitor Holter ambulatorio.

---

### 🌐 Módulo 4: Node-RED para Telesalud (`4_NODE-RED/`)
Desarrollo de flujos de eventos para pasarelas médicas (*Edge Gateways*):
- [`01-hello-world-debug`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/01-hello-world-debug/README.md): Inyección periódica de timestamps, cadenas y JSON de constantes vitales con panel Debug.
- [`02-function-and-context`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/02-function-and-context/README.md): JavaScript en nodo Function, cálculo de media móvil y variables de contexto (`flow.set` / `flow.get`).
- [`03-parsers-json-csv-xml`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/03-parsers-json-csv-xml/README.md): Parsers bidireccionales JSON, CSV de biosensores y XML hospitalario.
- [`04-sequences-and-flow-ctrl`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/04-sequences-and-flow-ctrl/README.md): Triage clínico con `switch`, categorización de prioridad con `change`, y procesamiento en ráfagas de ondas ECG con `split` y `join`.
- [`05-mqtt-communication`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/05-mqtt-communication/README.md): Conexión con broker público (`broker.emqx.io`), tópicos jerárquicos y QoS en telemetría médica.
- [`06-http-rest-api`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/06-http-rest-api/README.md): Servidor REST API propio en Node-RED con endpoints `POST` y `GET /api/paciente/vitales`.
- [`07-storage-and-databases`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/07-storage-and-databases/README.md): Persistencia en archivos locales de auditoría (`pacientes_vitales.csv`) en modo append y lectura.
- [`08-dashboard-telehealth`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/08-dashboard-telehealth/README.md): Panel web interactivo (`http://localhost:1880/ui`) con tacómetros gauges de SpO2/temperatura y gráfica temporal de ritmo cardíaco.

---

### 🚀 Módulo 5: Integración ESP32 con Node-RED (`5_ESP32_NODE-RED/`)
Desde el firmware del microcontrolador hasta su enlace físico y telemático con Node-RED:
- [`01-esp32-blink-led-timer`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/01-esp32-blink-led-timer/README.md): Temporización no bloqueante con `millis()` y watchdog de latidos en Node-RED.
- [`02-esp32-wifi-scan-rssi`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/02-esp32-wifi-scan-rssi/README.md): Escaneo del espectro RF y clasificación de cobertura clínica (RSSI en dBm).
- [`03-esp32-wifi-access-point`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/03-esp32-wifi-access-point/README.md): Modo SoftAP autónomo (`192.168.4.1`) para aprovisionamiento inicial de equipos médicos.
- [`04-esp32-ap-webserver`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/04-esp32-ap-webserver/README.md): Servidor web HTTP embebido con consola HTML5 para monitoreo directo en smartphones de campo.
- [`05-esp32-wifi-http-client`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/05-esp32-wifi-http-client/README.md): Conexión en modo Station (STA) y cliente REST HTTP enviando JSON con `ArduinoJson`.
- [`06-esp32-nodered-serial`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/06-esp32-nodered-serial/README.md): Telemetría bidireccional por puerto Serie USB (UART a 115200 baudios) entre el ESP32 y Node-RED.
- [`07-esp32-nodered-http-rest`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/07-esp32-nodered-http-rest/README.md): Integración REST punto a punto (ESP32 envía `POST` y deserializa la respuesta de confirmación 201).
- [`08-esp32-nodered-mqtt`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/08-esp32-nodered-mqtt/README.md): Arquitectura IoMT definitiva con `PubSubClient`, tópicos de vitales, telecontrol remoto y presencia LWT (*Last Will*).

---

## 🛠️ Herramientas de Software Requeridas

1. **Simulador de Circuitos Electrónicos y Embebidos:**
   - **SimulIDE** (versión 1.0.0 o superior): [simulide.com](https://www.simulide.com/). Permite abrir y simular directamente los archivos `.sim1` presentes en los módulos 1, 2 y 3.
2. **Entorno de Programación Embebida:**
   - **Arduino IDE** (v2.x) o **PlatformIO** en VS Code.
   - Paquete de tarjetas **Arduino AVR Boards** y **ESP32 by Espressif**.
   - Librerías: `ArduinoJson`, `PubSubClient`, `SoftwareSerial`, `Wire`, `SPI`, `SD`.
3. **Plataforma de Flujos IoT:**
   - **Node.js** (versión 18 o 20 LTS).
   - **Node-RED** (`npm install -g --unsafe-perm node-red`).
   - Paletas recomendadas: `node-red-dashboard`, `node-red-node-serialport`.

---

## 👨‍🏫 Información Académica
- **Curso:** Hardware para Telesalud
- **Institución:** Universidad Peruana Cayetano Heredia (UPCH)
- **Facultad:** Ciencias e Ingeniería
- **Carrera:** Ingeniería Biomédica
- **Año Académico:** 2026
