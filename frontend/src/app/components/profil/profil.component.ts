import { Component, OnInit } from '@angular/core';
import { Route } from '@angular/compiler/src/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-profil',
  templateUrl: './profil.component.html',
  styleUrls: ['./profil.component.css']
})
export class ProfilComponent implements OnInit {

  constructor(private router: Router) { }

  ngOnInit(): void {
  }

  goToProfil(){
    this.router.navigate(['/profilUser']);
  }

  goToSave(){
    this.router.navigate(['/registration']);
  }

  goToParameters(){
    this.router.navigate(['/parameters']);
  }

  goToDeconnexion(){}

}
