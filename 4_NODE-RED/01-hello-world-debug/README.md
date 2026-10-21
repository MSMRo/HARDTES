# Laboratorio 01: Hello World, Inyección y Depuración en Node-RED

## 1. Introducción y Objetivos
En este primer laboratorio aprenderás las bases operativas de Node-RED:
- Comprender el ciclo de vida del objeto mensaje (`msg`).
- Manipular los atributos estándar `msg.payload`, `msg.topic` y `msg._msgid`.
- Utilizar el nodo **Inject** para disparar eventos manuales y temporizados con distintos tipos de datos (string, timestamp, boolean, JSON).
- Utilizar el nodo **Debug** para inspeccionar las propiedades del mensaje en el panel lateral.

En telesalud, este flujo simula la generación periódica de paquetes de estado de un monitor de signos vitales (heartbeat / presencia).

---

## 2. Nodos Utilizados

| Nodo | Icono / Tipo | Rol |
| :--- | :--- | :--- |
| **Inject** | Entrada | Genera mensajes de forma manual al hacer clic en el botón o automáticamente por intervalos (ej. cada 2 segundos). |
| **Debug** | Salida | Muestra el contenido del mensaje en la pestaña *Debug messages* de la barra lateral derecha. Permite inspeccionar solo `msg.payload` o el objeto completo `complete msg object`. |

---

## 3. Arquitectura del Flujo

```
 [ Inject: "Hello World" ] -----\
                                  +--> [ Debug: Inspeccionar msg.payload ]
 [ Inject: Timestamp actual ] ---+
                                  |
 [ Inject: JSON Paciente ] ------+---> [ Debug: Objeto msg completo ]
```

### Tipos de Datos Inyectados:
1. **String:** `"Monitor Telesalud UPCH activo"` con tópico `upch/status`.
2. **Timestamp:** Marca de tiempo numérica (epoch en milisegundos), vital para sincronización de series temporales médicas.
3. **JSON Object:** Estructura con datos de paciente:
   ```json
   {
     "patient_id": "PAC-1002",
     "device": "ESP32-Holter",
     "battery_pct": 89
   }
   ```

---

## 4. Guía Paso a Paso para Crear el Flujo Manualmente

1. **Abrir el editor:** Ve a `http://localhost:1880` en tu navegador.
2. **Arrastrar nodos:**
   - Desde la paleta izquierda, arrastra tres nodos **Inject** a la hoja de trabajo (*Flow 1*).
   - Arrastra dos nodos **Debug** a la derecha.
3. **Configurar el Nodo Inject 1 (Texto simple):**
   - Haz doble clic sobre el nodo.
   - En **msg.payload**, cambia el selector de tipo a `string` e introduce: `Hola Mundo desde Telesalud UPCH`.
   - En **msg.topic**, escribe `telesalud/saludo`.
   - Asigna el nombre: `Inyectar Saludo`.
   - Haz clic en **Done**.
4. **Configurar el Nodo Inject 2 (Timestamp periódico):**
   - En **msg.payload**, deja seleccionado `timestamp`.
   - En el apartado **Repeat**, selecciona `interval` y pon `every 5 seconds`.
   - Asigna el nombre: `Heartbeat cada 5s`.
   - Haz clic en **Done**.
5. **Configurar el Nodo Inject 3 (Objeto JSON de telemetría):**
   - En **msg.payload**, cambia el tipo a `{}` (JSON) e introduce los campos del paciente.
   - Asigna el nombre: `Inyectar Datos Paciente`.
6. **Configurar los Nodos Debug:**
   - Nodo 1: Deja la salida como `msg.payload`. Nombre: `Ver Payload`.
   - Nodo 2: Cambia la salida a `complete msg object`. Nombre: `Ver Objeto Completo`.
7. **Conectar y Desplegar:**
   - Une las salidas de los nodos Inject a las entradas de los nodos Debug con cables virtuales.
   - Presiona el botón rojo **Deploy** en la esquina superior derecha.
   - Abre la pestaña de la cucaracha (Debug) en el panel derecho y presiona los botones de inyección para observar los mensajes recibidos.

---

## 5. Importar el Flujo Directamente
Si prefieres cargar el flujo preconfigurado:
1. Copia el contenido del archivo [`flow.json`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/01-hello-world-debug/flow.json).
2. En Node-RED, abre el menú de hamburguesa superior derecho `☰` > **Import**.
3. Pega el JSON en la ventana emergente y presiona **Import**.
4. Haz clic en **Deploy**.
