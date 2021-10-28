import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { AllarmeModalePage } from './allarme-modale.page';

describe('AllarmeModalePage', () => {
  let component: AllarmeModalePage;
  let fixture: ComponentFixture<AllarmeModalePage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AllarmeModalePage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(AllarmeModalePage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
