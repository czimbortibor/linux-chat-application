
/*
 * File:   User.cpp
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 5:29 PM
 */

#include "User.h"

User::User(std::string loginTime) {
    this->loginTime = loginTime;
}

User::User(const User& original) {
}

User::~User() {
}
