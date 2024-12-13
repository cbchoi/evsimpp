#pragma once

#include <iostream>
#include <mutex>

// Singleton Class
class SystemSimulator {
public:
    // Public method to access the instance
    static SystemSimulator& getInstance() {
        static SystemSimulator instance; // Guaranteed to be thread-safe in C++11 and later
        return instance;
    }

    // Deleting copy constructor and assignment operator to prevent copies
    SystemSimulator(const SystemSimulator&) = delete;
    SystemSimulator& operator=(const SystemSimulator&) = delete;

    // Example method
    void showMessage() {
        std::cout << "Singleton Instance Address: " << this << std::endl;
    }

private:
    // Private constructor to prevent instantiation
    SystemSimulator() {
        std::cout << "Singleton Constructor Called!" << std::endl;
    }

    // Private destructor (optional)
    ~SystemSimulator() {
        std::cout << "Singleton Destructor Called!" << std::endl;
    }
};