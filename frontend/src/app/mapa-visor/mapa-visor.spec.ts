import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MapaVisor } from './mapa-visor';

describe('MapaVisor', () => {
  let component: MapaVisor;
  let fixture: ComponentFixture<MapaVisor>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [MapaVisor],
    }).compileComponents();

    fixture = TestBed.createComponent(MapaVisor);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
