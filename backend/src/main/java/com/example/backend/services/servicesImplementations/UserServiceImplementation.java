package com.example.backend.services.servicesImplementations;

import com.example.backend.domaine.models.User;
import com.example.backend.domaine.repositories.UserRepository;
import com.example.backend.services.servicesRepositories.UserServiceRepository;
import org.springframework.stereotype.Service;

import java.util.*;

@Service
public class UserServiceImplementation implements UserServiceRepository {

    private static Map<Long, User> userMap = new HashMap<>();

    private UserRepository userRepository;

    public UserServiceImplementation(UserRepository userRepository) {
        this.userRepository = userRepository;
    }


    @Override
    public void createUser(User user) {

    }

    @Override
    public void updateUser(Long id, User user) {

    }

    @Override
    public void deleteUser(Long id) {

    }

    @Override
    public Collection<User> getUsers() {
        return null;
    }
}
