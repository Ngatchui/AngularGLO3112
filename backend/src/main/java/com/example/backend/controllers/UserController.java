package com.example.backend.controllers;

import com.example.backend.domaine.models.User;
import com.example.backend.domaine.repositories.UserRepository;
import com.example.backend.services.servicesRepositories.UserServiceRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@CrossOrigin(origins = "http://localhost:4200")
@RequestMapping("/register")
public class UserController {

    @Autowired
    UserRepository userRepository;

//    @RequestMapping(value = "/register", method = RequestMethod.GET)
    @GetMapping()
    public List<User> getUsers(){
        return  userRepository.findAll();
    }

//    @RequestMapping(value = "/register/{id}", method = RequestMethod.PUT)
//    public ResponseEntity<Object> updateUser(@PathVariable("id") Long id, @RequestBody User user){
//        userServiceRepository.updateUser(id, user);
//        return new ResponseEntity<>("User is updated successfully", HttpStatus.OK);
//    }
//
//    @RequestMapping(value = "/register/{id}", method = RequestMethod.DELETE)
//    public ResponseEntity<Object> deleteUser(@PathVariable("id") Long id){
//        userServiceRepository.deleteUser(id);
//        return new ResponseEntity<>("User is deleted successfully", HttpStatus.OK);
//    }
//
//    @RequestMapping(value = "/register", method = RequestMethod.POST)
//    public ResponseEntity<Object> createUser(@RequestBody User user){
//        userServiceRepository.createUser(user);
//        return new ResponseEntity<>("User is created successfully", HttpStatus.CREATED);
//    }

}
