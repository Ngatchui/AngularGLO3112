import { NgModule, Component } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { ConnexionComponent } from './composants/connexion/connexion.component';



const routes: Routes = [
  {path: 'connexion', component: ConnexionComponent},
];

@NgModule({
  imports: [
    RouterModule.forRoot(routes)
  ],
  exports: [
    RouterModule
  ]
})
export class AppRoutingModule { }
