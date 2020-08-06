import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { LovepublicationComponent } from './lovepublication.component';

describe('LovepublicationComponent', () => {
  let component: LovepublicationComponent;
  let fixture: ComponentFixture<LovepublicationComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ LovepublicationComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(LovepublicationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
