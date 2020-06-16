package com.example.backend.controllers;

import com.example.backend.domaine.Exceptions.RecordNotFoundException;
import com.example.backend.domaine.models.User;
import com.example.backend.services.repositoriesServicesImplementations.UserImplementationService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@CrossOrigin(origins = "http://localhost:4200")
@RestController
@RequestMapping("/inscription")
public class UserController {


    @Autowired
    UserImplementationService userService;

    @GetMapping
    public ResponseEntity<List<User>> getAllUser(){
        List<User> list = userService.getAllUser();

        return new ResponseEntity<List<User>>(list, new HttpHeaders(), HttpStatus.OK);
    }

    @GetMapping("/{id}")
    public ResponseEntity<User> getUserById(@PathVariable("id") Long id) throws RecordNotFoundException {
        User user = userService.getUserById(id);

        return  new ResponseEntity<User>(user, new HttpHeaders(), HttpStatus.OK);
    }

    @PostMapping
    public ResponseEntity<User> createOrUpdate(User userParam) throws RecordNotFoundException {
        User userUpdate = userService.createOrUpdateUser(userParam);

        return new ResponseEntity<User>(userUpdate, new HttpHeaders(), HttpStatus.OK);
    }


    @DeleteMapping("/{id}")
    public HttpStatus deleteUserById(@PathVariable("id") Long id) throws RecordNotFoundException {
        userService.deleteUserById(id);

        return HttpStatus.FORBIDDEN;
    }
}
