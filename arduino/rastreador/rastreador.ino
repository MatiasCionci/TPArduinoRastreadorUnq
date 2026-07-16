#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// --- Configuración GPS ---
const int GPS_RX = 4;
const int GPS_TX = 3;
const int GPSBaud = 9600;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus gps;

// --- Configuración ESP8266 ---
const int ESP_RX = 10;
const int ESP_TX = 11;
SoftwareSerial esp8266(ESP_RX, ESP_TX);
String serverIP = "10.12.18.219";
String serverPort = "5000";

unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 10000; // Enviar cada 10 segundos (10000 ms)

void setup() {
  Serial.begin(9600);
  
  gpsSerial.begin(GPSBaud);
  esp8266.begin(9600);

  Serial.println(F("--- Iniciando sistema GPS + WiFi ---"));
  Serial.println(F("Esperando 5 segundos para asegurar la conexion WiFi del ESP..."));
  delay(5000);
 
  gpsSerial.listen();
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (millis() - ultimoEnvio >= intervaloEnvio) {
    ultimoEnvio = millis();

    if (gps.location.isValid()) {
      String latitud = String(gps.location.lat(), 6);
      String longitud = String(gps.location.lng(), 6);

      Serial.print(F("Ubicación válida: Lat: ")); Serial.print(latitud);
      Serial.print(F(" | Lng: ")); Serial.println(longitud);
      
      // --- PROCESO DE ENVÍO ---
      esp8266.listen(); 
      
      enviarCoordenadas(latitud, longitud);
      
      gpsSerial.listen(); 
      
    } else {
      Serial.println(F("Esperando señal de satélites válida para poder enviar datos..."));
    }
  }
}

void enviarCoordenadas(String lat, String lng) {
  
  Serial.println(F("1. Abriendo conexion TCP..."));
  String cmd = "AT+CIPSTART=\"TCP\",\"" + serverIP + "\"," + serverPort;
  esp8266.println(cmd);
  delay(2000); 
  imprimirRespuestaESP();

  String peticionHTTP = "GET /api/actualizar?lat=" + lat + "&lng=" + lng + " HTTP/1.1\r\n";
  peticionHTTP += "Host: " + serverIP + "\r\n";
  peticionHTTP += "Connection: close\r\n\r\n"; 

  Serial.println(F("2. Definiendo tamano del paquete..."));
  cmd = "AT+CIPSEND=" + String(peticionHTTP.length());
  esp8266.println(cmd);
  delay(1000);
  imprimirRespuestaESP();

  Serial.println(F("3. Enviando peticion HTTP..."));
  esp8266.print(peticionHTTP);
  delay(2000);
  imprimirRespuestaESP();

  Serial.println(F("==== Intento de envio terminado ===="));
}

void imprimirRespuestaESP() {
  while (esp8266.available()) {
    String linea = esp8266.readStringUntil('\n');
    Serial.println(linea);
  }
}
