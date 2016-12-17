
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
    User(std::string loginTime = "");
    User(const User& original);
    virtual ~User();
    
    std::string getUsername();
    std::string getLoginTime();

private:
    std::string username;
    std::string loginTime;
};

#endif /* USER_H */

