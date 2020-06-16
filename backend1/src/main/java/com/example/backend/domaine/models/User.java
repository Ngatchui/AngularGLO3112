package com.example.backend.domaine.models;

import javax.persistence.*;

@Entity
@Table(name = "Clients")
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Long id;

    @Column(name = "UserName")
    private String userName;

    @Column(name = "LastName")
    private String lastName;

    @Column(name = "FirstName")
    private String firstName;

    @Column(name = "Courriel")
    private String email;

    @Column(name = "MotDePasse")
    private String motdePasse;


    public User() {
    }

    public Long getId_User() {
        return id;
    }

    public String getEmail() {
        return email;
    }

    public String getUserName() {
        return userName;
    }

    public String getLastName() {
        return lastName;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getMotdePasse() {
        return motdePasse;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public void setMotdePasse(String motdePasse) {
        this.motdePasse = motdePasse;
    }

    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", userName='" + userName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", firstName='" + firstName + '\'' +
                ", email='" + email + '\'' +
                ", motdePasse='" + motdePasse + '\'' +
                '}';
    }
}
