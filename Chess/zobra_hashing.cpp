//
//  zobra_hashing.cpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//

#include "zobra_hashing.hpp"

// A program to illustrate Zobrist Hashing Algorithm
#include <iostream>
#include <random>
#include <fstream>
#include "utilities.hpp"

// random piece keys [piece][square]
U64 piece_keys[12][8][8];

// random enpassant keys [square]
U64 enpassant_keys[8][8];

// random castling keys
U64 castle_keys[16];

// random side key
U64 side_key;

// pseudo random number state
unsigned int random_state = 1804289383;


// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number()
{
    // get current state
    unsigned int number = random_state;
    
    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    
    // update random number state
    random_state = number;
    
    // return random number
    return number;
}

U64 get_random_U64_number()
{
    // define 4 random numbers
    U64 n1, n2, n3, n4;
    
    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;
    
    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

// Initializes the table
void initTable()
{
    
    for (int piece = 0; piece <12; piece++){
        // loop over board squares
        for (int line = 0; line < 8; line++){
            for (int col = 0; col < 8; col++){
                // init random piece keys
                piece_keys[piece][line][col] = get_random_U64_number();
            }
        }
    }
    // loop over board squares
    for (int line = 0; line < 8; line++){
        for(int col= 0; col < 8; col ++){
            // init random enpassant keys
            enpassant_keys[line][col] = get_random_U64_number();
        }
    }
    // loop over castling keys
    for (int index = 0; index < 16; index++){
        // init castling keys
        castle_keys[index] = get_random_U64_number();
    }
    // init random side key
    side_key = get_random_U64_number();
}
 
int castle_rights(Piece (&pos)[8][8]){
    /*
    bin  dec
        
       0001    1  white king can castle to the king side
       0010    2  white king can castle to the queen side
       0100    4  black king can castle to the king side
       1000    8  black king can castle to the queen side
       examples
       1111       both sides an castle both directions
       1001       black king => queen side
                  white king => king side
     It doesn't deal with pins and checks
     */
    int rights = 0;
    
    //for white king
    if((pos[0][4].code == 5) and (not pos[0][4].has_moved)){
        if ((pos[0][5].code == -1) and (pos[0][6].code == -1) and (pos[0][7].code == 3) and (not pos[0][7].has_moved)){//short castle
            rights += 1;
        }
        if ((pos[0][3].code == -1) and (pos[0][2].code == -1) and (pos[0][0].code == 3) and (not pos[0][0].has_moved)){//short castle
            rights += 2;
        }
    }
    //for black king
    if((pos[7][4].code == 11) and (not pos[7][4].has_moved)){
        if ((pos[7][5].code == -1) and (pos[7][6].code == -1) and (pos[7][7].code == 3) and (not pos[7][7].has_moved)){//short castle
            rights += 4;
        }
        if ((pos[7][3].code == -1) and (pos[7][2].code == -1) and (pos[7][0].code == 3) and (not pos[7][0].has_moved)){//short castle
            rights += 8;
        }
    }
    return rights;
}

// Computes the hash value of a given position
unsigned long long int computeHash(Piece (&pos)[8][8], bool turn){
    unsigned long long int h = 0;
    for (int i = 0; i<8; i++)
    {
        for (int j = 0; j<8; j++)
        {
            if (pos[i][j].code != -1)
            {
                h ^= piece_keys[i][j][pos[i][j].code];
                if (pos[i][j].en_passant){
                    h ^= enpassant_keys[i][j];
                }
            }
        }
    }
    h ^= castle_keys[castle_rights(pos)];
    if (!turn){
        h ^= side_key;
    }
    
    return h;
}

