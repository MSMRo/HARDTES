# Laboratorio 04: Control de Flujo, Secuencias y Alertas Médicas

## 1. Introducción y Objetivos
En la telemonitorización clínica, las decisiones rápidas salvan vidas. No todos los mensajes deben seguir la misma ruta: una desaturación crítica de oxígeno requiere notificación inmediata por canal prioritario, mientras que una lectura normal puede enviarse a un log de baja prioridad.

En este laboratorio aprenderás a:
- Utilizar el nodo **Switch** para implementar un sistema de **Triage Clínico Automatizado** con múltiples salidas condicionales.
- Utilizar el nodo **Change** para mutar propiedades y añadir niveles de prioridad sin necesidad de escribir código JavaScript.
- Utilizar el nodo **Delay** para evitar la saturación de APIs o brokers (*Rate Limiting*).
- Utilizar los nodos de secuencia **Split** y **Join** para segmentar y procesar arrays de muestras de señales biomédicas (como un paquete de 10 muestras ECG).

---

## 2. Nodos de Control y Secuencia

| Nodo | Función Principal | Caso de Uso en Telesalud |
| :--- | :--- | :--- |
| **Switch** | Evalúa una o más reglas y enruta el mensaje a diferentes salidas. | Clasificación de riesgo clínico según SpO2 y frecuencia cardíaca. |
| **Change** | Establece, cambia o elimina propiedades de `msg`, `flow` o `global`. | Asignar banderas como `msg.alerta = "CODIGO_ROJO"`. |
| **Delay** | Pausa o limita la tasa de mensajes por segundo. | Evitar sobrecarga del servidor hospitalario enviando máx. 1 mensaje/seg. |
| **Split** | Descompone un array en múltiples mensajes individuales. | Procesar ráfagas de muestras ECG enviadas en bloque por el ESP32. |
| **Join** | Agrupa múltiples mensajes en un solo array o estructura. | Consolidar datos de múltiples biosensores en un único reporte. |

---

## 3. Arquitectura del Flujo de Triage Clínico

```
                             +--> [ Regla 1: SpO2 < 90 ] ----> [ Change: CRÍTICO ] ---> [ Alarma Roja ]
                             |
 [ Sensor SpO2 (Simulado) ] -+--> [ Regla 2: 90 <= SpO2 <= 94 ] -> [ Change: ALERTA ] -> [ Alerta Amarilla ]
                             |
                             +--> [ Regla 3: SpO2 > 94 ] ----> [ Change: ESTABLE ] ---> [ Log Normal ]
```

### Reglas Clínicas Configuradas en el Nodo Switch:
1. `msg.payload.spo2 < 90` -> Salida 1 (Urgencia Máxima / Hipoxemia Severa).
2. `msg.payload.spo2` está entre `90` y `94` -> Salida 2 (Observación).
3. `msg.payload.spo2 > 94` -> Salida 3 (Parámetros Normales).

---

## 4. Procesamiento de Ráfagas ECG con Split y Join

Los microcontroladores como el ESP32 suelen tomar muestras a 100 Hz o 250 Hz. Transmitir muestra por muestra vía Wi-Fi genera una sobrecarga excesiva de encabezados TCP/IP. La mejor práctica consiste en enviar **paquetes por lotes (arrays)**:
```json
{
  "canal": "ECG_Derivacion_II",
  "frecuencia_hz": 100,
  "muestras": [512, 520, 535, 780, 1023, 640, 510, 505, 512, 515]
}
```
1. El nodo **Split** recibe el mensaje y divide el array `msg.payload.muestras` en 10 mensajes secuenciales con metadatos `msg.parts` (índice, total).
2. Cada muestra puede ser filtrada individualmente (ej. detector de pico R).
3. El nodo **Join** vuelve a reunir los valores procesados en un único arreglo para visualización o archivo.

---

## 5. Cómo Importar y Probar
1. Copia el archivo [`flow.json`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/04-sequences-and-flow-ctrl/flow.json).
2. En Node-RED: `☰` > **Import** > Pegar > **Import** > **Deploy**.
3. Prueba los botones de inyección:
   - **SpO2 Crítico (86%):** Verás cómo el flujo enruta el mensaje únicamente a la alarma roja.
   - **SpO2 Límite (92%):** Se activa la alerta intermedia.
   - **SpO2 Normal (98%):** Se registra en el log normal verde.
   - **Ráfaga ECG (Array de 10 muestras):** Se activa la cadena Split -> Join con reconstrucción del vector.
