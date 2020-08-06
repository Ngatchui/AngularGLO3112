import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { SentmessageComponent } from './sentmessage.component';

describe('SentmessageComponent', () => {
  let component: SentmessageComponent;
  let fixture: ComponentFixture<SentmessageComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ SentmessageComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SentmessageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
