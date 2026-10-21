# Laboratorio 05: Protocolo Hardware I2C (TWI), Sensores Digitales y Pantallas Clínicas

## 1. Introducción y Relevancia en Telesalud
El bus **I2C (Inter-Integrated Circuit)**, también denominado **TWI (Two-Wire Interface)** por Atmel, es el estándar más extendido en la instrumentación biomédica moderna. Prácticamente todos los chips médicos avanzados —como el sensor de pulso y oxígeno en sangre **MAX30102**, el acelerómetro de detección de caídas de pacientes **MPU6050** o los sensores de temperatura infrarroja clínica **MLX90614**— se comunican exclusivamente por I2C.

Su gran ventaja es que permite conectar **hasta 127 dispositivos distintos utilizando únicamente 2 cables** compartidos en paralelo.

En este laboratorio aprenderás a:
- Conocer la arquitectura del bus I2C a nivel de hardware (líneas de colector abierto y resistencias **Pull-Up de $4.7\text{ k}\Omega$**).
- Entender el diagrama de tiempos: condiciones de **START, STOP, ACK y NACK**.
- Implementar un **I2C Scanner** para descubrir direcciones de 7 bits en el bus.
- Controlar una pantalla gráfica o display I2C (dirección estándar `0x3C` o `0x27`) para visualizar constantes vitales a pie de cama.

---

## 2. Fundamento Físico y Protocolo del Bus I2C

```
     +5V
      |        |
    [4.7k]   [4.7k] (Resistencias Pull-Up obligatorias)
      |        |
 SDA -+--------+-------+--------------------+ (A4 en Arduino Uno)
                       |                    |
 SCL ------------------+--------------------+ (A5 en Arduino Uno)
                       |                    |
                 +-----------+        +-----------+
                 | Sensor    |        | Display   |
                 | MAX30102  |        | OLED/LCD  |
                 | (Dir 0x57)|        | (Dir 0x3C)|
                 +-----------+        +-----------+
```

### Anatomía de una Transacción I2C:
1. **Condición START:** La línea `SDA` pasa de nivel alto a bajo mientras `SCL` permanece en nivel alto.
2. **Dirección de 7 bits + Bit R/W:** El maestro transmite los 7 bits de dirección del esclavo y un 8vo bit: `0` para Escritura (*Write*) o `1` para Lectura (*Read*).
3. **Bit ACK/NACK:** El esclavo direccionado pone la línea `SDA` en bajo (ACK) para confirmar su presencia.
4. **Transferencia de Datos:** Bytes de 8 bits seguidos de su respectivo ACK.
5. **Condición STOP:** La línea `SDA` pasa de bajo a alto mientras `SCL` está en nivel alto, liberando el bus.

---

## 3. Elementos en SimulIDE (`circuito.sim1`)
1. **Arduino Uno:** Pines `A4` (SDA) y `A5` (SCL).
2. **Resistencias Pull-Up:** Dos resistores de $4.7\text{ k}\Omega$ conectadas a la línea de $5\text{ V}$.
3. **Display / Módulo I2C:** Conectado en paralelo a las líneas SDA y SCL.
4. **Osciloscopio Virtual de 2 Canales:**
   - **Canal 1 (Amarillo):** Línea de reloj `SCL`.
   - **Canal 2 (Azul):** Línea de datos `SDA`.

---

## 4. Código Fuente del Firmware (`firmware/i2c_sensor_display.ino`)
Incluye una rutina de escaneo del bus I2C (`Wire.beginTransmission` y `Wire.endTransmission`) para reportar qué direcciones de dispositivos responden en el bus, seguido del envío estructurado de datos clínicos.

---

## 5. Simulación en SimulIDE
1. Abre [`circuito.sim1`](file:///d:/DOCUMENTOS/PROJECTS/HARDTES/3_ARDUINO_UNO_SENSORES/05-protocolo-i2c-sensores-oled/circuito.sim1) en SimulIDE.
2. Carga el firmware compilado y enciende el circuito.
3. Observa en el Monitor Serie el escaneo de direcciones (confirmando la respuesta en `0x3C` o `0x27`) y visualiza en el Osciloscopio los trenes de pulsos de reloj y datos síncronos.
