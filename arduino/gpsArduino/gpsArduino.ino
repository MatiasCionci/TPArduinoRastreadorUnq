#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Mantenemos los mismos pines: Pin 4 (RX de Arduino, va al TX del GPS) y Pin 3 (TX de Arduino, va al RX del GPS)
const int RXPin = 4;
const int TXPin = 3;
const int GPSBaud = 9600;

// Creamos el procesador de datos GPS
TinyGPSPlus gps;

// Creamos el puerto serie virtual
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup() {
  // Comunicación con la computadora (Monitor Serie)
  Serial.begin(9600);
  
  // Comunicación con el GPS
  gpsSerial.begin(GPSBaud);

  Serial.println(F("--- Buscando satélites (Mantén el GPS al aire libre) ---"));
}

void loop() {
  // Leemos los datos que llegan del GPS y se los pasamos a la librería para que los "traduzca"
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      mostrarUbicacion();
    }
  }
}

void mostrarUbicacion() {
  // Si la librería logra descifrar una ubicación válida (cuando el LED del GPS parpadee)
  if (gps.location.isValid()) {
    Serial.print(F("Latitud: "));
    Serial.print(gps.location.lat(), 6); // Muestra 6 decimales de precisión
    Serial.print(F(" | Longitud: "));
    Serial.print(gps.location.lng(), 6);
    
    Serial.print(F(" | Satélites conectados: "));
    Serial.println(gps.satellites.value());
  } else {
    // Si todavía no tiene señal de los satélites, te avisa aquí
    Serial.println(F("Conexión OK, pero esperando señal de satélites..."));
  }
}