# Proyecto 03: ESP32 en Modo Punto de Acceso (SoftAP) Autónomo

## 1. Introducción y Relevancia en Telesalud
Cuando se despliega un equipo médico en el domicilio de un paciente o en una posta médica rural sin router Wi-Fi disponible, ¿cómo se conecta el personal de salud al equipo para configurarlo o descargar sus registros?

La solución es el modo **SoftAP** (*Software Access Point*). El ESP32 se comporta como un punto de acceso inalámbrico independiente:
- Genera su propio SSID y contraseña de red.
- Asigna direcciones IP mediante un servidor DHCP embebido en el rango `192.168.4.x`.
- Permite que médicos, enfermeros o técnicos se conecten directamente con su teléfono inteligente, tablet o laptop sin necesidad de Internet ni infraestructura externa.

---

## 2. Topología de Red Ad-Hoc

```
 [ Smartphone Médico ] \
                         +--- (Wi-Fi: ESP32-UPCH-TeleSalud) ---> [ ESP32 SoftAP ]
 [ Laptop Estación ] ---/                                        IP: 192.168.4.1
```

---

## 3. Parámetros de Red del Dispositivo
- **SSID:** `ESP32-UPCH-TeleSalud`
- **Contraseña:** `Biomedica2026` (WPA2-PSK, mínimo 8 caracteres)
- **Canal RF:** `1`
- **Dirección IP del Gateway ESP32:** `192.168.4.1`
- **Máximo de clientes simultáneos:** `4`

---

## 4. Código Fuente del Firmware (`firmware/wifi_softap.ino`)

El firmware inicializa el stack TCP/IP y la radio en modo `WIFI_AP`, configurando la subred y monitoreando en tiempo real la cantidad de clientes conectados mediante `WiFi.softAPgetStationNum()`.

---

## 5. Flujo Node-RED Acompañante (`nodered/flow.json`)

Simula un asistente de aprovisionamiento de red que envía credenciales de configuración hacia el microcontrolador cuando este se encuentra en modo AP.

---

## 6. Instrucciones de Carga y Prueba

1. Sube [`firmware/wifi_softap.ino`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/5_ESP32_NODE-RED/03-esp32-wifi-access-point/firmware/wifi_softap.ino) a la placa ESP32.
2. Abre el Monitor Serie a `115200 baudios`.
3. Con tu smartphone o laptop, busca redes Wi-Fi disponibles.
4. Conéctate a la red **`ESP32-UPCH-TeleSalud`** introduciendo la clave **`Biomedica2026`**.
5. Observa en el Monitor Serie cómo el ESP32 detecta de inmediato el nuevo cliente conectado e incrementa el contador de estaciones activas.
