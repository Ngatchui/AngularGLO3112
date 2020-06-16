import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ConnexionComponent } from './composants/connexion/connexion.component';
import { InscriptionComponent } from './composants/inscription/inscription.component';
import { AccueilComponent } from './composants/accueil/accueil.component';
import { ReactiveFormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { InscriptionService } from './services/inscription/inscription.service';




@NgModule({
  declarations: [
    AppComponent,
    ConnexionComponent,
    InscriptionComponent,
    AccueilComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    ReactiveFormsModule,
    HttpClientModule,
  ],
  providers: [InscriptionService],
  bootstrap: [AppComponent]
})
export class AppModule { }
