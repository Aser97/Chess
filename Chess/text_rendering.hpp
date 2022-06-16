//
//  text_rendering.hpp
//  Chess
//
//  Created by Aser on 28/05/2022.
//

#ifndef text_rendering_hpp
#define text_rendering_hpp
#include <iostream>
#include <SDL2/SDL.h>

bool init_font();
void close_font();
SDL_Texture* loadTexture_text(std::string text);
bool load_text(std::string text, int code);
SDL_Rect set_rect_for_text(int code);
int locate_button(int x, int y);
#endif /* text_rendering_hpp */
