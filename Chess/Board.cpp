//
//  Board.cpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//

#include "Board.hpp"
#include <iostream>
#include <iomanip>
#include "utilities.hpp"
#include "zobra_hashing.hpp"
#include <vector>
#include <map>
#include <cmath>
#include "animation.hpp"
#include <random>
#include "text_rendering.hpp"
#include <filesystem>
#include <fstream>

/*correspondance table:
 For whites:
 pawn = 0, knight = 1, bishop = 2, rook = 3, queen = 4, king = 5
 
 For blacks:
 pawn = 6, knight = 7, bishop = 8, rook = 9, queen = 10, king = 11
 
 free square = -1
 */

#define WHITE_WINS 1
#define BLACK_WINS -1
#define DRAW 0
#define MAX_LINE 5000

extern int square_size;
extern int h_margin;
extern int w_margin;
//Current displayed texture
extern SDL_Texture* pTexture[8][8];

//The window renderer
extern SDL_Renderer* gRenderer;

//textures and messages for texts
extern std::string messages[5];
extern SDL_Texture* tTexture[5];

int sign[2] = {-1, 1}; //signs for each color: true-> 1 false ->-1 Useful for rewards computations

//initializing a new game
void Board::initData(bool new_game, bool turn, int pos[8][8]){
    std::vector <std::tuple<int, int>> _locations[12];//locations of pieces
    whose_turn = turn;
    //We set a new board
    if (new_game){
        for (int i=0; i<8; i++){
            position[1][i] = create_piece(0);
            position[6][i] = create_piece(6);
        }
            
        for (int i=2; i<6; i++){
            for (int j = 0; j<8; j++){
                position[i][j] = create_piece(-1);
            }
        }
        position[0][0] = create_piece(3);
        position[0][1] = create_piece(1);
        position[0][2] = create_piece(2);
        position[0][3] = create_piece(4);
        position[0][4] = create_piece(5);
        position[0][5] = create_piece(2);
        position[0][6] = create_piece(1);
        position[0][7] = create_piece(3);
        
        position[7][0] = create_piece(9);
        position[7][1] = create_piece(7);
        position[7][2] = create_piece(8);
        position[7][3] = create_piece(10);
        position[7][4] = create_piece(11);
        position[7][5] = create_piece(8);
        position[7][6] = create_piece(7);
        position[7][7] = create_piece(9);
        
    } else {
        for (int i = 0; i<8; i++){
            for (int j = 0; j<8; j++){
                position[i][j] = create_piece(pos[i][j]);
            }
        }
    }
    
    //computation of the controled squares and locations
    
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            position[i][j].control = controled_squares({i, j}, position[i][j].code);
            if (position[i][j].code != -1){
                _locations[position[i][j].code].push_back({i, j});
            }
        }
    }
    for (int i = 0; i<12; i++){
        locations[i] = _locations[i];
    }
    //clear the records
    record_moves.clear();
    record_positions.clear();
    record_appreciations.clear();
}

//computes the controled squares of a piece
std::vector <std::tuple<int, int>> Board::controled_squares(std::tuple<int, int> square, int code){
    std::vector <std::tuple<int, int>> control;
    if (code != -1){
        int line = get<0>(square);
        int col = get<1>(square);
        //white pawn
        if (code == 0){
            if ((line <7) and (col < 7)){
                control.push_back({line+1, col+1});
            }
            if ((line <7) and (col >0)){
                control.push_back({line+1, col-1});
            }
        }
        
        //white and black knights
        else if ((code == 1) or (code == 7)){
            if ((line + 2 < 8) and (col + 1 <8)){
                control.push_back({line+2, col+1});
            }
            if ((line + 2 < 8) and (col - 1 >-1)){
                control.push_back({line+2, col-1});
            }
            if ((line - 2 > -1) and (col + 1 <8)){
                control.push_back({line-2, col+1});
            }
            if ((line - 2 > -1) and (col - 1 >-1)){
                control.push_back({line-2, col-1});
            }
            if ((line + 1 < 8) and (col + 2 <8)){
                control.push_back({line+1, col+2});
            }
            if ((line + 1 < 8) and (col - 2 >-1)){
                control.push_back({line+1, col-2});
            }
            if ((line - 1 > -1) and (col + 2 <8)){
                control.push_back({line-1, col+2});
            }
            if ((line - 1 > -1) and (col - 2 >-1)){
                control.push_back({line-1, col-2});
            }
        }
            
            //white or black bishops
        else if ((code == 2) or (code == 8)){
            int amp = 1;
            while ((line+amp<8) and (col+amp<8) and (position[line+amp][col+amp].code == -1)){
                control.push_back({line+amp, col+amp});
                amp ++;
            }
            if ((line+amp<8) and (col+amp<8)){
                control.push_back({line+amp, col+amp});
            }
            
            amp = 1;
            while ((line+amp<8) and (col-amp>-1) and (position[line+amp][col-amp].code == -1)){
                control.push_back({line+amp, col-amp});
                amp ++;
            }
            if ((line+amp<8) and (col-amp>-1)){
                control.push_back({line+amp, col-amp});
            }
            
            amp = 1;
            while ((line-amp>-1) and (col+amp<8) and (position[line-amp][col+amp].code == -1)){
                control.push_back({line-amp, col+amp});
                amp ++;
            }
            if ((line-amp>-1) and (col+amp<8)){
                control.push_back({line-amp, col+amp});
            }
            
            amp = 1;
            while ((line-amp>-1) and (col-amp>-1) and (position[line-amp][col-amp].code == -1)){
                control.push_back({line-amp, col-amp});
                amp ++;
            }
            if ((line-amp>-1) and (col-amp>-1)){
                control.push_back({line-amp, col-amp});
            }
        }
        
        //white or black rooks
        else if ((code == 3) or (code == 9)){
            int amp = 1;
            while ((line+amp<8) and (position[line+amp][col].code == -1)){
                control.push_back({line+amp, col});
                amp ++;
            }
            if ((line+amp < 8)){
                control.push_back({line+amp, col});
            }
            
            amp = 1;
            while ((col-amp>-1) and (position[line][col-amp].code == -1)){
                control.push_back({line, col-amp});
                amp ++;
            }
            if ((col-amp>-1)){
                control.push_back({line, col-amp});
            }
            
            amp = 1;
            while ((col+amp<8) and (position[line][col+amp].code == -1)){
                control.push_back({line, col+amp});
                amp ++;
            }
            if ((col+amp < 8)){
                control.push_back({line, col+amp});
            }
            
            amp = 1;
            while ((line-amp>-1) and (position[line-amp][col].code == -1)){
                control.push_back({line-amp, col});
                amp ++;
            }
            if ((line-amp >-1)){
                control.push_back({line-amp, col});
            }
        }
        
        //white or black queen
        else if ((code == 4) or (code == 10)){
            int amp = 1;
            while ((line+amp<8) and (col+amp<8) and (position[line+amp][col+amp].code == -1)){
                control.push_back({line+amp, col+amp});
                amp ++;
            }
            
            if ((line+amp<8) and (col+amp<8)){
                control.push_back({line+amp, col+amp});
            }
            amp = 1;
            while ((line+amp<8) and (col-amp>-1) and (position[line+amp][col-amp].code == -1)){
                control.push_back({line+amp, col-amp});
                amp ++;
            }
            if ((line+amp<8) and (col-amp>-1)){
                control.push_back({line+amp, col-amp});
            }
            amp = 1;
            
            while ((line-amp>-1) and (col+amp<8) and (position[line-amp][col+amp].code == -1)){
                control.push_back({line-amp, col+amp});
                amp ++;
            }
            if ((line-amp>-1) and (col+amp<8)){
                control.push_back({line-amp, col+amp});
            }
            amp = 1;
            while ((line-amp>-1) and (col-amp>-1) and (position[line-amp][col-amp].code == -1)){
                control.push_back({line-amp, col-amp});
                amp ++;
            }
            if ((line-amp>-1) and (col-amp>-1)){
                control.push_back({line-amp, col-amp});
            }
            amp = 1;
            while ((line+amp<8) and (position[line+amp][col].code == -1)){
                control.push_back({line+amp, col});
                amp ++;
            }
            if ((line+amp<8)){
                control.push_back({line+amp, col});
            }
            amp = 1;
            while ((line-amp>-1) and (position[line-amp][col].code == -1)){
                control.push_back({line-amp, col});
                amp ++;
            }
            if ((line-amp>-1)){
                control.push_back({line-amp, col});
            }
            amp = 1;
            while ((col+amp<8) and (position[line][col+amp].code == -1)){
                control.push_back({line, col+amp});
                amp ++;
            }
            if ((col+amp<8)){
                control.push_back({line, col+amp});
            }
            amp = 1;
            while ((col-amp>-1) and (position[line][col-amp].code == -1)){
                control.push_back({line, col-amp});
                amp ++;
            }
            if ((col-amp>-1)){
                control.push_back({line, col-amp});
            }
        }
        
        
        //white or black king
        else if ((code == 5) or (code ==11)){
            if((line+1 <8)and(col+1<8)){
                control.push_back({line+1, col+1});
            }
            if((col+1<8)){
                control.push_back({line, col+1});
            }
            if((line-1 >-1)and(col+1<8)){
                control.push_back({line-1, col+1});
            }
            if((line-1 >-1)){
                control.push_back({line-1, col});
            }
            if((line-1 >-1)and(col-1>-1)){
                control.push_back({line-1, col-1});
            }
            if((col-1>-1)){
                control.push_back({line, col-1});
            }
            if((line+1 <8)and(col-1>-1)){
                control.push_back({line+1, col-1});
            }
            if((line+1 <8)){
                control.push_back({line+1, col});
            }
        }
        
        
        //black pawn
        if (code == 6){
            if ((line >0) and (col > 0)){
                control.push_back({line-1, col-1});
            }
            if ((line >0) and (col <7)){
                control.push_back({line-1, col+1});
            }
        }
    }
    return control;
}

void Board::display_board(){
    for (int i = 0; i<8; i++){
        for (int j =0; j<8; j++){
            std::cout << std::fixed << std::setprecision(2);
            std::cout << std::setw(2) << position[7-i][j].code << " ";
        }
        std::cout << "\n";
    }
}

void Board::display_vector(std::vector <std::tuple<int, int>> vector){
    for (int i = 0; i < vector.size(); i++){
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(2) << get<0>(vector[i]) << "," << get<1>(vector[i]) << " ";
    }
    std::cout << "\n";
}

//find the line of a piece from its code, its target square and possibly its column
int Board::find_line(int code, bool capture, int target_line, int target_column, bool known_column, int column){
    int var_line;//variable which will holds lines
    int var_column;//variable which will holds columns
    std::pair<bool, int> pair;//variable which holds info
    
    if ((code == 0) and capture){//a white pawn captures
        return target_line - 1;
    }
    else if (code == 0){//a white pawn moves
        if (position[target_line - 1][target_column].code == 0){
            return target_line - 1;
        }
        else if (position[target_line - 2][target_column].code == 0){
            return target_line - 2;
        }
        else{
            std::cout << "I don't know where is the white pawn dude \n";
            return -1;
        }
    }
    
    if ((code == 6) and capture){//a black pawn captures
        return target_line + 1;
    }
    else if (code == 6){//a black pawn moves
        if (position[target_line + 1][target_column].code == 6){
            return target_line + 1;
        }
        else if (position[target_line + 2][target_column].code == 6){
            return target_line + 2;
        }
        else{
            std::cout << "I don't know where is the black pawn dude \n";
            return -1;
        }
    }
    
    else {//another piece moves
        for (int i = 0; i < locations[code].size(); i++){
            var_line = get<0>(locations[code][i]);
            var_column = get<1>(locations[code][i]);
            if (known_column and (var_column == column)){
                pair = findInVector(position[var_line][var_column].control,{target_line, target_column});
                if (pair.first and is_legal(locations[code][i], {target_line, target_column})){//we have to check if the move is legal before
                    return var_line;
                }
            }
            else if (known_column and (var_column != column)){
            }
            else {
                pair = findInVector(position[var_line][var_column].control,{target_line, target_column});
                /*std::cout << var_line << " " << var_column << " " << pair.first << "\n";
                display_vector(position[var_line][var_column].control);*/
                if (pair.first and is_legal(locations[code][i], {target_line, target_column})){
                    return var_line;
                }
            }
        }
        std::cout << "I don't know where is the piece (" << code << "), dude \n";
        return -1;
    }
}

//find the column of a piece from its code, its target square and possibly its column
int Board::find_column(int code, bool capture, int target_line, int target_column, bool known_line, int line){
    int var_line;//variable which will holds lines
    int var_column;//variable which will holds columns
    std::pair<bool, int> pair;//variable which holds info
    
    if (((code == 0) or (code == 6)) and capture){//a pawn captures
        std::cout << "You should normally know the column \n";
        return -1;
    }
    else if ((code == 0) or (code == 6)){//a pawn moves
        return target_column;
    }
        
    else {//another piece moves
        for (int i = 0; i < locations[code].size(); i++){
            var_line = get<0>(locations[code][i]);
            var_column = get<1>(locations[code][i]);
            if (known_line and (var_line == line)){
                pair = findInVector(position[var_line][var_column].control,{target_line, target_column});
                if (pair.first and is_legal(locations[code][i], {target_line, target_column})){//we need to check if the move is legal
                    return var_column;
                }
            }
            else if (known_line and (var_line != line)){
            }
            else {
                pair = findInVector(position[var_line][var_column].control,{target_line, target_column});
                if (pair.first and is_legal(locations[code][i], {target_line, target_column})){
                    return var_column;
                }
            }
        }
        std::cout << "I don't know where is the piece (" << code << "), dude \n";
        return -1;
    }
}

//executes a move taken in pgn notation and returns the detals of the move: pair{move_output, action} where action is {start_square, target_square}
std::pair<int, std::tuple<int, int, int, int, int>>  Board::interpret_pgn(std::string move){
    std::pair<int, std::tuple<int, int, int, int, int>> result;
    //dealing with the final score
    if (move.substr(0, 3) == "1/2"){
        result.first = DRAW;
        return result;
    }
    else if (move.substr(0, 3) == "1-0"){
        result.first = WHITE_WINS;
        return result;
    }
    else if (move.substr(0, 3) == "0-1"){
        result.first = BLACK_WINS;
        return result;
    }
    
    //first step in the interpretation of the move
    if (move[0] == 'O'){//castling
        int i;
        if (whose_turn){
            i = 0;
        }
        else{
            i = 7;
        }
        if (move[3] == '-'){//long_castle
            execute_move({i, 4}, {i, 2}, -1);
            result.second = {i, 4, i, 2, -1};
        }
        else {//short_castle
            execute_move({i, 4}, {i, 6}, -1);
            result.second = {i, 4, i, 6, -1};
        }
        result.first = -2;
        return result;
    }
    
    int depth; //depth of the exploration of the move
    int code = -1; //code of the piece involved
    int promote_code = -1;//code of the promoted piece
    bool known_column = false; //do we know the column of the piece involved
    bool known_line = false;//do we know the line of the piece involved
    bool capture = false;//is there capture
    int start_column = -1; //column of the piece involved
    int start_line = -1;//line of the piece involved
    int final_column = -1;//target square column
    int final_line = -1;//target square line
    
    
    if ((move[0] == 'K') or (move[0] == 'Q') or (move[0] == 'B') or (move[0] == 'N') or (move[0] == 'R')) {//the move involves a piece
        depth = 1; //we make a step deeper in the move interpretation
        code = letter_to_int(move[0], whose_turn);
    }
    else {//the move involves a pawn
        depth = 0; //we make a step deeper in the move interpretation
        
        if (whose_turn){
            code = 0;
        }
        else{
            code = 6;
        }
    }
    
    //second step in the interpretation of the move
    if (from_a_to_h(move[depth]) and ((move[depth+1] == 'x') or from_a_to_h(move[depth+1]))){//the piece or the pawn involved is in a known column
        known_column = true;
        start_column = (int) move[depth] - 97;
        depth ++;
    }
    else if (from_1_to_8(move[depth]) and ((move[depth+1] == 'x') or from_a_to_h(move[depth+1]))){//the piece or the pawn involved is in a known line
        known_line = true;
        start_line = (int) move[depth] - 49;
        depth ++;
    }
    else if (from_a_to_h(move[depth]) and from_1_to_8(move[depth+1]) and ((move[depth+2] == 'x') or from_a_to_h(move[depth+2]))){//the piece or the pawn involved is in a known square
        known_column = true;
        known_line = true;
        start_column = (int) move[depth] - 97;
        start_line = (int) move[depth+1] - 49;
        depth += 2;
    }
    
    //third step in the interpretation of the move
    if (move[depth] == 'x'){//there is a capture
        capture = true;
        depth ++;
    }
    
    //fourth step in the interpretation of the move
    if (from_a_to_h(move[depth]) and from_1_to_8(move[depth+1])){//coordinate of the move
        final_column = (int) move[depth] - 97;
        final_line = (int) move[depth+1] - 49;
        depth += 2;
    }
    
    //fifth step in the interpretation of the move
    if (move[depth] == '='){//pawn promotion
        promote_code = letter_to_int(move[depth+1], whose_turn);
    }
    
    //now we execute the move
    if (not known_line){
        known_line = true;
        start_line = find_line(code, capture, final_line, final_column, known_column, start_column);
    }
    if (not known_column){
        start_column = find_column(code, capture, final_line, final_column, known_line, start_line);
    }
    execute_move({start_line, start_column}, {final_line, final_column}, promote_code);
    
    result.first = -2;
    result.second = {start_line, start_column, final_line, final_column, promote_code};
    return result;
}

void Board::castle(bool short_castle){
    int index;
    std::pair <bool, int> pair;
    
    if (whose_turn){//for white
        index = 0;
    }
    else{//for black
        index = 7;
    }
        
    if (short_castle){//short castling
        position[index][6] = position[index][4];
        position[index][5] = position[index][7];
        position[index][4] = create_piece(-1);
        position[index][7] = create_piece(-1);
        
        //updating textures
        pTexture[index][6] = pTexture[index][4];
        pTexture[index][5] = pTexture[index][7];
        
        //recomputing the controled squares
        for (int i = 0; i<8; i++){
            for (int j = 0; j<8; j++){
                position[i][j].control = controled_squares({i, j}, position[i][j].code);
            }
        }
        //updating the locations
        pair = findInVector(locations[position[index][6].code], {index, 4});
        locations[position[index][6].code].erase(locations[position[index][6].code].begin() + pair.second);
        locations[position[index][6].code].push_back({index, 6});
        
        pair = findInVector(locations[position[index][5].code], {index, 7});
        locations[position[index][5].code].erase(locations[position[index][5].code].begin() + pair.second);
        locations[position[index][5].code].push_back({index, 5});
    }
    
    else{//long castling
        position[index][2] = position[index][4];
        position[index][3] = position[index][0];
        position[index][4] = create_piece(-1);
        position[index][0] = create_piece(-1);
        
        //updating textures
        pTexture[index][2] = pTexture[index][4];
        pTexture[index][3] = pTexture[index][0];
        
        //recomputing the controled squares
        for (int i = 0; i<8; i++){
            for (int j = 0; j<8; j++){
                position[i][j].control = controled_squares({i, j}, position[i][j].code);
            }
        }
        //updating the locations
        pair = findInVector(locations[position[index][2].code], {index, 4});
        locations[position[index][2].code].erase(locations[position[index][2].code].begin() + pair.second);
        locations[position[index][2].code].push_back({index, 2});
        
        pair = findInVector(locations[position[index][3].code], {index, 0});
        locations[position[index][3].code].erase(locations[position[index][3].code].begin() + pair.second);
        locations[position[index][3].code].push_back({index, 3});
    }
    whose_turn = not whose_turn;
}

void Board::execute_move(std::tuple<int, int> start_square, std::tuple<int, int> target_square, int promote_code){
    int start_line = get<0>(start_square);
    int start_column = get<1>(start_square);
    int final_line = get<0>(target_square);
    int final_column = get<1>(target_square);
    std::pair <bool, int> pair;
    bool appreciation = false; //the move was pawn push or capture ?
    
    //recording
    record_moves.push_back({start_line, start_column, final_line, final_column, promote_code});
    record_positions.push_back(computeHash(position));
    
    //figuring out whether there is castle
    if (((position[start_line][start_column].code == 5) or (position[start_line][start_column].code == 11)) and (abs(start_column - final_column) == 2)){
        position[start_line][start_column].has_moved = true;
        if (start_column < final_column){//short castle
            castle(true);
        }
        else {
            castle(false);
        }
    }
    
    else{
        //figuring_out if there is en_passant to update
        if ((position[start_line][start_column].code == 0) or (position[start_line][start_column].code == 6)){
            appreciation = true;
            if (abs(final_line - start_line) == 2){
                if ((final_column+1< 8) and (position[final_line][final_column+1].code == whose_turn*6)){
                    position[start_line][start_column].en_passant = true;
                }
                if ((final_column-1> -1) and (position[final_line][final_column-1].code == 6*whose_turn)){
                    position[start_line][start_column].en_passant = true;
                }
            }
        }
        
        if (position[start_line][start_column].code == 6){
            if (abs(final_line - start_line) == 2){
                if ((final_column+1< 8) and (position[final_line][final_column+1].code == 6)){
                    position[start_line][start_column].en_passant = true;
                }
                if ((final_column-1> -1) and (position[final_line][final_column-1].code == 6)){
                    position[start_line][start_column].en_passant = true;
                }
            }
        }
        
        bool capture = false;
        //figuring out whether there is a capture
        if ((position[start_line][start_column].code == 0) or (position[start_line][start_column].code == 6)){
            if (start_column != final_column){
                capture = true;
                appreciation = true;
            }
        }
        else {
            if (position[final_line][final_column].code != -1){
                capture = true;
                appreciation = true;
            }
        }
        
        if (capture){
            if (((position[start_line][start_column].code == 0) or (position[start_line][start_column].code == 6)) and (position[final_line][final_column].code == -1)){ //dealing with en_passant first
                
                //updating the locations of captured pawn
                pair = findInVector(locations[position[start_line][final_column].code], {start_line, final_column});
                locations[position[start_line][final_column].code].erase(locations[position[start_line][final_column].code].begin() + pair.second);
                
                //removing the pawn from the board
                position[start_line][final_column] = create_piece(-1);
            }
            else{
                //updating the locations of captured piece
                pair = findInVector(locations[position[final_line][final_column].code], {final_line, final_column});
                locations[position[final_line][final_column].code].erase(locations[position[final_line][final_column].code].begin() + pair.second);
                
            }
        }
        
        //common case for every one
        if (promote_code > -1){//there is a pawn promotion
            position[final_line][final_column] = create_piece(promote_code);
            position[final_line][final_column].has_moved = true;
            init_board(position);//will create the new piece
            
            //updating the locations of the promoted pawn
            pair = findInVector(locations[position[start_line][start_column].code], {start_line, start_column});
            locations[position[start_line][start_column].code].erase(locations[position[start_line][start_column].code].begin() + pair.second);
            locations[promote_code].push_back({final_line, final_column});
        }
        else{
            position[final_line][final_column] = position[start_line][start_column];
            position[final_line][final_column].has_moved = true;
            //updating textures
            pTexture[final_line][final_column] = pTexture[start_line][start_column];
            
            //updating the locations
            pair = findInVector(locations[position[final_line][final_column].code], {start_line, start_column});
            //display_vector(locations[position[final_line][final_column].code]);
            locations[position[final_line][final_column].code].erase(locations[position[final_line][final_column].code].begin() + pair.second);
            locations[position[final_line][final_column].code].push_back({final_line, final_column});
            //display_vector(locations[position[final_line][final_column].code]);
        }
        
        position[start_line][start_column] = create_piece(-1);
        
        //recomputing the controled squares
        for (int i = 0; i<8; i++){
            for (int j = 0; j<8; j++){
                position[i][j].control = controled_squares({i, j}, position[i][j].code);
            }
        }
        
        //updating previous_move en passant
        for (int i=0; i<locations[whose_turn*6].size(); i++){
            position[get<0>(locations[whose_turn*6][i])][get<1>(locations[whose_turn*6][i])].en_passant = false;
        }
        
        //recording appreciation
        record_appreciations.push_back(appreciation);
        whose_turn = not whose_turn;
    }
}

void Board::train_from_pgn(std::string game, bool watch){
    
    std::pair<int, std::tuple<int, int, int, int, int>> result; //code result of the move
    result.first = -2;
    int index = header_extractor(game); //index for reading the game
    std::pair<std::string, int> pair;
    float gamma = .9;
    int n_th_move = 1;
    int number_moves = game_length(game);
    unsigned long long int position_code;
    
    int reward = winner_extractor(game);//if the color won->+1, if draw ->0, if loss ->-1
    int turn_index = 0;//counting the number of turns
    
    //starting the training loop
    SDL_Event e;
    bool quit = false;
    
    //no need to learn from drawn games
    if (reward == 0){
        quit = true;
    }
    
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_LEFT:
                        step_back();
                        break;
                        
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                //Get mouse position
                int x, y;
                SDL_GetMouseState( &x, &y );
                int button = locate_button(x, y);
                if (button < 4){
                    handle_button(button);
                    quit = true;
                    break;
                }
            }
        }
        if (watch){
            SDL_Delay(2000);
        }
        
        position_code = computeHash(position);
        pair = move_extractor(game, index, whose_turn);
        result = interpret_pgn(pair.first);
        index = pair.second;
        
        if (watch){
            update_board(position);
        }
        
        if (result.first > -2){
            quit = true;
            break;
        }
        
        //updating the values of the player who made last move
        count[1-whose_turn][position_code][result.second] += 1;
        Q[1-whose_turn][position_code][result.second] += (sign[1-whose_turn] * reward*pow(gamma,number_moves-n_th_move) - Q[1-whose_turn][position_code][result.second]) / count[1-whose_turn][position_code][result.second];
        
        n_th_move += turn_index % 2;
        turn_index ++;
    }
}

void Board::train_from_record(int reward){
    if (reward != 0){//no need to learn from drawn game
        std::pair<std::string, int> pair;
        float gamma = .9;
        int n_th_move = 1;
        int number_moves = ceil(static_cast<double>(record_moves.size())/2);
        int turn_index = 0;//current index in the record_moves vector
        
        //starting the training loop
        while(turn_index < record_moves.size()){
            count[1 - (turn_index % 2)][record_positions[turn_index]][record_moves[turn_index]] ++;
            Q[1 - (turn_index % 2)][record_positions[turn_index]][record_moves[turn_index]] += (sign[1 - (turn_index % 2)]*reward*pow(gamma, number_moves-n_th_move) - Q[1 - (turn_index % 2)][record_positions[turn_index]][record_moves[turn_index]]) / count[1 - (turn_index % 2)][record_positions[turn_index]][record_moves[turn_index]];
            
            n_th_move += (turn_index %2);
            turn_index ++;
        }
    }
}

void Board::play_vs_AI(bool player, int proba){
    std::tuple<int, int> square;
    std::tuple<int, int> start_square;
    std::tuple<int, int> final_square;
    std::tuple<int, int, int, int, int> proposal; //AI move
    std::vector<std::tuple<int, int>> legal_moves;
    std::pair<bool, int> pair;
    int line;
    int col;
    
    bool know_first_square = false;
    int x, y;
    int promote_code = -1;
    
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_LEFT:
                        step_back();
                        break;
                        
                }
            }
            //If mouse event happened
            if(e.type == SDL_MOUSEBUTTONDOWN){
                //Get mouse position
                SDL_GetMouseState( &x, &y );
                int button = locate_button(x, y);
                if (button < 4){
                    handle_button(button);
                    quit= true;
                    break;
                }
                
                else if ((x>w_margin) and (x < w_margin+8*square_size) and (y > h_margin) and (y < h_margin + 8*square_size)){
                    if (know_first_square){
                        square = locate_square(x, y);
                        line = get<0>(start_square);
                        col = get<1>(start_square);
                        pair = findInVector(legal_moves, square);
                        know_first_square = false;
                        
                        if (pair.first){
                            final_square = square;
                            if (((get<0>(final_square) == 0) or (get<0>(final_square) == 7)) and ((position[line][col].code == 0) or (position[line][col].code == 6))){//dealing with promotion
                                //std::cout << "Which piece do you want to promote to ? \n";
                                //std::cin >> promote_code;
                                promote_code = 4 + (1-player)*6;
                            }
                            messages[4] = " ";
                            load_text(messages[4], 4);
                            
                            execute_move(start_square, final_square, promote_code);
                            update_board(position);
                            promote_code = -1;//reset the variable
                            
                            SDL_Delay(100);
                            pair = game_over();
                            if (pair.first){
                                quit = true;
                                break;
                            }
                        }
                        else{
                            std::cout << "Move (following) is not permitted ";
                            display_move({line, col, get<0>(square), get<1>(square), -1});
                            display_vector(legal_moves);
                            messages[4] = "Illegal move !";
                            load_text(messages[4], 4);
                            update_board(position);
                        }
                    }
                    else {
                        square = locate_square(x, y);
                        if (not are_nemesis(position[get<0>(square)][get<1>(square)].code, 1 + (1-whose_turn)*6)){//is the piece the same color than player?
                            start_square = square;
                            know_first_square = true;
                            legal_moves = compute_legal_moves(square);
                            highlight_moves(legal_moves, position);
                            
                        }
                    }
                }
            }
        }
        if (not quit and (whose_turn == not player)){//if it is Ai's turn
            proposal = propose_move(proba, not player);
            start_square = {get<0>(proposal), get<1>(proposal)};
            final_square = {get<2>(proposal), get<3>(proposal)};
            execute_move(start_square, final_square, get<4>(proposal));
            update_board(position);
        
            pair = game_over();
            if (pair.first){
                quit = true;
                break;
            }
        }
    }
}

void Board::AI_vs_AI_SARSA(int proba){
    std::tuple<int, int> start_square;
    std::tuple<int, int> final_square;
    std::tuple<int, int, int, int, int> proposal; //AI move
    //std::vector<std::tuple<int, int>> legal_moves;
    std::pair<bool, int> pair;
    int reward = 0;
    int index = 0;
    
    float alpha = 1;
    float gamma = .9;
    
    //first moves
    //white move
    proposal = propose_move(proba, true);
    start_square = {get<0>(proposal), get<1>(proposal)};
    final_square = {get<2>(proposal), get<3>(proposal)};
    execute_move(start_square, final_square, get<4>(proposal));
    //update_board(position);
    
    pair = game_over();
    reward = pair.second;
    index = record_positions.size()-1;
    //updating values (TD)
    count[true][record_positions[index]][proposal] += 1;
    
    //SDL_Delay(100);
    
    //black move
    proposal = propose_move(proba, false);
    start_square = {get<0>(proposal), get<1>(proposal)};
    final_square = {get<2>(proposal), get<3>(proposal)};
    execute_move(start_square, final_square, get<4>(proposal));
    update_board(position);
    pair = game_over();
    reward = pair.second;
    index = record_positions.size()-1;
    //updating values (TD)
    count[false][record_positions[index]][proposal] += 1;
    
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_LEFT:
                        step_back();
                        break;
                }
            }
            //If mouse event happened
            if(e.type == SDL_MOUSEBUTTONDOWN){
                //Get mouse position
                int x, y;
                SDL_GetMouseState( &x, &y );
                int button = locate_button(x, y);
                if (button < 4){
                    handle_button(button);
                    quit= true;
                    break;
                }
            }
            
        }
        //white move
        if (not quit and whose_turn){
            proposal = propose_move(proba, true);
            start_square = {get<0>(proposal), get<1>(proposal)};
            final_square = {get<2>(proposal), get<3>(proposal)};
            execute_move(start_square, final_square, get<4>(proposal));
            //update_board(position);
        
            pair = game_over();
            //reward = pair.second;
            index = record_positions.size()-1;
            //updating values (TD)
            count[true][record_positions[index]][proposal] += 1;
            Q[true][record_positions[index-2]][record_moves[index-2]] += alpha*(sign[true]*reward + gamma*Q[true][record_positions[index]][proposal] - Q[true][record_positions[index-2]][record_moves[index-2]]);
            
            if (Q[true][record_positions[index-2]][record_moves[index-2]] > 0){
                std::cout << "white -> " << Q[true][record_positions[index-2]][record_moves[index-2]] << "\n";
            }
            
            if (pair.first){
                quit = true;
                break;
            }
            
            //SDL_Delay(500);
        }
        
        //black move
        if (not quit and not whose_turn){
            proposal = propose_move(proba, false);
            start_square = {get<0>(proposal), get<1>(proposal)};
            final_square = {get<2>(proposal), get<3>(proposal)};
            execute_move(start_square, final_square, get<4>(proposal));
            //update_board(position);
        
            pair = game_over();
            //reward = pair.second;
            index = record_positions.size()-1;
            //updating values (TD)
            count[false][record_positions[index]][proposal] += 1;
            
            Q[false][record_positions[index-2]][record_moves[index-2]] += alpha*(sign[false]*reward + gamma*Q[false][record_positions[index]][proposal] - Q[false][record_positions[index-2]][record_moves[index-2]]);
            
            if (Q[false][record_positions[index-2]][record_moves[index-2]] > 0){
                std::cout << "black ->" << Q[false][record_positions[index-2]][record_moves[index-2]] << "\n";
            }
            
            if (pair.first){
                quit = true;
            }
            
            //SDL_Delay(500);
        }
    }
}

void Board::AI_vs_AI_MC(int proba){
    std::tuple<int, int> start_square;
    std::tuple<int, int> final_square;
    std::tuple<int, int, int, int, int> proposal; //AI move
    std::pair<bool, int> pair;
    int index = 0;
    
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_LEFT:
                        step_back();
                        break;
                }
            }
            //If mouse event happened
            if(e.type == SDL_MOUSEBUTTONDOWN){
                //Get mouse position
                int x, y;
                SDL_GetMouseState( &x, &y );
                int button = locate_button(x, y);
                if (button < 4){
                    handle_button(button);
                    quit= true;
                    break;
                }
            }
        }
        //white move
        if (not quit and whose_turn){
            proposal = propose_move(proba, true);
            start_square = {get<0>(proposal), get<1>(proposal)};
            final_square = {get<2>(proposal), get<3>(proposal)};
            execute_move(start_square, final_square, get<4>(proposal));
            //update_board(position);
        
            pair = game_over();
            index = record_positions.size()-1;
    
            if (pair.first){
                quit = true;
                break;
            }
            //SDL_Delay(500);
        }
        
        //black move
        if (not quit and not whose_turn){
            proposal = propose_move(proba, false);
            start_square = {get<0>(proposal), get<1>(proposal)};
            final_square = {get<2>(proposal), get<3>(proposal)};
            execute_move(start_square, final_square, get<4>(proposal));
            //update_board(position);
        
            pair = game_over();
            index = record_positions.size()-1;
            
            if (pair.first){
                quit = true;
            }
            //SDL_Delay(500);
        }
    }
    
    //learning from the game
    train_from_record(pair.second);
}

std::tuple<int, int, int, int, int> Board::propose_move(int proba, bool color){
    unsigned long long int position_code = computeHash(position);
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    
    if (Q[color].count(position_code)){
        std::uniform_int_distribution<> distrib(0, 100);
        
        if (distrib(gen) <= proba){//propose the best move so far}
            auto pr = std::max_element(Q[color][position_code].begin(), Q[color][position_code].end(), [](std::pair<std::tuple<int, int, int, int, int>, float> const &x, std::pair<std::tuple<int, int, int, int, int>, float> const &y){return x.second < y.second;});
            
            std::cout << pr->second << " " << count[color][position_code][pr->first] << ": ";
            display_move(pr->first);
            
            return pr->first;
        }
    }
    
    //explore in other cases
    std::vector<std::tuple<int, int, int, int, int>> proposals; //AI moves
    std::vector<std::tuple<int, int>> legal_moves;
    //spanning all the legal moves
    for (int i = (1-whose_turn)*6; i < (2-whose_turn)*6; i++){
        for (int j = 0; j < locations[i].size(); j++){
            //std::cout << "key = " << get<0>(locations[i][j]) << " " << get<1>(locations[i][j]) << "\n";
            legal_moves = compute_legal_moves(locations[i][j]);
            for (int k = 0; k < legal_moves.size(); k++){
                if ((position[get<0>(locations[i][j])][get<1>(locations[i][j])].code == (1-whose_turn)*6) and (get<0>(legal_moves[k]) == 7*whose_turn)){//dealing with promotion
                    proposals.push_back({get<0>(locations[i][j]), get<1>(locations[i][j]), get<0>(legal_moves[k]), get<1>(legal_moves[k]), 4+(1-whose_turn)*6});
                }
                else{
                    proposals.push_back({get<0>(locations[i][j]), get<1>(locations[i][j]), get<0>(legal_moves[k]), get<1>(legal_moves[k]), -1});

                }
                //display_move(proposals[proposals.size()-1]);
            }
        }
    }

    //std::cout << "---------------\n";
    int n = proposals.size();
    
    //chosing the move
    std::uniform_int_distribution<> distrib(0, n-1);
    
    return proposals[distrib(gen)];
}


std::vector<std::tuple<int, int>> Board::compute_possible_moves(std::tuple<int, int> square){
    std::vector<std::tuple<int, int>> possible_moves;
    int line = get<0>(square);
    int col = get<1>(square);
    int code = position[line][col].code;
    int var_line;
    int var_col;
    
    if (code == 0){//white pawn
        if (position[line+1][col].code == -1){//1 square move
            possible_moves.push_back({line+1, col});
            
            if ((line == 1) and (position[line+2][col].code == -1)){//2 square move
                possible_moves.push_back({line+2, col});
            }
        }
        if ((col + 1 < 8) and (are_nemesis(code, position[line+1][col+1].code))and (position[line+1][col+1].code != -1)){//capture
            possible_moves.push_back({line+1, col+1});
        }
        if ((col - 1 > -1) and (are_nemesis(code, position[line+1][col-1].code))and (position[line+1][col-1].code != -1)){//capture
            possible_moves.push_back({line+1, col-1});
        }
        
        if ((col + 1 < 8) and (are_nemesis(code, position[line][col+1].code)) and position[line][col+1].en_passant){//en passant
            possible_moves.push_back({line+1, col+1});
        }
        if ((col - 1 > -1) and (are_nemesis(code, position[line][col-1].code)) and position[line][col-1].en_passant){//en passant
            possible_moves.push_back({line+1, col-1});
        }
    }
    
    else if (code == 6){//black pawn
        if (position[line-1][col].code == -1){//1 square move
            possible_moves.push_back({line-1, col});
            
            if ((line == 6) and (position[line-2][col].code == -1)){//2 square move
                possible_moves.push_back({line-2, col});
            }
        }
        if ((col + 1 < 8) and (are_nemesis(code, position[line-1][col+1].code)) and (position[line-1][col+1].code != -1)){//capture
            possible_moves.push_back({line-1, col+1});
        }
        if ((col - 1 > -1) and (are_nemesis(code, position[line-1][col-1].code)) and (position[line-1][col-1].code != -1)){//capture
            possible_moves.push_back({line-1, col-1});
        }
        
        if ((col + 1 < 8) and (are_nemesis(code, position[line][col+1].code)) and (position[line][col+1].en_passant)){//en passant
            possible_moves.push_back({line-1, col+1});
        }
        if ((col - 1 > -1) and (are_nemesis(code, position[line][col-1].code)) and position[line][col-1].en_passant){//en passant
            possible_moves.push_back({line-1, col-1});
        }
    }
    
    else if ((code == 5) or (code == 11)){//kings
        //display_vector(position[line][col].control);
        for (int i=0; i < position[line][col].control.size(); i++){
            var_line = get<0>(position[line][col].control[i]);
            var_col = get<1>(position[line][col].control[i]);
            if ((not is_controled(position[line][col].control[i])) and are_nemesis(code, position[var_line][var_col].code)){
                //std::cout << "----" << var_line << " " << var_col << "\n";
                possible_moves.push_back(position[line][col].control[i]);
            }
        }
        
        if ((not position[line][col].has_moved) and (position[line][5].code == -1) and (position[line][6].code == -1) and (not are_nemesis(code, position[line][7].code)) and (not position[line][7].has_moved) and (position[line][7].code==3 or position[line][7].code==9)){//short castle
            if ((not is_controled({line, 5})) and (not is_controled({line, 6}))){
                possible_moves.push_back({line, 6});
            }
        }
        
        if ((not position[line][col].has_moved) and (position[line][3].code == -1) and (position[line][2].code == -1) and (not are_nemesis(code, position[line][0].code)) and (not position[line][0].has_moved) and (position[line][0].code==3 or position[line][0].code==9)){//long castle
            if ((not is_controled({line, 3})) and (not is_controled({line, 2}))){
                possible_moves.push_back({line, 2});
            }
        }
    }
    else {
        for (int i=0; i< position[line][col].control.size(); i++){
            var_line = get<0>(position[line][col].control[i]);
            var_col = get<1>(position[line][col].control[i]);
            if (are_nemesis(code, position[var_line][var_col].code)){
                possible_moves.push_back(position[line][col].control[i]);
            }
        }
    }
    return possible_moves;
}


bool Board::is_controled(std::tuple<int, int> square){
    std::pair<bool, int> pair;
    bool result = false;
    int var_line;
    int var_col;
    int i = 0;//starting index of the involved pieces codes
    if (whose_turn){
        i = 6;
    }
    
    int king_line = get<0>(locations[5+(1-whose_turn)*6][0]);
    int king_col = get<1>(locations[5+(1-whose_turn)*6][0]);
    
    Piece piece = position[king_line][king_col];
    //we remove the king first
    position[king_line][king_col] = create_piece(-1);
    
    //computing the new controled squares
    for (int j = 0; j<8; j++){
        for (int k = 0; k<8; k++){
            position[j][k].control = controled_squares({j, k}, position[j][k].code);
        }
    }
    
    //test if the square is controled
    for (int j = i; j<i+6; j++){
        for (int k = 0; k < locations[j].size(); k++){
            var_line = get<0>(locations[j][k]);
            var_col = get<1>(locations[j][k]);
            pair = findInVector(position[var_line][var_col].control, square);
            if (pair.first){
                result = true;
                break;
            }
        }
    }
    
    //we put back the piece
    position[king_line][king_col] = piece;
    
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            position[i][j].control = controled_squares({i, j}, position[i][j].code);
        }
    }
    
    return result;
}

std::pair<bool, std::tuple<int, int>> Board::is_pinned(std::tuple<int, int> square, int numb_checks){
    std::pair<bool, std::tuple<int, int>> result;
    result.first = false;
    int king_code = 11;
    if (whose_turn){
        king_code = 5;
    }
    
    int line = get<0>(square);
    int col = get<1>(square);
    
    Piece piece = position[line][col];
    //we remove the piece on the square
    position[line][col] = create_piece(-1);
    
    //computing the new controled squares
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            position[i][j].control = controled_squares({i, j}, position[i][j].code);
        }
    }
    
    //compute the new number of checks
    std::vector<std::tuple<int, int>> checks = count_checks();
    int m = checks.size();
    
    if (m > numb_checks){
        result.first = true;
        //finding the piece pinning
        result.second = checks[0];
    }
    
    //we put back the piece
    position[line][col] = piece;
    
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            position[i][j].control = controled_squares({i, j}, position[i][j].code);
        }
    }
    
    return result;
}

//checks if a possible move is legal
bool Board::is_legal(std::tuple<int, int> start_square, std::tuple<int, int> final_square){
    
    int var_line;
    int var_col;
    std::vector<std::tuple<int, int>> checks = count_checks(); //all the locations of the checking pieces
    
    int i = 0;//starting index of the involved pieces codes
    int king_code = 11;
    if (whose_turn){
        i = 6;
        king_code = 5;
    }
    
    std::tuple<int, int> king_square = locations[king_code][0];
    
    int start_line = get<0>(start_square);
    int start_col = get<1>(start_square);
    
    int final_line = get<0>(final_square);
    int final_col = get<1>(final_square);
    
    int king_line = get<0>(king_square);
    int king_col = get<1>(king_square);
    
    //dealing with checks
    if (checks.size() > 0){
        //forbidding castling
        if ((abs(get<1>(final_square) - get<1>(start_square)) == 2) and (position[start_line][start_col].code == king_code)){//the king can not castle
            return false;
        }
        
        //The king can always move to a not controled squared
        if (position[start_line][start_col].code == king_code){//the king can move
            return true;
        }
        
        //dealing with double checks
        if (checks.size() == 2){
            return false;
        }
        
        //dealing with single checks
        else if (checks.size() == 1){
            if (is_pinned(start_square, 1).first){//pinned piece cannot move
                return false;
            }

            var_line = get<0>(checks[0]);
            var_col = get<1>(checks[0]);
            
            if (final_square == checks[0]){//an unpinned piece can capture the checking piece
                return true;
            }
            
            //dealing with pawn and knight checks
            if ((position[var_line][var_col].code == i) or (position[var_line][var_col].code == 1+i)){
                return false;
            }
            
            //dealing with bishop checks
            if (position[var_line][var_col].code == 2+i){
                if ((static_cast<double>(var_line - king_line)/(final_line - king_line) == static_cast<double>(var_col - king_col)/(final_col - king_col)) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//an unpinned piece can come in between
                    return true;
                }
                else{
                    return false;
                }
            }
            
            //dealing with rooks checks
            if (position[var_line][var_col].code == 3+i){
                if ((var_line == king_line) and (var_line == final_line) and (static_cast<double>(var_col - king_col)/(final_col - king_col) >= 1)){//an unpinned piece can come in between
                    return true;
                }
                else if ((var_col == king_col) and (var_col == final_col) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//an unpinned piece can come in between
                    return true;
                }
                else{
                    return false;
                }
            }
            
            //dealing with queen checks
            if (position[var_line][var_col].code == 4+i){
                if ((var_line == king_line) and (var_line == final_line) and (static_cast<double>(var_col - king_col)/(final_col - king_col) >= 1)){//an unpinned piece can come in between
                    return true;
                }
                else if ((var_col == king_col) and (var_col == final_col) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//an unpinned piece can come in between
                    return true;
                }
                else if ((static_cast<double>(var_line - king_line)/(final_line - king_line) == static_cast<double>(var_col - king_col)/(final_col - king_col)) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//an unpinned piece can come in between
                    return true;
                }
                
                else{
                    return false;
                }
            }
        }
    }
    
    //dealing with pins
    std::pair<bool, std::tuple<int, int>> pair = (is_pinned(start_square, 0));
    if (pair.first){
        var_line = get<0>(pair.second);
        var_col = get<1>(pair.second);
        
        //dealing with bishop pins
        if (position[var_line][var_col].code == 2+i){
            if ((static_cast<double>(var_line - king_line)/(final_line - king_line) == static_cast<double>(var_col - king_col)/(final_col - king_col)) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//the piece cannot leave the diagonal
                return true;
            }
            else{
                return false;
            }
        }
        
        //dealing with rooks pins
        if (position[var_line][var_col].code == 3+i){
            if ((var_line == king_line) and (var_line == final_line) and (static_cast<double>(var_col - king_col)/(final_col - king_col) >= 1)){//the piece cannot leave the rank
                return true;
            }
            else if ((var_col == king_col) and (var_col == final_col) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//the piece cannot leave the line
                return true;
            }
            else{
                return false;
            }
        }
        
        //dealing with queen checks
        if (position[var_line][var_col].code == 4+i){
            if ((var_line == king_line) and (var_line == final_line) and (static_cast<double>(var_col - king_col)/(final_col - king_col) >= 1)){//the piece cannot leave the rank
                return true;
            }
            else if ((var_col == king_col) and (var_col == final_col) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//the piece cannot leave the line
                return true;
            }
            else if ((static_cast<double>(var_line - king_line)/(final_line - king_line) == static_cast<double>(var_col - king_col)/(final_col - king_col)) and (static_cast<double>(var_line - king_line)/(final_line - king_line) >= 1)){//the piece cannot leave the diagonal
                return true;
            }
            
            else{
                return false;
            }
        }
    }
    //dealing with other cases
    return true;
}

std::vector<std::tuple<int, int>> Board::compute_legal_moves(std::tuple<int, int> square){
    std::vector<std::tuple<int, int>> possible_moves;
    std::vector<std::tuple<int, int>> legal_moves;
    //spanning all the possible moves
    possible_moves = compute_possible_moves(square);
    for (int k = 0; k < possible_moves.size(); k++){
        if (is_legal(square, possible_moves[k])){
            legal_moves.push_back(possible_moves[k]);
        }
    }
    return legal_moves;
}
std::vector<std::tuple<int, int>> Board::count_checks(){
    std::pair<bool, int> pair;
    int var_line;
    int var_col;
    std::vector<std::tuple<int, int>> checks; //all the locations of the checking pieces
    
    int i = 0;//starting index of the involved pieces codes
    int king_code = 11;
    if (whose_turn){
        i = 6;
        king_code = 5;
    }
    
    std::tuple<int, int> king_square = locations[king_code][0];
    //std::cout << get<0>(king_square) << ", " << get<1>(king_square) << "\n";
    //collecting all the checks
    for (int j = i; j<i+5; j++){
        for (int k = 0; k < locations[j].size(); k++){
            var_line = get<0>(locations[j][k]);
            var_col = get<1>(locations[j][k]);
            pair = findInVector(position[var_line][var_col].control, king_square);
            //std::cout << var_line << ", " << var_col << " " << pair.first << "\n";
            if (pair.first){
                checks.push_back({var_line, var_col});
            }
        }
    }
    //std::cout << "---------------\n";
    return checks;
}

bool Board::is_stuck(){
    std::vector<std::tuple<int, int>> possible_moves;
    //spanning all the possible moves
    for (int i = (1-whose_turn)*6; i < (2-whose_turn)*6; i++){
        for (int j = 0; j < locations[i].size(); j++){
            possible_moves = compute_possible_moves(locations[i][j]);
            for (int k = 0; k < possible_moves.size(); k++){
                if (is_legal(locations[i][j], possible_moves[k])){
                    return false;
                }
            }
        }
    }
    return true;
}

std::pair<bool, int> Board::game_over(){
    std::pair<bool, int> result;
    if (is_stuck()){
        std::string winner;
        result.first = true;
        //checks if the king is in check
        std::vector<std::tuple<int, int>> checks = count_checks();
        if ( checks.size()> 0){
            if (whose_turn){//black won
                winner = "Black";
                result.second = BLACK_WINS;
            }
            else {
                winner = "White";
                result.second = WHITE_WINS;
            }
            
            messages[4] = "Mate! " + winner + " wins";
            std::cout << messages[4] << "\n";
            load_text(messages[4], 4);
        }
        else{
            result.second = DRAW;
            
            messages[4] = "Stalemate! Draw ";
            std::cout << messages[4] << "\n";
            load_text(messages[4], 4);
        
        }
        update_board(position);
        return result;
    }
    
    //rule of the 50 moves
    auto it = std::find(record_appreciations.end() - 50, record_appreciations.end(), true);
    if ((it == record_appreciations.end()) and (record_appreciations.size() > 50)){//rule of the 50 moves satisfied ?
        result.first = true;
        result.second = DRAW;
        
        messages[4] = "Draw! 50 moves' rule";
        std::cout << messages[4] << "\n";
        load_text(messages[4], 4);
        update_board(position);
        return result;
    }
    
    //rule of repetition
    if (std::count(record_positions.begin(), record_positions.end(), record_positions[record_positions.size()-1]) > 2){
        result.first = true;
        result.second = DRAW;
        messages[4] = "Draw per repetition";
        std::cout << messages[4] << "\n";
        load_text(messages[4], 4);
        update_board(position);
        
        return result;
    }
    
    //in any other case
    result.first = false;
    result.second = 0;
    return result;
}

void Board::step_back(){
    if (record_moves.size() > 0){
        //replay the whole game till the last position
        std::vector <std::tuple<int, int, int, int, int>> _record_moves = record_moves;//record the moves
        std::vector <unsigned long long int> _record_positions = record_positions;//record the positions
        std::vector<bool> _record_appreciations = record_appreciations; //record wether the moves were capture or pawn
        
        initData(true);
        init_board(position);
        
        std::tuple<int, int, int, int, int> move;
        for (int i = 0; i < _record_moves.size() -1; i++){
            move = _record_moves[i];
            execute_move({get<0>(move), get<1>(move)}, {get<2>(move), get<3>(move)}, get<4>(move));
        }
        update_board(position);
    }
}

void Board::handle_button(int button){
    //reseting everything
    initData(true);
    init_board(position);
    //Update screen
    SDL_RenderPresent( gRenderer );
    
    if (button == 0){
        play_vs_AI(true, 100);
    }
    else if (button == 1){
        play_vs_AI(false, 100);
    }
    else if (button == 2){
        for (int i=0; i<100; i++){
            messages[4] = "game " + std::to_string(i);
            load_text(messages[4], 4);
            SDL_Rect fillRect = set_rect_for_text(4);
            SDL_RenderCopy( gRenderer, tTexture[4], nullptr, &fillRect);
            SDL_RenderPresent(gRenderer);
            AI_vs_AI_MC(85);
            initData(true);
            
            std::cout << "game" << i << "\n";
        }
    }
    else if (button == 3){
        for (int i=0; i<100; i++){
            messages[4] = "game " + std::to_string(i);
            load_text(messages[4], 4);
            SDL_Rect fillRect = set_rect_for_text(4);
            SDL_RenderCopy( gRenderer, tTexture[4], nullptr, &fillRect);
            SDL_RenderPresent(gRenderer);
            AI_vs_AI_SARSA(85);
            initData(true);
            
            std::cout << "game" << i << "\n";
        }
    }
}

void Board::save_AI(){
    std::map<unsigned long long int, std::map<std::tuple<int, int, int, int, int>, float>>::iterator it;
    std::map<std::tuple<int, int, int, int, int>, float>::iterator it2;
    std::map<std::tuple<int, int, int, int, int>, int>::iterator it3;
    
    int file_index;
    int line_index;
    std::ofstream fout;
    
    //for white Q values
    file_index = 0;
    line_index = 0;
    fout.open("Chess/WHITE_AI/Q/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
    
    for (it = Q[true].begin(); it != Q[true].end(); it++){
        //save the position data
        fout << it->first << "\n";//first line is the position code
        line_index ++;
        
        for (it2 = Q[true][it->first].begin(); it2 != Q[true][it->first].end(); it2++){
            //std::cout << "hey\n";
            fout << move_to_str(it2->first) << " " << it2 ->second << "\n";
            line_index ++;
        }
        
        if (line_index > MAX_LINE){//if the file is full
            fout.close();
            file_index ++;
            
            //start again with a new file
            fout.open("Chess/WHITE_AI/Q/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
            line_index = 0;
        }
    }
    fout.close();
    
    //for white count values
    file_index = 0;
    line_index = 0;
    fout.open("Chess/WHITE_AI/COUNT/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
    
    for (it = Q[true].begin(); it != Q[true].end(); it++){
        //save the position data
        fout << it->first << "\n";//first line is the position code
        line_index ++;
        
        for (it3 = count[true][it->first].begin(); it3 != count[true][it->first].end(); it3++){
            fout << move_to_str(it3->first) << " " << it3 ->second << "\n";
            line_index ++;
        }
        
        if (line_index > MAX_LINE){//if the file is full
            fout.close();
            file_index ++;
            
            //start again with a new file
            fout.open("Chess/WHITE_AI/COUNT/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
            line_index = 0;
        }
    }
    fout.close();
    
    
    //for black Q values
    file_index = 0;
    line_index = 0;
    fout.open("Chess/BLACK_AI/Q/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
    
    for (it = Q[false].begin(); it != Q[false].end(); it++){
        //save the position data
        fout << it->first << "\n";//first line is the position code
        line_index ++;
        
        for (it2 = Q[false][it->first].begin(); it2 != Q[false][it->first].end(); it2++){
            fout << move_to_str(it2->first) << " " << it2 ->second << "\n";
            line_index ++;
        }
        
        if (line_index > MAX_LINE){//if the file is full
            fout.close();
            file_index ++;
            
            //start again with a new file
            fout.open("Chess/BLACK_AI/Q/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
            line_index = 0;
        }
    }
    fout.close();
    
    //for black count values
    line_index = 0;
    fout.open("Chess/BLACK_AI/COUNT/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
    
    for (it = Q[false].begin(); it != Q[false].end(); it++){
        //save the position data
        fout << it->first << "\n";//first line is the position code
        line_index ++;
        
        for (it3 = count[false][it->first].begin(); it3 != count[false][it->first].end(); it3++){
            fout << move_to_str(it3->first) << " " << it3 ->second << "\n";
            line_index ++;
        }
        
        if (line_index > MAX_LINE){//if the file is full
            fout.close();
            file_index ++;
            
            //start again with a new file
            fout.open("Chess/BLACK_AI/COUNT/" + std::to_string(file_index) +".txt", std::ofstream::out | std::ofstream::trunc);
            line_index = 0;
        }
    }
    fout.close();
}

//load AI from archive
void Board::load_AI(){
    std::ifstream fin;
    std::string line;
    std::tuple<int, int, int, int, int> move;
    unsigned long long int position_code;
    
    //loading White_AI Q values
    for (const auto& entry : std::filesystem::directory_iterator("Chess/WHITE_AI/Q")) {
        const auto filenameStr = entry.path().filename().string();
        if (entry.is_regular_file()) {
            //execute a loop until EOF (End of File)
            fin.open("Chess/WHITE_AI/Q/" + filenameStr);
            while(fin){
                // Read a Line from File
                getline(fin, line);
                if (line.empty()){
                }
                else if (line[1] == ' '){//the line contains a move
                    move = str_to_move(line.substr(0, 10));
                    Q[true][position_code][move] = std::stof(line.substr(10));
                }
                else {//the line contains a position code
                    position_code = str_to_i(line);
                }
            }
            fin.close();
        }
    }
    
    //loading White_AI count values
    for (const auto& entry : std::filesystem::directory_iterator("Chess/WHITE_AI/COUNT")) {
        const auto filenameStr = entry.path().filename().string();
        if (entry.is_regular_file()) {
            //execute a loop until EOF (End of File)
            fin.open("Chess/WHITE_AI/COUNT/" + filenameStr);
            while(fin){
                // Read a Line from File
                getline(fin, line);
                if (line.empty()){
                }
                else if (line[1] == ' '){//the line contains a move
                    move = str_to_move(line.substr(0, 10));
                    count[true][position_code][move] = std::stof(line.substr(10));
                }
                else {//the line contains a position code
                    position_code = str_to_i(line);
                }
            }
            fin.close();
        }
    }
    
    //loading Black_AI Q values
    for (const auto& entry : std::filesystem::directory_iterator("Chess/BLACK_AI/Q")) {
        const auto filenameStr = entry.path().filename().string();
        if (entry.is_regular_file()) {
            //execute a loop until EOF (End of File)
            fin.open("Chess/BLACK_AI/Q/" + filenameStr);
            while(fin){
                // Read a Line from File
                getline(fin, line);
                if (line.empty()){
                }
                else if (line[1] == ' '){//the line contains a move
                    move = str_to_move(line.substr(0, 10));
                    Q[false][position_code][move] = std::stof(line.substr(10));
                }
                else {//the line contains a position code
                    position_code = str_to_i(line);
                }
            }
            fin.close();
        }
    }
    
    //loading Black_AI count values
    for (const auto& entry : std::filesystem::directory_iterator("Chess/BLACK_AI/COUNT")) {
        const auto filenameStr = entry.path().filename().string();
        if (entry.is_regular_file()) {
            //execute a loop until EOF (End of File)
            fin.open("Chess/BLACK_AI/COUNT/" + filenameStr);
            while(fin){
                // Read a Line from File
                getline(fin, line);
                if (line.empty()){
                }
                else if (line[1] == ' '){//the line contains a move
                    move = str_to_move(line.substr(0, 10));
                    count[false][position_code][move] = std::stof(line.substr(10));
                }
                else {//the line contains a position code
                    position_code = str_to_i(line);
                }
            }
            fin.close();
        }
    }
}

void Board::learn_from_directory(std::string address, bool watch){
    std::string games;
    int start_index, end_index;
    for (const auto& entry : std::filesystem::directory_iterator(address)) {
        const auto filenameStr = entry.path().filename().string();
        if (entry.is_regular_file() and filenameStr != ".DS_Store") {
            games = pgn_to_string(address + "/" + filenameStr);
            std::cout << filenameStr << "\n";
            start_index = 0;
            end_index = find_end_game(games, start_index);
            //std::cout << games.size() << "\n";
            while (end_index < games.size() - 2){
                //std::cout << end_index << "\n";
                train_from_pgn(games.substr(start_index, end_index - start_index), watch);
                initData(true);
        
                //std::cout << " next game \n";
                start_index = end_index;
                end_index = find_end_game(games, start_index);
            }
            //std::cout << " end of file \n";
        }
    }
}
