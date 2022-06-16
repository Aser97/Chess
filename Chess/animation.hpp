//
//  animation.hpp
//  Chess
//
//  Created by Aser on 24/05/2022.
//

#ifndef animation_hpp
#define animation_hpp
#include "utilities.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(int code, int line, int col);

//Frees media and shuts down SDL
void close();

SDL_Texture* loadTexture(std::string path);

void draw_square(int line, int col);

void make_board();

void init_board(Piece position[8][8]);

std::tuple<int, int> locate_square(int x, int y);

void update_board(Piece position[8][8]);

void highlight_moves(std::vector<std::tuple<int, int>> squares, Piece position[8][8]);

//computes a sdl_rect to render texture
SDL_Rect set_rect_for_render(int code, int line, int col);
#endif /* animation_hpp */
