# Proyecto 02: Escaneo de Redes Wi-Fi y Análisis de Señal (RSSI)

## 1. Introducción y Relevancia en Telesalud
En un entorno hospitalario o en visitas domiciliarias de telemedicina rural, la fiabilidad del enlace inalámbrico es crítica. Una señal Wi-Fi débil provoca desconexiones intempestivas, pérdida de paquetes de electrocardiografía o retrasos en alarmas de emergencia.

El indicador **RSSI** (*Received Signal Strength Indicator*) se mide en decibelios-milivatio (**dBm**), siendo valores negativos donde un número más cercano a cero indica una señal más potente:
- **-30 a -60 dBm:** Excelente. Enlace ideal para transmisión continua de señales de alta tasa (ECG a 250 Hz).
- **-60 a -75 dBm:** Buena a regular. Adecuada para telemetría periódica (temperatura, pulsioximetría cada 5 segundos).
- **-75 a -85 dBm:** Débil. Riesgo elevado de pérdida de paquetes y retardos.
- **< -85 dBm:** Crítica / Inutilizable para equipos de soporte de vida.

---

## 2. Diagrama de Funcionamiento

```
                 +-------------------+
                 | ESP32 RF Receiver |
                 +-------------------+
                           |
                  (Escaneo 2.4 GHz)
                           |
             +-------------+-------------+
             |                           |
     [ Red Hospitalaria ]        [ Red Visitante ]
     SSID: UPCH-Salud-5G         SSID: Invitados
     RSSI: -52 dBm (Óptimo)      RSSI: -88 dBm (Crítico)
             |
             v
   [ Salida Serial JSON ] ----> [ Gateway Node-RED ]
```

---

## 3. Código Fuente del Firmware (`firmware/wifi_scan_rssi.ino`)

El firmware utiliza la librería nativa `WiFi.h`:
1. Inicializa el módulo Wi-Fi en modo Estación (`WIFI_STA`) y desconecta cualquier enlace previo.
2. Lanza un escaneo síncrono `WiFi.scanNetworks()`.
3. Ordena e imprime los resultados en formato JSON para que puedan ser interpretados directamente por pasarelas IoT.

---

## 4. Flujo Node-RED Acompañante (`nodered/flow.json`)

El flujo complementario procesa el informe de auditoría de red generado por el ESP32, clasifica la idoneidad clínica de cada red detectada y emite una recomendación para el despliegue del dispositivo médico.

---

## 5. Instrucciones de Compilación y Prueba

1. Abre [`firmware/wifi_scan_rssi.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/02-esp32-wifi-scan-rssi/firmware/wifi_scan_rssi.ino) en Arduino IDE.
2. Selecciona la placa `ESP32 Dev Module` y tu puerto COM.
3. Sube el código y abre el Monitor Serie a `115200 baudios`.
4. El ESP32 escaneará el espectro de 2.4 GHz cada 10 segundos mostrando el listado de redes, canales, tipo de cifrado y dBm.
