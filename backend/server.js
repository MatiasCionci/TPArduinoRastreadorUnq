const express = require('express');
const cors = require('cors');
const app = express();
const PORT = 3000;

app.use(cors());
app.use(express.json()); // Para que entienda JSON si Arduino manda un POST

// Variable temporal para guardar la última ubicación recibida
let ultimaUbicacion = { lat: -34.6037, lng: -58.3816 }; // Coordenadas por defecto (ej. Obelisco)

// 1. Ruta que va a recibir las coordenadas (Ej: Arduino o HTTP Post)
// Puede ser por GET usando query params: /api/coordenadas?lat=X&lng=Y
app.get('/api/actualizar', (req, res) => {
    const { lat, lng } = req.query;
    
    if (!lat || !lng) {
        return res.status(400).json({ error: "Faltan las coordenadas lat o lng" });
    }

    ultimaUbicacion = {
        lat: parseFloat(lat),
        lng: parseFloat(lng)
    };

    console.log(`Ubicación actualizada desde Arduino: Lat ${lat}, Lng ${lng}`);
    res.json({ status: "OK", ubicacionRecibida: ultimaUbicacion });
});

// 2. Ruta que va a consumir Angular para leer la ubicación actual
app.get('/api/ubicacion', (req, res) => {
    res.json(ultimaUbicacion);
});

app.listen(PORT, () => {
    console.log(`Servidor backend corriendo en http://localhost:${PORT}`);
});