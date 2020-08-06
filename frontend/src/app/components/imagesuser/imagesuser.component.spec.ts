import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ImagesuserComponent } from './imagesuser.component';

describe('ImagesuserComponent', () => {
  let component: ImagesuserComponent;
  let fixture: ComponentFixture<ImagesuserComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ImagesuserComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ImagesuserComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
