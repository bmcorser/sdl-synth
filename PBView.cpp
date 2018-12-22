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

   #define PRINT(x) std::cout << x << std::endl;

   void setValue(float val) {
       slideRect.h = mainRect.h * val;
   }

    void draw(SDL_Renderer* renderer) {
        if (SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255)) {
            PRINT("bum");
        }
        //SDL_RenderFillRect(renderer, &slideRect);
         SDL_RenderDrawRect(renderer, &slideRect);
         SDL_RenderDrawRect(renderer, &mainRect);
    }

};
