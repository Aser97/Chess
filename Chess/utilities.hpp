//
//  utilities.hpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//
#ifndef utilities_hpp
#define utilities_hpp

#include <iostream>
#include <vector>

//this function tests whether two pieces are in same color
bool are_nemesis(int code1, int code2);

bool from_a_to_h(char c);

bool from_1_to_8(char c);

int letter_to_int(char c, bool whose_turn);

std::pair<bool, int > findInVector(std::vector<std::tuple<int, int>> vecOfElements, std::tuple<int, int> square);

struct Piece{
    int code;//value ex:white pawn->0. Rmk: empty is also a piece
    bool en_passant = false; //can we take the pawn en_passant
    bool has_moved = false;//already moved
    std::vector<std::tuple<int, int>> legal_moves; //list of legal reachable squares
    std::vector <std::tuple<int, int>> control;//list of controled squares
};

Piece create_piece(int code);

//turn a pgn into a string
std::string pgn_to_string(std::string address);

//returns the header of a pgn ending position
int header_extractor(std::string str);

//tells which color won:0->black, 1->white, (-1)->draw
int winner_extractor(std::string game);

//extracts the move and returns it and the index of the next move
std::pair<std::string, int> move_extractor(std::string str, int index, bool turn);

//compute the total number of moves of the game
int game_length(std::string game);

//finds the indice of the end of the game (game is a list of games)
int find_end_game(std::string game, int start_index);

//display a move
void display_move(std::tuple<int, int, int, int, int> move);
#endif /* utilities_hpp */
