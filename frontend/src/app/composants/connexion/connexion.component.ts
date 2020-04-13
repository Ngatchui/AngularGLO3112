import { Component, OnInit } from '@angular/core';
import {FormGroup} from '@angular/forms';

@Component({
  selector: 'app-connexion',
  templateUrl: './connexion.component.html',
  styleUrls: ['./connexion.component.css']
})
export class ConnexionComponent implements OnInit {
  loginForm: FormGroup;
  loading = false;
  submitted = false;

  constructor() { }

  ngOnInit(): void {
  }

  onSubmit() {
    this.submitted = true;

    // stop here if form is invalid
    if (this.loginForm.invalid) {
      return;
    }

    this.loading = true;
  }
    seConnecter() {
    }
}
