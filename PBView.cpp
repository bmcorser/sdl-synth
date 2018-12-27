#pragma once

#include <stdlib.h>
#include <SDL2/SDL.h>

struct PBRGB {
    int r,g,b;
};

class PBView
{
private:
    PBRGB colour;
    int id_;
    float value_;
    bool isPressed_ = false;
public:
    
    SDL_Rect mainRect;
    SDL_Rect slideRect;

    PBView(int x, int y, int h, int w) {
        colour.r = 10;
        colour.g = 10;
        colour.b = 10;
        slideRect.x = x; mainRect.x = x;
        slideRect.y = y; mainRect.y = y;
        slideRect.h = h; mainRect.h = h;
        slideRect.w = w; mainRect.w = w;
    }
    ~PBView(){}
    // SDL_Rect getSDL_Rect(){ 
    //     SDL_Rectrect;
    //   mainR>x = x_;
    //   mainR>y = y_;
    //   mainR>w = w_;
    //   mainR>h = h_;
    //   mainRturnrect;
    // }
    void setSelected(bool flag) {
        if(flag) {
            colour.r = 255;
            colour.g = 10;
            colour.b = 10;
        } else {
            colour.r = 10;
            colour.g = 255;
            colour.b = 10;
        }
    }


   void setValue(float val) {
       if (isPressed_) {
            value_ = val;
            slideRect.h = mainRect.h * val;
       }
   }
   float getValue() {
       return value_;
   }
   void setPressed(bool flag) {
       isPressed_ = flag;
   }
   bool getPressed() {
       return isPressed_;
   }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255);
        //SDL_RenderFillRect(renderer, &slideRect);
        SDL_RenderDrawRect(renderer, &slideRect);
        SDL_RenderDrawRect(renderer, &mainRect);

        // TTF_Font* font = TTF_OpenFont("DroidSans.ttf", 24);
        // std::string score_text = "Value: " + std::to_string(value_);        
        // SDL_Color textColor = { 255, 255, 255, 0 };
        // SDL_Surface* textSurface = TTF_RenderText_Solid(font, score_text.c_str(), textColor);
        // SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
        // int text_width = textSurface->w;
        // int text_height = textSurface->h;
        // SDL_FreeSurface(textSurface);
        // SDL_Rect renderQuad = { mainRect.x, mainRect.y, text_width, text_height };
        // SDL_RenderCopy(renderer, text, NULL, &renderQuad);
        // SDL_DestroyTexture(text);
    }

};
