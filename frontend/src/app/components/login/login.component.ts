import { Component, OnInit } from '@angular/core';
import { User } from 'src/app/models/user';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  
  user: User = new User();
  submitted = false;

  constructor() { }

  ngOnInit(): void {
  }

  onSubmit(){
    
  }
}
