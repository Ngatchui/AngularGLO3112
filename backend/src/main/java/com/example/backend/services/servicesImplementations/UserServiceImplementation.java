package com.example.backend.services.servicesImplementations;

import com.example.backend.domaine.models.User;
import com.example.backend.domaine.repositories.UserRepository;
import com.example.backend.services.servicesRepositories.UserServiceRepository;
import org.springframework.stereotype.Service;

import java.util.*;

@Service
public class UserServiceImplementation implements UserServiceRepository {

    UserRepository userRepository;

    private static Map<Long, User> userMap = new HashMap<>();

    @Override
    public void createUser(User user) {
        userMap.put(user.getId(), user);
    }

    @Override
    public void updateUser(Long id, User user) {
        userMap.remove(id);
        user.setId(id);
        userMap.put(id,user);
    }

    @Override
    public void deleteUser(Long id) {
        userMap.remove(id);
    }

    @Override
    public List<User> getUsers() {
        return userRepository.findAll();
    }
}
