import { Component, OnInit } from '@angular/core';
import {FormBuilder, FormControl, FormGroup} from '@angular/forms';
import {User} from '../../models/user';

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

  constructor(private formBuilder: FormBuilder) {
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
  inscription() {
    this.user.email  = this.loginForm.get('email').value;
    this.user.username = this.loginForm.get('username').value;
    this.user.lastName = this.loginForm.get('lastName').value;
    this.user.firstName = this.loginForm.get('firstName').value;
    this.user.password = this.loginForm.get('password').value;
  }
}
