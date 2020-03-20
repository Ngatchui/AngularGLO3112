import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { EnteteDePageComponent } from './entete-de-page.component';

describe('EnteteDePageComponent', () => {
  let component: EnteteDePageComponent;
  let fixture: ComponentFixture<EnteteDePageComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ EnteteDePageComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(EnteteDePageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
