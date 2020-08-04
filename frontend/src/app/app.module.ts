import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { FormsModule } from '@angular/forms';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './components/login/login.component';
import { RegisterComponent } from './components/register/register.component';
import { UpdateUserComponent } from './components/update-user/update-user.component';
import { UserListeComponent } from './components/user-liste/user-liste.component';
import { ProfilComponent } from './components/profil/profil.component';
import { ProfilUserComponent } from './components/profil-user/profil-user.component';
import { RegistrationComponent } from './components/registration/registration.component';
import { ParametersComponent } from './components/parameters/parameters.component';
import { HomeComponent } from './components/home/home.component';
import { ReceptionComponent } from './components/reception/reception.component';
import { HttpClientModule } from '@angular/common/http';


@NgModule({
  declarations: [
    AppComponent,
    RegisterComponent,
    LoginComponent,
    UpdateUserComponent,
    UserListeComponent,
    ProfilComponent,
    ProfilUserComponent,
    RegistrationComponent,
    ParametersComponent,
    HomeComponent,
    ReceptionComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
