/*
 * Laboratorio 03: Comunicación UART e Intérprete de Comandos en C++
 * Curso: Hardware para Telesalud
 * Universidad Peruana Cayetano Heredia (UPCH)
 *
 * Descripción:
 * Intérprete de comandos serie no bloqueante para control y consulta remota
 * de un monitor biomédico basado en Arduino Uno.
 */

const int PIN_ALERTA = 13;
bool alarmaActiva = false;

// Variables simuladas de paciente
int pulsoActual = 75;
int saturacionActual = 98;
float temperaturaActual = 36.7;

// Buffer para acumular el comando entrante
String bufferComando = "";

void procesarComando(const String &cmd) {
  String c = cmd;
  c.trim();
  c.toUpperCase();

  if (c == "GET_STATUS") {
    Serial.print("{\"dispositivo\":\"ARDUINO-MONITOR-01\",\"uptime_s\":");
    Serial.print(millis() / 1000);
    Serial.print(",\"alarma\":");
    Serial.print(alarmaActiva ? "true" : "false");
    Serial.println(",\"status\":\"OPERACIONAL\"}");
  } 
  else if (c == "GET_VITALS") {
    // Variación fisiológica leve simulada
    pulsoActual = 72 + random(-3, 6);
    saturacionActual = 97 + random(0, 3);

    Serial.print("{\"bpm\":");
    Serial.print(pulsoActual);
    Serial.print(",\"spo2\":");
    Serial.print(saturacionActual);
    Serial.print(",\"temp_c\":");
    Serial.print(temperaturaActual, 1);
    Serial.println("}");
  } 
  else if (c == "ALARM_ON") {
    alarmaActiva = true;
    digitalWrite(PIN_ALERTA, HIGH);
    Serial.println("[ACK] Alarma luminosa ACTIVADA.");
  } 
  else if (c == "ALARM_OFF") {
    alarmaActiva = false;
    digitalWrite(PIN_ALERTA, LOW);
    Serial.println("[ACK] Alarma luminosa SILENCIADA.");
  } 
  else if (c == "HELP") {
    Serial.println("--- COMANDOS DISPONIBLES EN TELESALUD UPCH ---");
    Serial.println("  GET_STATUS  : Reporta estado del hardware y tiempo activo");
    Serial.println("  GET_VITALS  : Retorna constantes vitales actuales en JSON");
    Serial.println("  ALARM_ON    : Activa indicador lumínico de alerta");
    Serial.println("  ALARM_OFF   : Silencia indicador de alerta");
    Serial.println("  HELP        : Muestra este menú");
  } 
  else {
    Serial.print("[ERROR] Comando no reconocido: '");
    Serial.print(c);
    Serial.println("'. Escriba HELP para ayuda.");
  }
}

void setup() {
  pinMode(PIN_ALERTA, OUTPUT);
  digitalWrite(PIN_ALERTA, LOW);

  Serial.begin(115200);
  delay(100);

  Serial.println("==================================================");
  Serial.println(" UPCH - Hardware para Telesalud");
  Serial.println(" Lab 03: Intérprete Serial UART listo (115200 bps)");
  Serial.println(" Escriba 'HELP' y presione Enter para iniciar.");
  Serial.println("==================================================");
}

void loop() {
  // Lectura no bloqueante byte a byte
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();

    if (inChar == '\n' || inChar == '\r') {
      if (bufferComando.length() > 0) {
        procesarComando(bufferComando);
        bufferComando = ""; // Limpiar buffer para el próximo comando
      }
    } else {
      bufferComando += inChar;
    }
  }
}
