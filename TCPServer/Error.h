/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   error.h
 * Author: czimbortibor
 *
 * Created on December 13, 2016, 1:18 AM
 */

#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// inline, to prevent: for every file "Error.h" gets included to, a public function "error" will be created, resulting in a name clash
inline void error(const char* errorMsg) {
    perror(errorMsg);
    exit(1);
}

#endif /* ERROR_H */

