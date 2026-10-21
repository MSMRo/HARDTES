# Laboratorio 03: Bloques Parser (JSON, CSV y XML) en Telesalud

## 1. Introducción y Objetivos
Los dispositivos biomédicos en campo transmiten información en formatos heterogéneos: tramas separadas por comas (CSV) desde puertos serie o tarjetas SD, estructuras jerárquicas XML en estándares hospitalarios (CDA/HL7) o cadenas de texto JSON sobre sockets TCP.

En este laboratorio aprenderás a:
- Utilizar el nodo **JSON** para serializar (objeto -> texto) y deserializar (texto -> objeto) automáticamente.
- Emplear el nodo **CSV** para convertir líneas de texto delimitadas por comas en objetos estructurados con nombres de columnas.
- Usar el nodo **XML** para transformar documentos biomédicos basados en etiquetas XML a objetos manipulables en JavaScript.
- Estandarizar tramas biomédicas crudas hacia modelos de datos clínicos uniformes.

---

## 2. Nodos Parser Fundamentales

| Nodo Parser | Entrada | Salida | Aplicación en Telesalud |
| :--- | :--- | :--- | :--- |
| **JSON** | Cadena de texto JSON (o viceversa) | Objeto nativo de JavaScript | Intercambio entre ESP32 (ArduinoJson) y servidores Node-RED. |
| **CSV** | Texto delimitado (`"72,98,36.5"`) | Objeto `{bpm: 72, spo2: 98, temp: 36.5}` | Parseo de logs generados en micro-SD o puertos Serial UART. |
| **XML** | Documento XML (`<paciente><id>1</id>...`) | Objeto anidado `{paciente: {id: 1...}}` | Integración con sistemas de información hospitalaria (HIS/RIS). |

---

## 3. Arquitectura del Flujo

```
 [ Inject: Trama CSV Cruda ] ---> [ Parser: CSV a Objeto ] ---> [ Debug: Objeto Parseado ]
                                                                       |
 [ Inject: String JSON ] --------> [ Parser: JSON ] -------------> [ Transformación ]
                                                                       |
 [ Inject: Bloque XML ] ---------> [ Parser: XML a JS ] -----------> [ Debug: JSON Unificado ]
```

### Ejemplos de Cargas Útiles:

#### Trama CSV:
```text
timestamp,paciente_id,bpm,spo2,temp_c
2026-10-20T12:00:00Z,PAC-089,78,97,36.8
```

#### Documento XML Biomédico:
```xml
<telemetria>
    <paciente id="PAC-089">
        <signos>
            <frecuencia_cardiaca unidad="lpm">78</frecuencia_cardiaca>
            <saturacion_oxigeno unidad="%">97</saturacion_oxigeno>
        </signos>
    </paciente>
</telemetria>
```

---

## 4. Configuración del Nodo CSV
Para convertir la trama CSV automáticamente a un objeto:
1. Haz doble clic en el nodo **CSV**.
2. En la propiedad **Columns**, introduce: `timestamp,paciente_id,bpm,spo2,temp_c`.
3. Marca la casilla **Parse numerical values** para que los campos numéricos se conviertan a tipos `number` de JavaScript en lugar de cadenas de texto.
4. Salida: Selecciona **a JavaScript object** para que cada fila genere un mensaje con el objeto listo.

---

## 5. Cómo Importar y Probar
1. Abre el archivo [`flow.json`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/4_NODE-RED/03-parsers-json-csv-xml/flow.json) y cópialo.
2. En Node-RED: `☰` > **Import** > Pegar > **Import** > **Deploy**.
3. Haz clic en el nodo **Inject Trama CSV Cruda**:
   - Observa en el panel Debug cómo la cadena de texto plana se transformó instantáneamente en un objeto tipado con propiedades numéricas accesibles (`msg.payload.bpm`, etc.).
4. Haz clic en el nodo **Inject XML Biomédico**:
   - Observa la conversión limpia de XML jerárquico a un objeto JSON listo para ser consumido por un dashboard o base de datos.
