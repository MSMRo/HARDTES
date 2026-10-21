# Laboratorio 08: Dashboard de Monitoreo Clínico en Tiempo Real

## 1. Introducción y Objetivos
Un sistema de telesalud solo es efectivo si el personal médico o el paciente pueden interpretar rápidamente los datos sin requerir conocimientos de programación. Node-RED incluye la suite de componentes **Dashboard** (`node-red-dashboard`), permitiendo construir estaciones de enfermería y paneles clínicos interactivos.

En este laboratorio aprenderás a:
- Diseñar una interfaz gráfica web responsiva servida en `http://localhost:1880/ui`.
- Configurar grupos, pestañas y layouts visuales.
- Utilizar el nodo **UI Gauge** con sectores de color clínico (verde para SpO2 normal, amarillo para alerta, rojo para hipoxemia).
- Utilizar el nodo **UI Chart** para graficar la evolución temporal del pulso cardíaco (lpm).
- Utilizar **UI Text** y **UI Button** para visualización de estado de paciente y confirmación de alertas.

---

## 2. Requisito Previo: Instalación de la Paleta Dashboard
Si tu instalación de Node-RED aún no tiene instalados los nodos de dashboard:
1. En Node-RED, ve al menú superior derecho `☰` > **Manage palette**.
2. Dirígete a la pestaña **Install**.
3. Escribe en la búsqueda: `node-red-dashboard`.
4. Haz clic en **Install**.
*(Alternativamente por terminal: `cd ~/.node-red && npm install node-red-dashboard`)*.

---

## 3. Elementos de la Interfaz Clínica

```
+-----------------------------------------------------------------------+
|  🏥 UPCH TELESALUD — ESTACIÓN DE ENFERMERÍA (UCI PEDIÁTRICA)          |
+-----------------------------------------------------------------------+
|  [ PACIENTE: Juan Pérez - Cama 04 ]            [ ESTADO: ESTABLE 🟢 ] |
+-----------------------------------+-----------------------------------+
|  SATURACIÓN DE OXÍGENO (SpO2)     |  TEMPERATURA CORPORAL (°C)        |
|               98 %                |               36.7 °C             |
|       (Gauge Semicircular)        |        (Gauge Tipo Termómetro)    |
+-----------------------------------+-----------------------------------+
|  HISTORIAL DE FRECUENCIA CARDÍACA (lpm)                               |
|   120 |                                                               |
|    80 |  /\    /\    /\    /\    /\                                   |
|    40 |    \  /  \  /  \  /  \  /  \                                  |
|     0 +-----------------------------------------------------------    |
|       17:35        17:36        17:37        17:38      (Tiempo Real) |
+-----------------------------------------------------------------------+
```

---

## 4. Estructura de los Nodos del Flujo

1. **Generador Periódico (Inject):** Dispara una lectura cada 2 segundos simulando la transmisión de un monitor biomédico.
2. **Distribuidor de Señales (Function):** Extrae y formatea cada parámetro vital:
   - `msg.payload.spo2` -> Hacia el Gauge de SpO2.
   - `msg.payload.bpm` -> Hacia el Chart de Frecuencia Cardíaca.
   - `msg.payload.temp` -> Hacia el Gauge de Temperatura.
   - `msg.payload.estado` -> Hacia la etiqueta de estado clínico.
3. **Botón de Simulación de Emergencia (UI Button):** Inyecta instantáneamente un episodio de bradicardia severa con desaturación para observar la respuesta visual de las alarmas en el Dashboard.

---

## 5. Cómo Importar y Visualizar
1. Copia el contenido de [`flow.json`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/08-dashboard-telehealth/flow.json).
2. En Node-RED: `☰` > **Import** > Pegar > **Import** > **Deploy**.
3. Abre una nueva pestaña en tu navegador web y navega a:
   ```
   http://localhost:1880/ui
   ```
4. Observa el panel de monitoreo dinámico actualizándose en tiempo real. Presiona el botón **Simular Emergencia** en la interfaz para probar la reacción visual del sistema.
