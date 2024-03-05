#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>

class Account {
    private:
        std::string name;
        std::string email;
        std::string password;

    public:
        Account(std::string name, std::string email, std::string password);
        std::string getName() const;
        std::string getEmail() const;
        std::string getPassword() const;
};

#endif // ACCOUNT_HPP