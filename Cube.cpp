#pragma once

#include <stdlib.h>
#include <SDL2/SDL.h>


struct Vertex {
    float x, y, z;
};

class Cube
{
private:

public:
    SDL_Renderer* renderer_;
    Vertex vertices[8] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
    };
    int line_index[12][2] = {
        {0, 1},
        {1, 3},
        {3, 2},
        {2, 0},
        {1, 4},
        {4, 7},
        {3, 7},
        {7, 6},
        {6, 2},
        {4, 5},
        {5, 0},
        {6, 5},
    };
    Cube() {
        
    }
    void setRenderer(SDL_Renderer* renderer) {
        renderer_ = renderer;
    }
    void update(int x, int y) {
        float SCALE = 200;
        for (auto index : line_index) {
           SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 1);
           SDL_RenderDrawLine(renderer_,    vertices[index[0]].x * SCALE, vertices[index[0]].y * SCALE, 
                                            vertices[index[1]].x * SCALE, vertices[index[1]].y * SCALE);
       }
    }
};