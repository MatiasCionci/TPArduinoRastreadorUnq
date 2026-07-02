import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Mapa } from '../services/mapa'; // Asegurate de que la ruta a tu servicio sea esta
import * as L from 'leaflet';

@Component({
  selector: 'app-mapa-visor',
  standalone: true,
  imports: [CommonModule], // Agregamos CommonModule por si usás directivas de Angular
  templateUrl: './mapa-visor.html',
  styleUrl: './mapa-visor.css',
})
export class MapaVisor implements OnInit {
  private map!: L.Map;
  private marker!: L.Marker;

  // Inyectamos tu servicio que se llama "Mapa"
  constructor(private mapaService: Mapa) {}

  ngOnInit() {
    this.initMap();
    
    // Consultamos al backend cada 3 segundos para ver si Arduino mandó coordenadas
    setInterval(() => {
      this.getArduinoLocation();
    }, 3000);
  }

  private initMap() {
    // Inicializar el mapa centrado (ej. Buenos Aires / Quilmes)
    this.map = L.map('map').setView([-34.6037, -58.3816], 13);

    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
      attribution: '© OpenStreetMap contributors'
    }).addTo(this.map);

    // Configurar el pin azul por defecto usando CDN para que no se rompa la imagen
    const iconDefault = L.icon({
      iconUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon.png',
      shadowUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-shadow.png',
      iconSize: [25, 41],
      iconAnchor: [12, 41]
    });

    this.marker = L.marker([-34.6037, -58.3816], { icon: iconDefault }).addTo(this.map);
  }

  getArduinoLocation() {
    this.mapaService.getUbicacion().subscribe({
      next: (data) => {
        if (data && data.lat && data.lng) {
          const nuevaPos: L.LatLngExpression = [data.lat, data.lng];
          
          // Movemos el marcador en tiempo real
          this.marker.setLatLng(nuevaPos);
          // Centramos el mapa suavemente
          this.map.panTo(nuevaPos);
        }
      },
      error: (e) => console.error("El backend de Node no responde:", e)
    });
  }
}