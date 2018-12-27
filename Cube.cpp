#pragma once

#include <stdlib.h>
#include <SDL2/SDL.h>

#define WIDTH 1000 
#define HEIGHT 800

struct vec3 {
    float x, y, z;
};
struct mat4x4 {
    float m[4][4] = { 0 };
};
struct line3d {
    vec3 a, b;
};

class Cube
{
private:
    SDL_Renderer* renderer_;
    mat4x4 matProj;
    
    vec3 vertices[8] = {
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
    };
    line3d line_index[12] = {
        { vertices[0] ,  vertices[1] },
        { vertices[1] ,  vertices[3] },
        { vertices[3] ,  vertices[2] },
        { vertices[2] ,  vertices[0] },
        { vertices[1] ,  vertices[4] },
        { vertices[4] ,  vertices[7] },
        { vertices[3] ,  vertices[7] },
        { vertices[7] ,  vertices[6] },
        { vertices[6] ,  vertices[2] },
        { vertices[4] ,  vertices[5] },
        { vertices[5] ,  vertices[0] },
        { vertices[6] ,  vertices[5] },
    };

    void MultiplyMatrixVector(vec3 &i, vec3 &o, mat4x4 &m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}
public:

    void updateCam(float fov) {
        // Projection Matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = fov;
		float fAspectRatio = (float)HEIGHT / (float)WIDTH;
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
    }

    Cube() {
        updateCam(180.0f);
    }
    void setRenderer(SDL_Renderer* renderer) {
        renderer_ = renderer;
    }
    void update(SDL_Renderer* renderer, float in01, float SCALE, float MOVE, float in04, float in05) {

        updateCam(in04);

        // Set up rotation matrices
		mat4x4 matRotZ, matRotX, matRotY;
		float fTheta = in01 ;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(in05 * 0.5f);
		matRotX.m[1][2] = sinf(in05 * 0.5f);
		matRotX.m[2][1] = -sinf(in05 * 0.5f);
		matRotX.m[2][2] = cosf(in05 * 0.5f);
		matRotX.m[3][3] = 1;

        // Rotation Y



        //float SCALE = 100.0f;
        //float MOVE = y;
        for (auto v : line_index) {

            v.a.x *= SCALE;
            v.a.y *= SCALE;
            v.a.z *= SCALE;
            v.b.x *= SCALE;
            v.b.y *= SCALE;
            v.b.z *= SCALE;

            

            line3d vRotZ, vRotX, vRotXZ;
            MultiplyMatrixVector(v.a, vRotZ.a, matRotZ);
            MultiplyMatrixVector(v.b, vRotZ.b, matRotZ);

            MultiplyMatrixVector(v.a, vRotX.a, matRotX);
            MultiplyMatrixVector(v.b, vRotX.b, matRotX);

            MultiplyMatrixVector(vRotZ.a, vRotXZ.a, matRotX);
            MultiplyMatrixVector(vRotZ.b, vRotXZ.b, matRotX);

            // move
            vRotXZ.a.x += MOVE;
            vRotXZ.a.y += MOVE;
            vRotXZ.a.z += MOVE;

            vRotXZ.b.x += MOVE;
            vRotXZ.b.y += MOVE;
            vRotXZ.b.z += MOVE;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
            // SDL_RenderDrawLine(renderer_,   v.a.x,  v.a.y, 
            //                                 v.b.x,  v.b.y);

            // SDL_RenderDrawLine(renderer_,   vRotZ.a.x,  vRotZ.a.y, 
            //                                 vRotZ.b.x,  vRotZ.b.y);

            // SDL_RenderDrawLine(renderer_,   vRotX.a.x,  vRotX.a.y, 
            //                                 vRotX.b.x,  vRotX.b.y);

            SDL_RenderDrawLine(renderer,   vRotXZ.a.x,  vRotXZ.a.y, 
                                            vRotXZ.b.x,  vRotXZ.b.y);
       }
    }
};