import { Component, OnInit } from '@angular/core';
import {FormBuilder, FormControl, FormGroup} from '@angular/forms';
import {User} from '../../models/user';
import { Router } from '@angular/router';
import { InscriptionService } from 'src/app/services/inscription/inscription.service';

@Component({
  selector: 'app-inscription',
  templateUrl: './inscription.component.html',
  styleUrls: ['./inscription.component.css']
})
export class InscriptionComponent implements OnInit {
  loginForm: FormGroup;
  loading = false;
  submitted = false;
  user: User = new User();

  constructor(private formBuilder: FormBuilder, private router: Router, private inscriptionService: InscriptionService) {
  }

  ngOnInit(): void {
    this.loginForm = this.formBuilder.group({
      email: [' ' ],
      username: [' '],
      lastName: [' ' ],
      firstName: [' '],
      password: [' ' ],
      passwordconfirm: [' ' ],
    });
  }

  newUser() {
    this.submitted = false;
    this.user = new User();
  }


  inscription(): void {
    this.user.email  = this.loginForm.get('email').value;
    this.user.username = this.loginForm.get('username').value;
    this.user.lastName = this.loginForm.get('lastName').value;
    this.user.firstName = this.loginForm.get('firstName').value;
    this.user.password = this.loginForm.get('password').value;
    console.log(this.user);

    this.inscriptionService.saveUser(this.user)
      .subscribe(data => console.log(data), error => console.log(error));
    this.user = new User(); 
  }

  onSubmit() {
    this.submitted = true;
    this.inscription();
  }
}
