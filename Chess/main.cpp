//
//  main.cpp
//  Chess
//
//  Created by Aser on 19/05/2022.
//

#include <iostream>
#include <iomanip>
#include "utilities.hpp"
#include "zobra_hashing.hpp"
#include <vector>
#include "Board.hpp"
#include "animation.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "text_rendering.hpp"

extern std::string messages[5];
//The window renderer
extern SDL_Renderer* gRenderer;

extern SDL_Texture* tTexture[5];

int main(int argc, const char * argv[]) {
    if( !init() ) {
        std::cout << "SDL Failed to initialize!\n";
        close();
        return 1;
    }
    else {
        int x, y;
        int button;
        
        Board board;
        board.initData(true);
        initTable();
        
        init_board(board.position);
        //Update screen
        SDL_RenderPresent( gRenderer );
        
        
        /*
        int index = 0;
        for (int i= 1; i<26; i++){
            std::string games = pgn_to_string("Chess/Blacks/games"+std::to_string(i)+".txt");
            int start_index = 0;
            int end_index = find_end_game(games, start_index);
            
            while (end_index < games.size() - 2){
                board.train_from_pgn(games.substr(start_index, end_index - start_index));
                board.initData(true);
        
            
                //std::cout << " next game \n";
                start_index = end_index;
                end_index = find_end_game(games, start_index);
            }
            index ++;
            std::cout << index << " end of file \n";
        }
        
        
        for (int i= 1; i<24; i++){
            std::string games = pgn_to_string("Chess/Whites/games"+std::to_string(i)+".txt");
            int start_index = 0;
            int end_index = find_end_game(games, start_index);
            
            while (end_index < games.size() - 2){
                board.train_from_pgn(games.substr(start_index, end_index - start_index));
                board.initData(true);
                
                //std::cout << "next game \n";
                start_index = end_index;
                end_index = find_end_game(games, start_index);
            }
            index ++;
            std::cout << index << " end of file \n";
        }
        
        board.save_AI();
        */
        board.load_AI();
        
        SDL_Event e;
        bool quit = false;
        while (!quit){
            while (SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT){
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN){
                    quit = true;
                }
                //If mouse event happened
                if(e.type == SDL_MOUSEBUTTONDOWN){
                    //Get mouse position
                    SDL_GetMouseState( &x, &y );
                    button = locate_button(x, y);
                    if (button < 4){
                        board.handle_button(button);
                    }
                }
            }
        }
        //this line is necessary for the renderer to be destroyed safely when user don't do anything after opening the app
        update_board(board.position);
        //Free resources and close SDL
        close();
        return 0;
    }
}
