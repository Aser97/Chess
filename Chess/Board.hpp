//
//  Board.hpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//

#ifndef Board_hpp
#define Board_hpp

#include <iostream>
#include <iomanip>
#include "utilities.hpp"
#include <vector>
#include <map>
#include <random>
/*correspondance table:
 For whites:
 pawn = 0, knight = 1, bishop = 2, rook = 3, queen = 4, king = 5
 
 For blacks:
 pawn = 6, knight = 7, bishop = 8, rook = 9, queen = 10, king = 11
 
 free square = -1
 */


class Board{
    public:
    Piece position[8][8];
    bool whose_turn; //1 for white and 0 for black
    std::vector <std::tuple<int, int>> locations[12];//locations of pieces
    std::map<unsigned long long int, std::map<std::tuple<int, int, int, int, int>, float>> Q[2]; //Q(S, A) map functions
    std::map<unsigned long long int, std::map<std::tuple<int, int, int, int, int>, int>> count[2]; //Count(S, A) map functions
    std::vector <std::tuple<int, int, int, int, int>> record_moves;//record the moves
    std::vector <unsigned long long int> record_positions;//record the positions
    std::vector<bool> record_appreciations; //record wether the moves were capture or pawn push
    //initializing a new game
    void initData(bool new_game, bool turn = 1, int pos[8][8] = {});
    
    void update_values(unsigned long long int position_code, std::tuple<int, int, int, int, int> move, bool color, int init_value);
    //computes the controled squares of a piece
    std::vector <std::tuple<int, int>> controled_squares(std::tuple<int, int> square, int code);

    void display_board();
        
    void display_vector(std::vector <std::tuple<int, int>> vector);
        
    
    //find the line of a piece from its code, its target square and possibly its column
    int find_line(int code, bool capture, int target_line, int target_column, bool known_column = false, int column = -1);
    
    //find the column of a piece from its code, its target square and possibly its column
    int find_column(int code, bool capture, int target_line, int target_column, bool known_line = false, int line = -1);
    
    //execute a move taken in pgn notation
    std::pair<int, std::tuple<int, int, int, int, int>> interpret_pgn(std::string move);
    
    void castle(bool short_castle);
    
    void execute_move(std::tuple<int, int> start_square, std::tuple<int, int> target_square, int promote_code);
    
    void train_from_pgn(std::string game);
    
    void train_from_record(int reward);
    
    void play_vs_AI(bool player);
    
    void AI_vs_AI_SARSA();
    
    void AI_vs_AI_MC();
    //proposes a move
    std::tuple<int, int, int, int, int> propose_move(int proba, bool color);
    
    std::vector<std::tuple<int, int>> compute_possible_moves(std::tuple<int, int> square);
    
    //checks if a possible move is legal
    bool is_legal(std::tuple<int, int> start_square, std::tuple<int, int> final_square);
    
    std::vector<std::tuple<int, int>> compute_legal_moves(std::tuple<int, int> square);
    
    bool is_controled(std::tuple<int, int> square);
    
    std::pair<bool, std::tuple<int, int>> is_pinned(std::tuple<int, int> square, int numb_checks);
    
    //lists all the squares containing a piece that checks the king
    std::vector<std::tuple<int, int>> count_checks();
    
    bool is_stuck();
    
    std::pair<bool, int> game_over();
    
    void step_back();
    
    void handle_button(int button);
};

#endif /* Board_hpp */
