import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ConnexionComponent } from './composants/connexion/connexion.component';
import { EnteteDePageComponent } from './composants/entete-de-page/entete-de-page.component';
import { PiedDePageComponent } from './composants/pied-de-page/pied-de-page.component';


@NgModule({
  declarations: [
    AppComponent,
    ConnexionComponent,
    EnteteDePageComponent,
    PiedDePageComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
