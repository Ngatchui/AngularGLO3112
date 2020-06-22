import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class RegisterService {

  private baseUrl = 'http://localhost:8080/register';

  constructor(private http: HttpClient) { }

  getUser(id: number): Observable<any>{
    return this.http.get(`${this.baseUrl}/${id}`);
  }

  // tslint:disable-next-line:ban-types
  registerUser(user: Object): Observable<Object>{
    return this.http.post(`${this.baseUrl}`, user);
  }

  deleteUser(id: number): Observable<any>{
    return this.http.delete(`${this.baseUrl}/${id}`, { responseType: 'text'});
  }

  // tslint:disable-next-line:ban-types
  updateUser(id: number, value: any): Observable<Object> {
    return this.http.put(`${this.baseUrl}/${id}`, value);
  }

  getUsersList(): Observable<any> {
    return this.http.get(`${this.baseUrl}`);
  }
}
