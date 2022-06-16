//
//  utilities.cpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//

#include "utilities.hpp"

#include <map>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <fcntl.h>
#include "Board.hpp"

/*correspondance table:
 For whites:
 pawn = 0, knight = 1, bishop = 2, rook = 3, queen = 4, king = 5
 
 For blacks:
 pawn = 6, knight = 7, bishop = 8, rook = 9, queen = 10, king = 11
 
 free square = -1
 */

std::ifstream filestream;
int filedescriptor;
int stdoutCopy;

int PawnTable[] ={
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 27, 27, 10,  5,  5,
     0,  0,  0, 25, 25,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-25,-25, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

int KnightTable[] ={
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-20,-30,-30,-20,-40,-50
};

int BishopTable[] ={
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-40,-10,-10,-40,-10,-20
};

int RookTable[] ={
    0,  0,  0,  0,  0,  0,  0,  0,
    5,  10,  10,  10,  10,  10,  10,5,
    -5,  0,  0,  0,  0,  0,  0,  -5,
    -5,  0,  0,  0,  0,  0,  0,  -5,
    -5,  0,  0,  0,  0,  0,  0,  -5,
    -5,  0,  0,  0,  0,  0,  0,  -5,
    -5,  0,  0,  0,  0,  0,  0,  -5,
    0,  0,  0,  5,  5,  0,  0,  0
};

int QueenTable[] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10,  0,  0,  0,  0,  0,  0,  -10,
    -10,  0,  5,  5,  5,  5,  0,  -10,
    -5,  0,  5,  5,  5,  5,  0,  -5,
    0,  0,  5,  5,  5,  5,  0,  -5,
    -10,  5,  5,  5,  5,  5,  0,  -10,
    -10,  0,  5,  0,  0,  0,  0,  -10,
    -20, -10, -10, -5, -5, -10, -10, -20
};

int KingTable[] ={
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -20, -30, -30, -40, -40, -30, -30, -20,
  -10, -20, -20, -20, -20, -20, -20, -10,
   20,  20,   0,   0,   0,   0,  20,  20,
   20,  30,  10,   0,   0,  10,  30,  20
};

int* PieceSquareValue[6] = {PawnTable, KnightTable, BishopTable, RookTable, QueenTable, KingTable};

extern int sign[2];

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

//for promotion (stockfish)
char int_to_letter(int code){
    if (code == -1){
        return ' ';
    }
    else if ((code == 1) or (code == 7)){
        return 'n';
    }
    else if ((code == 2) or (code == 8)){
        return 'b';
    }
    else if ((code == 3) or (code == 9)){
        return 'r';
    }
    else {
        return 'q';
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
        
        //append it to the result if not empty line
        if (not line.empty()){
            result.append(line);
        
            result.append(" ");
        }
    }
    return result;
}

//returns the header of a pgn ending position
int header_extractor(std::string str){
    int i = 0;
    bool header_end = false;
    while (not header_end){
        if (str.substr(i, 3) == " 1."){
            header_end = true;
        }
        else{
            i ++; 
        }
    }
    return i+1;
}

//extracts the move and returns it and the index of the next move
std::pair<std::string, int> move_extractor(std::string str, int index, bool turn){
    std::pair<std::string, int> pair;
    //dealing with final score
    if ((str.substr(index, 2) == "1-") or (str.substr(index, 2) == "0-")){
        pair.second = index + 3;
        pair.first = str.substr(index, 3);
        //std::cout << pair.first << "\n";
        return pair;
    }
    if ((str.substr(index+1, 2) == "1-") or (str.substr(index+1, 2) == "0-")){
        pair.second = index + 4;
        pair.first = str.substr(index+1, 3);
        //std::cout << pair.first << "\n";
        return pair;
    }
    if (str[index + 1] == '/'){
        pair.second = index + 7;
        pair.first = str.substr(index, 7);
        //std::cout << pair.first << "\n";
        return pair;
    }
    if (str[index + 2] == '/'){
        pair.second = index + 8;
        pair.first = str.substr(index+1, 7);
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

//transforms a move to a 5 chars long string
std::string move_to_str(std::tuple<int, int, int, int, int> move){
    char result[5];
    result[1] = get<0>(move) + 49;
    result[0] = (char) (97 + get<1>(move));
    result[3] = get<2>(move) + 49;
    result[2] = (char) (97 + get<3>(move));
    result[4] = int_to_letter(get<4>(move));
    result[5] = '\0';
    
    return std::string(result);
}

std::tuple<int, int, int, int, int> str_to_move(std::string str){
    std::tuple<int, int, int, int, int> move = {(int) str[1] -49, (int) str[0] -97, (int) str[3] -49, (int) str[2] -97, -1};
    if ((str.size() == 5) and (str[4] != ' ')){//if the promote code is not -1
        get<4>(move) = letter_to_int((char) (str[4]-32), str[3] == '8');
    }
    return move;
}

int getNextMoveStockfish(std::string& str0, std::string& str1, std::string& str2, std::string& nextMove, bool turn){
    filedescriptor  = open("Chess/redirected_output.txt",
                      O_WRONLY| O_NONBLOCK| O_TRUNC);
    stdoutCopy = dup(STDOUT_FILENO);
    dup2(filedescriptor, STDOUT_FILENO);
    close(filedescriptor);
    std::string cmd = "Chess/stockfish \"" + str0 + "\" \"" + str1 + "\" \"" + str2 + "\"";
    system(cmd.c_str());
    
    dup2(stdoutCopy, STDOUT_FILENO);
    close(stdoutCopy);
    return readNextMoveFromFile(nextMove, turn);
}

int readNextMoveFromFile(std::string& nextMove, bool turn){
    filestream.open("Chess/redirected_output.txt", std::ios::out);
    std::string line;
    int n, m, eval;
    int count = 0;
    int count_ = 0;
    bool mate = false;
    
    while(getline(filestream, line)) {
        count ++;
        //look for mate
        n = line.find(" mate ");
        if (n != -1){
            mate = true;
            if (line[n + 6] == '-'){//the opponent wins
                eval = 31800 * sign[not turn];
            }
            else{
                eval = 31800 * sign[turn];
            }
        }
        //look for the move
        n = line.find("bestmove");
        if (n != -1){
            filestream.close();
            nextMove = line.substr(n + 9, 5);
            break;
        }
    }
    //read the eval
    if (!mate){
        filestream.open("Chess/redirected_output.txt", std::ios::out);
        while (true) {
            getline(filestream, line);
            count_ ++;
            if (count_ == count-1){//if we reach the last line before bestmove
                n = line.find(" cp ");
                m = 0;
                while (line[n+4+m] != ' '){//looking for the ending position of the eval
                    m ++;
                }
                eval = sign[turn]*std::stoi(line.substr(n+4, m));
                break;
            }
        }
        //std::cout << "eval = " << eval << "\n";
        filestream.close();
    }
    return eval;
}

int eval_pos(std::vector <std::tuple<int, int>> locations[12]){
    int white_val = 0;
    int black_val = 0;
    int line, col;
    for (int i=0; i<6; i++){
        for (int j=0; j<locations[i].size(); j++){
            line = get<0>(locations[i][j]);
            col = get<1>(locations[i][j]);
            white_val += PieceSquareValue[i][(7-line)*8+col];
        }
    }
    for (int i=6; i<12; i++){
        for (int j=0; j<locations[i].size(); j++){
            line = get<0>(locations[i][j]);
            col = get<1>(locations[i][j]);
            black_val += PieceSquareValue[i-6][line*8 + col];
        }
    }
    if (white_val>black_val){
        return WHITE_WINS;
    }
    if (white_val<black_val){
        return BLACK_WINS;
    }
    else{
        return DRAW;
    }
}

int eval_Stockfish(int eval){
    if (eval > 150){
        return WHITE_WINS;
    }
    if (eval < -150){
        return BLACK_WINS;
    }
    return DRAW;
}
