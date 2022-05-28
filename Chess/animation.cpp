//
//  animation.cpp
//  Chess
//
//  Created by Aser on 24/05/2022.
//

#include "animation.hpp"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utilities.hpp"
#include <SDL2/SDL_ttf.h>
#include "text_rendering.hpp"


int SCREEN_WIDTH = 1000;
int SCREEN_HEIGHT = 800;
int square_size = 80;
int h_margin = (SCREEN_HEIGHT - square_size*8)/2;
int w_margin = (SCREEN_WIDTH - square_size*8)/2;

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Current displayed texture
SDL_Texture* pTexture[8][8] = {nullptr};

//textures and messages for texts
extern std::string messages[5];
extern SDL_Texture* tTexture[5];

//paths to pieces images
std::string paths[12] = {"Chess/media/Pawn_White.png", "Chess/media/Knight_White.png", "Chess/media/Bishop_White.png", "Chess/media/Rook_White.png", "Chess/media/Queen_White.png", "Chess/media/King_White.png", "Chess/media/Pawn_Black.png", "Chess/media/Knight_Black.png", "Chess/media/Bishop_Black.png", "Chess/media/Rook_Black.png", "Chess/media/Queen_Black.png", "Chess/media/King_Black.png"};

//dimensions of pieces
std::tuple<int, int> piece_dim[12] = {{40, 50}, {50, 56}, {40, 70}, {50, 60}, {40, 74}, {40, 74}, {40, 50}, {50, 56}, {40, 70}, {50, 60}, {40, 74}, {40, 74}};

bool init(){
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else {
        //Create window
        gWindow = SDL_CreateWindow( "Chess Meg-As", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == nullptr ){
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            success = false;
        }
        else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL ){
                std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
                success = false;
            }
            else {
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                    std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
                    success = false;
                }
                else {
                    success = init_font();
                }
            }
        }
    }
    return success;
}

SDL_Texture* loadTexture( std::string path ){
    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr ){
        std::cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << "\n";
    }
    else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == nullptr ){
            std::cout << "Unable to create texture! SDL Error: " << SDL_GetError() << "\n";
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

bool loadMedia(int code, int line, int col){
    //Loading success flag
    bool success = true;

    //Load PNG textures
    pTexture[line][col] = loadTexture(paths[code]);
    
    if( pTexture[line][col] == nullptr ){
        std::cout << "line = " << line << " col = " << col << " Failed to load texture image!\n";
        success = false;
    }
    return success;
}

void close() {
    //Free loaded images
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (pTexture[i][j] != nullptr){
                SDL_DestroyTexture( pTexture[i][j] );
                pTexture[i][j] = nullptr;
            }
        }
    }
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    close_font();
    IMG_Quit();
    SDL_Quit();
}

void draw_square(int line, int col){
    //Render square
    SDL_Rect fillRect = set_rect_for_render(-1, line, col);
    
    if ((line + col) % 2 == 0){
        SDL_SetRenderDrawColor( gRenderer, 216, 172, 150, 120);
    }
    else{
        SDL_SetRenderDrawColor( gRenderer, 230, 243, 243, 120);
    }
    SDL_RenderFillRect( gRenderer, &fillRect );
}

void make_board(){
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 47, 47, 47, 0);
    SDL_RenderClear( gRenderer );
    
    for (int i= 0; i < 8; i ++){
        for (int j = 0; j < 8; j ++){
            draw_square(i, j);
        }
    }
}

//computes a sdl_rect to render
SDL_Rect set_rect_for_render(int code, int line, int col){
    SDL_Rect fillRect;
    
    if (code == -1){//empty square
        fillRect.y = h_margin + (7-line)*square_size;
        fillRect.x = w_margin + col*square_size;
        fillRect.h = square_size;
        fillRect.w = square_size;
        
    }
    else{
        fillRect.y = h_margin + (7-line)*square_size + (square_size - 4 - get<1>(piece_dim[code]));
        fillRect.x = w_margin + col*square_size + (square_size - get<0>(piece_dim[code]))/2;
        fillRect.h = get<1>(piece_dim[code]);
        fillRect.w = get<0>(piece_dim[code]);
    }
    return fillRect;
}

void init_board(Piece position[8][8]){
    make_board();
    SDL_Rect fillRect;
    
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (position[i][j].code != -1){
                if( !loadMedia(position[i][j].code, i, j)) {
                    //std::cout << "Failed to load media!\n";
                }
                else {
                    //Render texture to screen
                    fillRect = set_rect_for_render(position[i][j].code, i, j);
                    SDL_RenderCopy( gRenderer, pTexture[i][j], nullptr, &fillRect);
                }
            }
        }
    }
    
    for (int i=0; i<5; i++){
        if (!load_text(messages[i], i)){
            //std::cout << "Failed to load text! \n";
        }
        else {
            fillRect = set_rect_for_text(i);
            if (i < 4){
                SDL_SetRenderDrawColor( gRenderer, 16, 72, 150, 120);
                SDL_RenderFillRect( gRenderer, &fillRect );
            }
            SDL_RenderCopy( gRenderer, tTexture[i], nullptr, &fillRect);
        }
    }
}

void update_board(Piece position[8][8]){
    make_board();
    SDL_Rect fillRect;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (position[i][j].code != -1){
                //Render texture to screen
                fillRect = set_rect_for_render(position[i][j].code, i, j);
                SDL_RenderCopy( gRenderer, pTexture[i][j], nullptr, &fillRect);
            }
        }
    }
    
    for (int i=0; i<5; i++){
        fillRect = set_rect_for_text(i);
        if (i<4){
            SDL_SetRenderDrawColor( gRenderer, 16, 72, 150, 120);
            SDL_RenderFillRect( gRenderer, &fillRect );
        }
        SDL_RenderCopy( gRenderer, tTexture[i], nullptr, &fillRect);
    }
    //Update screen
    SDL_RenderPresent( gRenderer );
}

void highlight_moves(std::vector<std::tuple<int, int>> squares, Piece position[8][8]){
    make_board();
    SDL_Rect fillRect;
    for (int i = 0; i<squares.size(); i++){
        SDL_Rect fillRect = set_rect_for_render(-1, get<0>(squares[i]), get<1>(squares[i]));
        SDL_SetRenderDrawColor( gRenderer, 240, 240, 10, 120);
        SDL_RenderFillRect( gRenderer, &fillRect );
    }
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (position[i][j].code != -1){
                //Render texture to screen
                fillRect = set_rect_for_render(position[i][j].code, i, j);
                SDL_RenderCopy( gRenderer, pTexture[i][j], nullptr, &fillRect);
            }
        }
    }
    for (int i=0; i<5; i++){
        fillRect = set_rect_for_text(i);
        if (i<4){
            SDL_SetRenderDrawColor( gRenderer, 16, 72, 150, 120);
            SDL_RenderFillRect( gRenderer, &fillRect );
        }
        SDL_RenderCopy( gRenderer, tTexture[i], nullptr, &fillRect);
    }
    
    //Update screen
    SDL_RenderPresent( gRenderer );
}

std::tuple<int, int> locate_square(int x, int y){
    int line;
    int col;
    
    line = (y - h_margin) /square_size;
    col = (x - w_margin) /square_size;
    return {7-line, col};
}
