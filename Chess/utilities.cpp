//
//  utilities.cpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//

#include "utilities.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
/*correspondance table:
 For whites:
 pawn = 0, knight = 1, bishop = 2, rook = 3, queen = 4, king = 5
 
 For blacks:
 pawn = 6, knight = 7, bishop = 8, rook = 9, queen = 10, king = 11
 
 free square = -1
 */

//this function tests whether two pieces are in same color
bool are_nemesis(int code1, int code2){
    if ((code1 ==-1) or (code2 == -1)){
        return true;
    }
    else if ((code1 < 6) and (code2 < 6)){
        return false;
    }
    else if ((code1 > 5) and (code2 > 5)){
        return false;
    }
    else {
        return true;
    }
}

bool from_a_to_h(char c){
    return (((int) c > 96) and ((int) c < 105));
}

bool from_1_to_8(char c){
    return ((int) c > 48) and ((int) c < 57);
}

int letter_to_int(char c, bool whose_turn){//convert a piece letter to its code
    int code;
    if (c == 'K'){
        code = 5;
    }
    else if (c == 'Q'){
        code = 4;
    }
    else if (c == 'N'){
        code = 1;
    }
    else if (c == 'R'){
        code = 3;
    }
    else if (c == 'B'){
        code = 2;
    }
    else {
        std::cout << "I don't recognize this piece : " << c << " dude \n";
        code = -7;
    }
    
    if (whose_turn) {
        return code;
    }
    else {
        return code + 6;
    }
}

//Convert a string to int
unsigned long long int str_to_i(std::string s){
    int len = s.size();
    unsigned long long int h = 0;
    int digit;
    for (int i = 0; i < len; i++){
        digit = (int) s[i] - 48;
        h = 10 * h + digit;
    }
    
    return h;
}
//turn a pgn into a string
std::string pgn_to_string(std::string address){
    std::string result;
    
    std::ifstream fin;
    fin.open(address);
    std::string line;
    
    // Execute a loop until EOF (End of File)
    while (fin) {
        // Read a Line from File
        getline(fin, line);
        
        //append it to the result
        result.append(line);
        
        result.append(" ");
    }
    return result;
}

//returns the header of a pgn ending position
int header_extractor(std::string str){
    int i = 0;
    bool header_end = false;
    while (not header_end){
        if ((str[i] == '1') and (str[i+1] == '.') and (str[i+2] == ' ')){
            header_end = true;
        }
        else{
            i ++; 
        }
    }
    return i;
}

//extracts the move and returns it and the index of the next move
std::pair<std::string, int> move_extractor(std::string str, int index, bool turn){
    std::pair<std::string, int> pair;
    //dealing with final score
    if (((str[index] == '1') or (str[index] == '0')) and (str[index + 1] == '-')){
        pair.second = index + 3;
        pair.first = str.substr(index, 3);
        //std::cout << pair.first << "\n";
        return pair;
    }
    else if (str[index + 1] == '/'){
        pair.second = index + 7;
        pair.first = str.substr(index, 7);
        //std::cout << pair.first << "\n";
        return pair;
    }
    
    int i = 0;
    if (turn){
        while (str[index + i] != '.'){
            i ++;
        }
        i++;
        while (str[index + i] == ' '){
            i ++;
        }
        
    }
    
    int j = 0;
    while (str[index+i+j] == ' '){
        j++;
    }
    int k = 0;
    while (str[index + i + j + k] != ' '){
        k ++;
    }
    pair.first = str.substr(index+i+j, k);
    pair.second = index + i + j + k + 1;
    //std::cout << pair.first << "\n";
    return pair;
}

//compute the total number of moves of the game
int game_length(std::string game){
    int index = game.size() - 1;
    int length = 0;
    while (game[index] != '.'){
        index --;
    }
    index --;
    int i = 0;
    while (game[index] != ' '){
        index --;
        i ++;
    }
    int j = 0;
    while (j < i){
        j ++;
        length *= 10;
        length += (int) game[index+j] - 48;
    }
    return length;
}

std::pair<bool, int > findInVector(std::vector<std::tuple<int, int>> vecOfElements, std::tuple<int, int> square)
{
    std::pair<bool, int > result;
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), square);
    if (it != vecOfElements.end())
    {
        result.second = distance(vecOfElements.begin(), it);
        result.first = true;
    }
    else
    {
        result.first = false;
        result.second = -1;
    }
    return result;
}

Piece create_piece(int code){
    Piece piece;
    piece.code = code;
    return piece;
}

//tells which color won:(-1)->black, 1->white, 0->draw
int winner_extractor(std::string game){
    int index = game.size() - 1;
    while (game[index] == ' '){
        index --;
    }
    int digit = (int) game[index] - 48;
    if (digit == 1){
        return 1;
    }
    else if (digit == 0){
        return -1;
    }
    else {
        return 0;
    }
}

//finds the indice of the end of the game (game is a list of games)
int find_end_game(std::string game, int start_index){
    int index = start_index;
    while (not ((game.substr(index, 4) == " 0-1") or (game.substr(index, 4) == " 1-0") or (game.substr(index, 4) == " 1/2"))){
        index ++;
    }
    if ((game.substr(index, 4) == " 0-1") or (game.substr(index, 4) == " 1-0")){
        return index + 4;
    }
    else{
        return index + 8;
    }
}

void display_move(std::tuple<int, int, int, int, int> move){
    std::cout << get<0>(move) << ", " << get<1>(move) << " -> " << get<2>(move) << ", " << get<3>(move) << " = " << get<4>(move) <<"\n";
}

//transforms a move to a 10 chars long string
std::string move_to_str(std::tuple<int, int, int, int, int> move){
    std::string result = std::to_string(get<0>(move)) + " " + std::to_string(get<1>(move)) + " " + std::to_string(get<2>(move)) + " " + std::to_string(get<3>(move)) + " " + std::to_string(get<4>(move));
    
    //making sure the result has the good size
    if ((get<3>(move)!= -1) and (get<3>(move) != 10)){
        result.append(" ");
    }
    return result;
}

std::tuple<int, int, int, int, int> str_to_move(std::string str){
    std::tuple<int, int, int, int, int> move = {(int) str[0] -48, (int) str[2] -48, (int) str[4] -48, (int) str[6] -48, -1};
    if (str[8] != '-'){//if the promote code is not -1
        if (str[9] == '0'){//for promote to queen
            get<4>(move) = 10;
        }
        else{
            get<4>(move) = (int) str[8] -48;
        }
    }
    return move;
}

void save_position(unsigned long long int position_code, std::map<std::tuple<int, int, int, int, int>, float> map_Q, std::map<std::tuple<int, int, int, int, int>, int> map_count, bool color){
    
    std::string AI;
    if (color){
        AI = "White";
    }
    else{
        AI = "Black";
    }
    std::ofstream fout;
    fout.open("Chess/" + AI +"_AI/Q/" + std::to_string(position_code) +".txt", std::ofstream::out | std::ofstream::trunc);

    std::map<std::tuple<int, int, int, int, int>, float>::iterator it;
    //save the Q values
    for (it = map_Q.begin(); it != map_Q.end(); it++){
        fout << move_to_str(it->first) << " " << it ->second << "\n";   //
    }
    fout.close();
    
    fout.open("Chess/" + AI +"_AI/count/" + std::to_string(position_code) +".txt", std::ofstream::out | std::ofstream::trunc);
    std::map<std::tuple<int, int, int, int, int>, int>::iterator it2;
    //save the count values
    for (it2 = map_count.begin(); it2 != map_count.end(); it2++){
        fout << move_to_str(it2->first) << " " << it2 ->second << "\n";   //
    }
    fout.close();
}
