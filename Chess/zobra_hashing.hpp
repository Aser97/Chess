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
// define bitboard data type
#define U64 unsigned long long int

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number();

U64 get_random_U64_number();
//init zobra table
void initTable();

int castle_rights(Piece (&pos)[8][8]);

// Computes the hash value of a given position
unsigned long long int computeHash(Piece (&pos)[8][8], bool turn);

#endif /* zobra_hashing_hpp */
