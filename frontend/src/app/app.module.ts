import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ConnexionComponent } from './composants/connexion/connexion.component';
import { EnteteDePageComponent } from './composants/entete-de-page/entete-de-page.component';
import { PiedDePageComponent } from './composants/pied-de-page/pied-de-page.component';
import { InscriptionComponent } from './composants/inscription/inscription.component';
import { AccueilComponent } from './composants/accueil/accueil.component';
import { ReactiveFormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';



@NgModule({
  declarations: [
    AppComponent,
    ConnexionComponent,
    EnteteDePageComponent,
    PiedDePageComponent,
    InscriptionComponent,
    AccueilComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    ReactiveFormsModule,
    HttpClientModule,
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
