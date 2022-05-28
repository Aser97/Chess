//
//  zobra_hashing.hpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//

// Generates a Random number from 0 to 2^64-1

#ifndef zobra_hashing_hpp
#define zobra_hashing_hpp

#include <iostream>
#include "utilities.hpp"

unsigned long long int randomInt();

//Convert a string to int
unsigned long long int str_to_i(std::string s);

//init zobra table
void initTable();

// Computes the hash value of a given position
unsigned long long int computeHash(Piece pos[8][8]);

#endif /* zobra_hashing_hpp */
