# Laboratorio 04: Protocolo Hardware UART y Módulo GPS para Teleasistencia Médica

## 1. Introducción y Relevancia en Telesalud
En los servicios de atención prehospitalaria (como el SAMU en Perú) o en el monitoreo de pacientes geriátricos con deterioro cognitivo/Alzheimer bajo teleasistencia domiciliaria, las constantes vitales deben ir acompañadas obligatoriamente de la **posición geoespacial exacta en tiempo real**.

La mayoría de los receptores satelitales (como el u-blox NEO-6M) se comunican con el microcontrolador mediante el **Protocolo Hardware UART** emitiendo sentencias estándar **NMEA 0183**.

En este laboratorio aprenderás a:
- Implementar comunicación UART por software con la librería `SoftwareSerial` en pines dedicados (`D4` RX, `D5` TX), reservando la UART física (`D0`/`D1`) para comunicación con Node-RED o PC.
- Decodificar y parsear la sentencia NMEA fundamental **`$GPRMC`**.
- Extraer Latitud, Longitud, Hora UTC y validez de la señal satelital (*Fix Status*).
- Generar paquetes de geolocalización en formato JSON listos para plataformas de mapas en telesalud.

---

## 2. Estructura de la Trama NMEA 0183 (`$GPRMC`)

```
 $GPRMC,123519.00,A,1205.3820,S,07701.8420,W,0.022,0.0,211026,,,A*7A
   |        |     |     |     |     |      |
   1        2     3     4     5     6      7
```
1. **Identificador:** `$GPRMC` (Recommended Minimum Specific GPS Data).
2. **Hora UTC:** `12:35:19.00`.
3. **Estado de Fijación (Fix):** `A` = Activo/Válido; `V` = Vacío/Inválido (sin satélites).
4. **Latitud:** `1205.3820` ($12^\circ\ 05.3820'$).
5. **Hemisferio Latitud:** `S` (Sur).
6. **Longitud:** `07701.8420` ($77^\circ\ 01.8420'$).
7. **Hemisferio Longitud:** `W` (Oeste - Coordenadas de Lima, Perú).

---

## 3. Conversión de Coordenadas NMEA a Grados Decimales
El formato NMEA viene en Grados y Minutos decimales (`DDMM.MMMM`):
$$\text{Grados Decimales} = \text{DD} + \frac{\text{MM.MMMM}}{60}$$
Si el hemisferio es Sur (`S`) u Oeste (`W`), el valor se multiplica por $-1$.

---

## 4. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:**
   - **Pin D4 (RX Software):** Recibe datos del transmisor GPS.
   - **Pines D0/D1:** Conectados a la consola para reporte de coordenadas.
2. **Terminal Serial Emisor:** Simula el módulo GPS inyectando sentencias NMEA periódicas.

---

## 5. Código Fuente del Firmware (`firmware/uart_gps_tracker.ino`)
Lee la trama del puerto serie por software, valida el prefijo `$GPRMC`, verifica si el fix es válido (`A`) y emite el JSON de telemetría.

---

## 6. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/04-protocolo-uart-gps-telesalud/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Envía una trama NMEA por el terminal serial conectado a D4:
   ```text
   $GPRMC,173000.00,A,1204.5500,S,07702.1200,W,15.2,180.0,211026,,,A*77
   ```
4. Observa cómo el Arduino procesa la información y reporta en consola la ubicación geográfica de la ambulancia en grados decimales (`Lat: -12.0758, Lon: -77.0353`).
