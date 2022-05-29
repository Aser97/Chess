//
//  text_rendering.cpp
//  Chess
//
//  Created by Aser on 28/05/2022.
//

#include "text_rendering.hpp"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//The window renderer
extern SDL_Renderer* gRenderer;

//Current displayed text texture
SDL_Texture* tTexture[5] = {nullptr};

int fontSize = 18;

std::string font_address = "Chess/Fonts/lazy.ttf";

//current font
TTF_Font *font;

int ZONE_WIDTH = 180;
int ZONE_HEIGHT = 300;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

std::string messages[5] = {"Play with White", "Play with Black", "Train AI-MC", "Train AI-SARSA", "Hello !"};

SDL_Color color = { 255, 255, 255, 255 };;

std::tuple<int, int> coord[5] = {{0, 0}, {0, SCREEN_HEIGHT/4}, {0, 2*SCREEN_HEIGHT/4}, {0, 3*SCREEN_HEIGHT/4}, {SCREEN_WIDTH - ZONE_WIDTH, (SCREEN_HEIGHT-ZONE_HEIGHT)/2}};

std::tuple<int, int> dim[5] = {{ZONE_WIDTH, SCREEN_HEIGHT/4}, {ZONE_WIDTH, SCREEN_HEIGHT/4}, {ZONE_WIDTH, SCREEN_HEIGHT/4}, {ZONE_WIDTH, SCREEN_HEIGHT/4}, {ZONE_WIDTH, ZONE_HEIGHT}};


bool init_font(){
    bool success = true;
    if (TTF_Init() != 0){
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << "\n";
        success = false;
    }
    //Open the font
    font = TTF_OpenFont(font_address.c_str(), fontSize);
    return success;
}

void close_font(){
    //Free textures
    for (int i = 0; i < 5; i++){
        if (tTexture[i] != nullptr){
            SDL_DestroyTexture( tTexture[i] );
            tTexture[i] = nullptr;
        }
    }

    //Free global font
    TTF_CloseFont( font );
    font = nullptr;
    TTF_Quit();
}
SDL_Texture* loadTexture_text(std::string text){
    if (font == nullptr){
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << "\n";
        return nullptr;
    }
    
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), color);
    if (surf == nullptr){
        std::cout << "Unable to render text surface! SDL Error: " << SDL_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surf);
    if (texture == nullptr){
        std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    return texture;
}

bool load_text(std::string text, int code){
    //Loading success flag
    bool success = true;
    
    tTexture[code] = loadTexture_text(text);
    if (tTexture[code] == nullptr){
        success = false;
    }
    //Get the texture w/h so we can center it in the screen
    return success;
}

//computes a sdl_rect for text
SDL_Rect set_rect_for_text(int code){
    SDL_Rect fillRect;
    
    int W, H;
    SDL_QueryTexture(tTexture[code], nullptr, nullptr, &W, &H);
    fillRect.x = get<0>(coord[code]) + get<0>(dim[code]) / 2 - W / 2;
    fillRect.y = get<1>(coord[code]) + get<1>(dim[code]) / 2 - H / 2;
    fillRect.w = W;
    fillRect.h = H;
    
    return fillRect;
}

int locate_button(int x, int y){
    int W, H;
    for (int i=0; i<4; i++){
        SDL_QueryTexture(tTexture[i], nullptr, nullptr, &W, &H);
        if ((x >= get<0>(coord[i]) + get<0>(dim[i]) / 2 - W/2) and (x <= get<0>(coord[i]) + get<0>(dim[i]) / 2 + W/2)){
            if ((y >= get<1>(coord[i]) + get<1>(dim[i]) / 2 - H / 2) and (y <= get<1>(coord[i]) + get<1>(dim[i]) / 2 + H / 2)){
                return i;
            }
        }
    }
    return 4;
}
