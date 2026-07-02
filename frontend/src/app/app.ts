import { Component, signal } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { MapaVisor } from './mapa-visor/mapa-visor'; // <-- AGREGÁ ESTA IMPORTACIÓN
@Component({
  selector: 'app-root',
  imports: [RouterOutlet,MapaVisor],
  templateUrl: './app.html',
  styleUrl: './app.css'
})
export class App {
  protected readonly title = signal('frontend');
}
