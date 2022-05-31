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
        
        bool watch = false;
        if (watch){
            init_board(board.position);
            //Update screen
            SDL_RenderPresent( gRenderer );
        }
        
        board.load_AI();
        
        //board.learn_from_directory("Chess/PGNs", watch);
        
        board.learn_from_directory("Chess/Blacks", watch);
        //board.learn_from_directory("Chess/Whites", watch);
        
        board.save_AI();
        
        std::cout << "hey\n";
        init_board(board.position);
        //Update screen
        SDL_RenderPresent( gRenderer );
        
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
        
        //save the AI
        board.save_AI();
        //Free resources and close SDL
        close();
        return 0;
    }
}
