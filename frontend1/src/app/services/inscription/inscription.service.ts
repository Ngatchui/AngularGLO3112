import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Router } from '@angular/router';



@Injectable({
  providedIn: 'root'
})
export class InscriptionService {

 private  baseUrl = 'http://localhost:8080'; /* apiUrl: string = 'enter-your-api-url'; */

  constructor(private http: HttpClient, private router: Router) { }

  saveUser(user: Object): Observable<any> {
    return this.http.post (`${this.baseUrl}`, user);
  }

  getUserList(): Observable<any> {
    return this.http.get(`${this.baseUrl}/inscription`);
  }
  
  getUser(id: number): Observable<any> {
    return this.http.get(`${this.baseUrl}/inscription/${id}`);
  }

  updateUser(id: number, value: any): Observable<Object> {
    return this.http.put(`${this.baseUrl}/inscription/${id}`, value);
  }

  deleteUser(id: number): Observable<any> {
    return this.http.delete(`${this.baseUrl}/inscription/${id}`, { responseType: 'text' });
  }

}
