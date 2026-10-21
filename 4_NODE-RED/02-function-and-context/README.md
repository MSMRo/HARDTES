# Laboratorio 02: Funciones JavaScript y Gestión de Contexto

## 1. Introducción y Objetivos
En este laboratorio aprenderás a programar lógica personalizada en Node-RED mediante el nodo **Function** y a gestionar el estado en memoria usando las variables de **Contexto**:
- Escribir código JavaScript ES6 dentro de un nodo Function.
- Estructurar y mutar las propiedades del objeto `msg`.
- Comprender la diferencia entre los tres niveles de contexto:
  1. **Node Context (`context`):** Solo visible por el nodo actual.
  2. **Flow Context (`flow`):** Compartido entre todos los nodos dentro de la misma pestaña/hoja.
  3. **Global Context (`global`):** Accesible desde cualquier pestaña o flujo de todo Node-RED.
- Implementar un algoritmo biomédico de media móvil de frecuencia cardíaca (BPM) y clasificación de taquicardia/bradicardia.

---

## 2. Conceptos Clave

### El Objeto Mensaje en una Función
Toda función en Node-RED debe recibir `msg` y terminar retornando `msg`, un array de mensajes, o `null` si se desea abortar el flujo:
```javascript
// Acceso y cálculo
let bpm = msg.payload.bpm;
let temp = msg.payload.temp;

// Modificar o agregar propiedades
msg.payload.is_fever = (temp >= 38.0);

// Retornar mensaje para los nodos siguientes
return msg;
```

### Gestión de Contexto en Node-RED
Normalmente los nodos son *stateless* (sin estado). Si un sensor envía una muestra de BPM cada segundo, para calcular el promedio de las últimas 5 muestras necesitamos memoria temporal:
```javascript
// Obtener historial previo (o iniciar array vacío si no existe)
let historial = flow.get("bpm_history") || [];

// Agregar nueva muestra
historial.push(msg.payload.bpm);
if (historial.length > 5) {
    historial.shift(); // Mantener solo las últimas 5 muestras
}

// Guardar de nuevo en el contexto del flujo
flow.set("bpm_history", historial);

// Calcular media móvil
let suma = historial.reduce((a, b) => a + b, 0);
let promedio = (suma / historial.length).toFixed(1);
```

---

## 3. Arquitectura del Flujo

![](./imgs/Captura%20de%20pantalla%202026-10-21%20111643.png)

---

## 4. Código del Nodo Function (Biomédico)
```javascript
// 1. Obtener la muestra recibida
let inputBpm = Number(msg.payload.bpm);

// 2. Gestionar historial en contexto de flujo
let history = flow.get("bpm_history") || [];
history.push(inputBpm);

if (history.length > 5) {
    history.shift(); // Ventana deslizante de 5 muestras
}
flow.set("bpm_history", history);

// 3. Calcular promedio
let sum = history.reduce((acc, val) => acc + val, 0);
let avgBpm = Math.round(sum / history.length);

// 4. Clasificación clínica
let estado = "NORMAL";
if (avgBpm > 100) {
    estado = "TAQUICARDIA";
} else if (avgBpm < 60) {
    estado = "BRADICARDIA";
}

// 5. Construir objeto de salida enriquecido
msg.payload = {
    patient_id: msg.payload.patient_id || "PACIENTE-01",
    bpm_instantaneo: inputBpm,
    bpm_promedio_5_muestras: avgBpm,
    muestras_en_buffer: history.length,
    diagnostico_preliminar: estado,
    timestamp: new Date().toISOString()
};

node.status({
    fill: (estado === "NORMAL" ? "green" : "red"),
    shape: "dot",
    text: `BPM Prom: ${avgBpm} (${estado})`
});

return msg;
```

---

## 5. Cómo Importar y Ejecutar
1. Copia el archivo [`flow.json`](./HARDTES/4_NODE-RED/02-function-and-context/flow.json).
2. En Node-RED, dirígete a `☰` > **Import** y pega el contenido.
3. Haz clic en **Deploy**.
4. Dispara los diferentes nodos de inyección (BPM Normal = 72, Taquicardia = 115, Bradicardia = 52).
5. Observa cómo el nodo `Function` muestra su estado debajo con un punto de color y el panel Debug detalla el promedio calculado a través del tiempo.
