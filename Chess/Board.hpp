//
//  Board.hpp
//  Chess
//
//  Created by Aser on 22/05/2022.
//

#ifndef Board_hpp
#define Board_hpp

#include "utilities.hpp"

#define WHITE_WINS 1
#define BLACK_WINS -1
#define DRAW 0
#define MAX_LINE 5000

/*correspondance table:
 For whites:
 pawn = 0, knight = 1, bishop = 2, rook = 3, queen = 4, king = 5
 
 For blacks:
 pawn = 6, knight = 7, bishop = 8, rook = 9, queen = 10, king = 11
 
 free square = -1
 */
//makes a simplified copy of the board
struct BufferBoard{
    Piece positionBuffer[8][8];
    bool whose_turnBuffer; //1 for white and 0 for black
    std::vector <std::tuple<int, int, int, int, int>> record_movesBuffer;//record the moves
    std::vector <unsigned long long int> record_positionsBuffer;//record the positions
    std::vector<bool> record_appreciationsBuffer; //record wether the moves were capture or pawn
    std::string moves_recordBuffer;
};


class Board{
    public:
    Piece position[8][8];
    bool whose_turn; //1 for white and 0 for black
    std::vector <std::tuple<int, int>> locations[12];//locations of pieces
    std::map<unsigned long long int, std::map<std::tuple<int, int, int, int, int>, float>> Q[2]; //Q(S, A) map functions
    std::map<unsigned long long int, std::map<std::tuple<int, int, int, int, int>, int>> count[2]; //Count(S, A) map functions
    std::vector <std::tuple<int, int, int, int, int>> record_moves;//record the moves
    std::string moves_record; //string version of record_moves
    std::vector <unsigned long long int> record_positions;//record the positions
    std::vector<bool> record_appreciations; //record wether the moves were capture or pawn push
    BufferBoard boardCopy;
    
    //initializing a new game
    void initData(bool new_game);
    
    //prints the board in the buffer
    void memorizeBoard();
    
    //computes the controled squares of a piece
    std::vector <std::tuple<int, int>> controled_squares(std::tuple<int, int> square, int code);

    void display_board();
        
    void display_vector(std::vector <std::tuple<int, int>> vector);
        
    
    //find the line of a piece from its code, its target square and possibly its column
    int find_line(int code, bool capture, int target_line, int target_column, bool known_column, int column);
    
    //find the column of a piece from its code, its target square and possibly its column
    int find_column(int code, bool capture, int target_line, int target_column, bool known_line, int line);
    
    //execute a move taken in pgn notation
    std::pair<int, std::tuple<int, int, int, int, int>> interpret_pgn(std::string move);
    
    void castle(bool short_castle);
    
    void execute_move(std::tuple<int, int> start_square, std::tuple<int, int> target_square, int promote_code);
    
    void train_from_pgn(std::string game, bool watch);
    
    void train_from_record(int reward);
    
    void play_vs_machine(bool player, std::string machine, int Elo = 0);
    
    void machin_vs_machin_MC(std::string machine1, bool color, std::string machine2, int Elo, int proba, float horizon, bool train = true);
    
    //proposes a move
    std::tuple<int, int, int, int, int> propose_move(int proba, bool color, bool train = true);
    
    void compute_possible_moves(std::tuple<int, int> square);
    
    //checks if a possible move is legal
    bool is_legal(std::tuple<int, int> start_square, std::tuple<int, int> final_square);
    
    void compute_legal_moves(std::tuple<int, int> square);
    
    bool is_controled(std::tuple<int, int> square);
    
    std::pair<bool, std::tuple<int, int>> is_pinned(std::tuple<int, int> square, int numb_checks, std::tuple<int, int> checking_square = {-1, -1});
    
    //lists all the squares containing a piece that checks the king
    std::vector<std::tuple<int, int>> count_checks();
    
    bool is_stuck();
    
    std::pair<bool, int> game_over(bool train = false);
    
    void step_back();
    
    void handle_button(int button);
    
    void save_AI();
    
    void load_AI();
    
    void learn_from_directory(std::string address, bool watch);
};

#endif /* Board_hpp */
