package com.example.backend.services.servicesRepositories;

import com.example.backend.domaine.models.User;

import java.util.Collection;
import java.util.List;

public interface UserServiceRepository {

public abstract  void createUser(User user);

public abstract  void updateUser(Long id, User user);

public abstract  void deleteUser(Long id);

public abstract List<User> getUsers();
}
