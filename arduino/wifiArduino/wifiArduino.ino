#include <SoftwareSerial.h>

// RX de Arduino (Pin 2) al TX del ESP
// TX de Arduino (Pin 3) al RX del ESP
SoftwareSerial esp8266(10,11); 

// Configuración de tu servidor Node
// Cambia esto si la IP de tu computadora cambia
String serverIP = "10.12.18.219";
String serverPort = "5000";

void setup() {
  Serial.begin(9600);
  esp8266.begin(9600); // Ajusta a 9600 si tu ESP quedó a esa velocidad

  Serial.println("Esperando 5 segundos para asegurar la conexion WiFi del ESP...");
  delay(5000);

  // Llamamos a la función con coordenadas de prueba (Bernal)
  Serial.println("Iniciando prueba de envio al servidor Node...");
  enviarCoordenadas("-33.7065", "-58.2778");
}

void loop() {
  // En este ejemplo enviamos el dato una sola vez en el setup.
  // Si quieres que se envíe cada 10 segundos, puedes descomentar esto:
  
  // delay(10000);
  // enviarCoordenadas("-34.7065", "-58.2778");
}

// Función que automatiza los comandos AT para un HTTP GET
void enviarCoordenadas(String lat, String lng) {
  
  // 1. Abrir conexión TCP con el servidor Node
  Serial.println("1. Abriendo conexion TCP...");
  String cmd = "AT+CIPSTART=\"TCP\",\"" + serverIP + "\"," + serverPort;
  esp8266.println(cmd);
  delay(2000); // Le damos 2 segundos para establecer la conexión
  imprimirRespuestaESP();

  // 2. Armar la petición HTTP GET exacta que espera Node.js
  String peticionHTTP = "GET /api/actualizar?lat=" + lat + "&lng=" + lng + " HTTP/1.1\r\n";
  peticionHTTP += "Host: " + serverIP + "\r\n";
  peticionHTTP += "Connection: close\r\n\r\n"; // El doble \r\n final es vital en HTTP

  // 3. Avisar al ESP-01S cuántos bytes (caracteres) vamos a enviarle
  Serial.println("2. Definiendo tamano del paquete...");
  cmd = "AT+CIPSEND=" + String(peticionHTTP.length());
  esp8266.println(cmd);
  delay(1000);
  imprimirRespuestaESP();

  // 4. Enviar la petición HTTP real
  Serial.println("3. Enviando peticion HTTP...");
  esp8266.print(peticionHTTP);
  delay(2000);
  imprimirRespuestaESP();

  Serial.println("==== Intento de envio terminado ====");
}

// Función auxiliar para leer lo que nos responde el ESP y mostrarlo
void imprimirRespuestaESP() {
  while (esp8266.available()) {
    String linea = esp8266.readStringUntil('\n');
    Serial.println(linea);
  }
}