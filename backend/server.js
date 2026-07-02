const express = require('express');
const cors = require('cors');
const app = express();

// Usamos el puerto 5000 para que no choque con Grafana si lo usás en el 3000
const PORT = 5000; 

app.use(cors());
app.use(express.json());

// --- VARIABLES LOCALES EN MEMORIA DEL SERVIDOR ---
// Arrancan con una ubicación por defecto
let latitudLocal = -34.7242;
let longitudLocal = -58.2608;

// 1. RUTA PARA EL ARDUINO: Modifica las variables locales
// El Arduino entrará a: http://192.168.0.33:5000/api/actualizar?lat=XXX&lng=YYY
app.get('/api/actualizar', (req, res) => {
    const { lat, lng } = req.query;
    
    if (!lat || !lng) {
        return res.status(400).json({ error: "Faltan datos de latitud o longitud" });
    }

    // Convertimos a número flotante y actualizamos las variables locales del servidor
    latitudLocal = parseFloat(lat);
    longitudLocal = parseFloat(lng);

    console.log(`[Arduino] Variables actualizadas -> Lat: ${latitudLocal}, Lng: ${longitudLocal}`);
    
    // Le respondemos al Arduino que recibimos todo OK
    res.json({ status: "OK", mensaje: "Coordenadas guardadas en el servidor" });
});

// 2. RUTA PARA ANGULAR: Lee las variables locales actuales
// Angular consultará: http://192.168.0.33:5000/api/ubicacion
app.get('/api/ubicacion', (req, res) => {
    // Respondemos con el valor que tengan las variables locales en este microsegundo
    res.json({
        lat: latitudLocal,
        lng: longitudLocal
    });
});

// ESCUCHAR EN LA IP LOCAL: Al poner '0.0.0.0', permitís que el Arduino entre usando tu IP 192.168.0.33
app.listen(PORT, '0.0.0.0', () => {
    console.log(`Servidor de Node corriendo en la red local!`);
    console.log(`- Local (esta compu): http://localhost:${PORT}`);
    console.log(`- Red (para el Arduino): http://192.168.0.33:${PORT}`);
});