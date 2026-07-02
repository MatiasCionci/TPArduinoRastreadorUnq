import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class Mapa {
  // Acá le pegamos a la ruta de Node que armamos en el servidor
  private apiUrl = 'http://192.168.0.33:5000/api/ubicacion';

  constructor(private http: HttpClient) { }

  // Este método va a ir a buscar las coordenadas actuales
  getUbicacion(): Observable<{lat: number, lng: number}> {
    return this.http.get<{lat: number, lng: number}>(this.apiUrl);
  }
}