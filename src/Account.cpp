#include "Account.hpp"

Account::Account(std::string name, std::string email, std::string password) {
    this->name = name;
    this->email = email;
    this->password = password;
}

std::string Account::getName() const {
    return this->name;
}

std::string Account::getEmail() const {
    return this->email;
}

std::string Account::getPassword() const {
    return this->password;
}