package com.example.backend.services.repositoriesServicesImplementations;

import com.example.backend.domaine.Exceptions.RecordNotFoundException;
import com.example.backend.domaine.models.User;
import com.example.backend.domaine.repositories.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Service
public class UserImplementationService {

    @Autowired
    UserRepository userRepository;

    public List<User> getAllUser() {
        List<User> userList = userRepository.findAll();

        if(userList.size() > 0){
            return userList;
        } else {
            return new ArrayList<User>();
        }
    }

    public User getUserById(Long id) throws RecordNotFoundException {
        Optional<User> user = userRepository.findById(id);

        if(user.isPresent()){
            return user.get();
        } else {
            throw new RecordNotFoundException("No user record exist for given id");
        }
    }

    public User createOrUpdateUser(User userParam){
        Optional<User> user = userRepository.findById(userParam.getId_User());

        if(user.isPresent()){
            User newUser = user.get();
            newUser.setUserName(userParam.getUserName());
            newUser.setLastName(userParam.getLastName());
            newUser.setFirstName(userParam.getFirstName());
            newUser.setEmail(userParam.getEmail());
            newUser.setMotdePasse(userParam.getMotdePasse());

            newUser = userRepository.save(newUser);

            return  newUser;
        } else {
            userParam = userRepository.save(userParam);

            return userParam;
        }
    }

    public void deleteUserById(Long id) throws RecordNotFoundException {

        Optional<User> user = userRepository.findById(id);

        if(user.isPresent()) {
            userRepository.deleteById(id);
        } else {
            throw new RecordNotFoundException("No user record exist for given id");
        }
    }
}
