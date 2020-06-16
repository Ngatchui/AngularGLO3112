package com.example.backend.domaine.repositories;

import com.example.backend.domaine.models.User;
import org.springframework.data.jpa.repository.JpaRepository;

public interface UserRepository extends JpaRepository<User, Long> {
}
