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

using namespace std;
 
unsigned long long int ZobristTable[8][8][12];
mt19937 mt(01234567);
 
// Generates a Random number from 0 to 2^64-1
unsigned long long int randomInt()
{
    uniform_int_distribution<unsigned long long int>
                                 dist(0, UINT64_MAX);
    return dist(mt);
}

// Initializes the table
void initTable()
{
    /*
     //Permet de sauvegarder la table de zobra
    ofstream fout;
    fout.open("/Users/Aser/Documents/Chess/Chess/zobra_table.txt");
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            for (int k = 0; k<12; k++){
                ZobristTable[i][j][k] = randomInt();
                fout << ZobristTable[i][j][k] <<"\n";
            }
        }
    }
    fout.close();
     */
    //This part loads the data initially saved in zobra_table.txt
    std::ifstream fin;
    fin.open("Chess/zobra_table.txt");
    std::string line;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            for (int k = 0; k<12; k++){
                // Read a Line from File
                getline(fin, line);
                ZobristTable[i][j][k] = str_to_i(line);
            }
        }
    }
    // Close the file
    fin.close();
    
}
 
// Computes the hash value of a given position
unsigned long long int computeHash(Piece pos[8][8])
{
    unsigned long long int h = 0;
    for (int i = 0; i<8; i++)
    {
        for (int j = 0; j<8; j++)
        {
            if (pos[i][j].code != -1)
            {
                h ^= ZobristTable[i][j][pos[i][j].code];
            }
        }
    }
    return h;
}

