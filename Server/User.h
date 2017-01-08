
/*
 * File:   User.h
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 5:29 PM
 */

#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User() {} 
    User(const std::string& username, const std::string& loginTime) { this->username = username; this->loginTime = loginTime; }
    virtual ~User() {} 

    std::string getUsername() const { return username; }
    std::string getLoginTime() const { return loginTime; }

private:
    std::string username = "asdsad";
    std::string loginTime;
};

#endif /* USER_H */
