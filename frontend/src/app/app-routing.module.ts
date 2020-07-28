import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { RegisterComponent } from './components/register/register.component';
import { LoginComponent } from './components/login/login.component';
import { UserListeComponent } from './components/user-liste/user-liste.component';
import { ProfilComponent } from './components/profil/profil.component';
import { ProfilUserComponent } from './components/profil-user/profil-user.component';
import { RegistrationComponent } from './components/registration/registration.component';
import { ParametersComponent } from './components/parameters/parameters.component';
import { HomeComponent } from './components/home/home.component';
import { ReceptionComponent } from './components/reception/reception.component';


const routes: Routes = [
  { path: '', redirectTo: '/reception', pathMatch: 'full'},
  { path: 'register', component: RegisterComponent},
  { path: 'login', component: LoginComponent},
  { path: 'registers', component: UserListeComponent},
  { path: 'profil', component: ProfilComponent},
  { path: 'profilUser', component: ProfilUserComponent},
  { path: 'registration', component: RegistrationComponent},
  { path: 'parameters', component: ParametersComponent},
  { path: 'home', component: HomeComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
