import { Component, OnInit } from '@angular/core';
import { User } from 'src/app/models/user';
import { RegisterService } from 'src/app/services/register/register.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent implements OnInit {
  user: User = new User();
  submitted = false;

  constructor(private registerService: RegisterService, private router: Router) { }

  ngOnInit(): void {
  }

  /* save() {
    this.registerService.registerUser(this.user)
      .subscribe(data => console.log(data), error => console.log(error));
    this.user = new User(); */
    /* this.gotoList();
 */
 /*  } */

  onSubmit(){
    this.submitted = true;
   /*  this.save(); */
    this.router.navigate(['/login']);
  }

  /* gotoList() {
    this.router.navigate(['/registers']);
  }
  il faut encore regarder le chemin pour afficher tous les utilisateurs
  */

}
