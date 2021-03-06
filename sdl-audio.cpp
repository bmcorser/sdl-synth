#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include "MainAudioPipe.cpp"
#include "PBView.cpp"
#include "Cube.cpp"

#define PRINT(x) std::cout << x << std::endl;

void draw();

#define WIDTH 1000 
#define HEIGHT 800

Uint32 sampleRate = 44100;
Uint32 frameRate =    60;
Uint32 floatStreamLength = 1024;
Uint32 samplesPerFrame;
Uint32 msPerFrame;
double practicallySilent = 0.001;

Uint32 audioBufferLength = 44100;
float *audioBuffer;

SDL_atomic_t audioCallbackLeftOff;
Sint32 audioMainLeftOff;
Uint8 audioMainAccumulator;

SDL_AudioDeviceID AudioDevice;
SDL_AudioSpec audioSpec;

//SDL_Surface * windowSurface = NULL;

double speed = 0.025;
// todo use value from cube class


SDL_Event event;
SDL_Event windowEvent;
SDL_bool running = SDL_TRUE;

SDL_Window *window;
SDL_Renderer * renderer;

PBView v1 = PBView(20, 500, 120, 18);
PBView v2 = PBView(40, 500, 120, 18);
PBView v3 = PBView(60, 500, 120, 18);
PBView v4 = PBView(80, 500, 120, 18);
PBView v5 = PBView(100, 500, 120, 18);
Cube cube;

// synth objects
MainAudioPipe mainAudioPipe_;

typedef struct {
    float *waveform;
    Uint32 waveformLength;
    double volume;
    double pan;
    double frequency;
    double phase;
} voice;

void logSpec(SDL_AudioSpec *as) {
    printf(
           " freq______%5d\n"
           " format____%5d\n"
           " channels__%5d\n"
           " silence___%5d\n"
           " samples___%5d\n"
           " size______%5d\n\n",
           (int) as->freq,
           (int) as->format,
           (int) as->channels,
           (int) as->silence,
           (int) as->samples,
           (int) as->size
           );
}

void logVoice(voice *v) {
    printf(
           " waveformLength__%d\n"
           " volume__________%f\n"
           " pan_____________%f\n"
           " frequency_______%f\n"
           " phase___________%f\n",
           v->waveformLength,
           v->volume,
           v->pan,
           v->frequency,
           v->phase
           );
}

void logWavedata(float *floatStream, Uint32 floatStreamLength, Uint32 increment) {
    printf("\n\nwaveform data:\n\n");
    Uint32 i=0;
    for (i = 0; i < floatStreamLength; i += increment)
        printf("%4d:%2.16f\n", i, floatStream[i]);
    printf("\n\n");
}

#pragma proc
void audioCallback(void *unused, Uint8 *byteStream, int byteStreamLength) {
    float* floatStream = (float*) byteStream;
    Uint32 i;
    for (i = 0; i < floatStreamLength; i++) {

        floatStream[i] = static_cast<float>( mainAudioPipe_.getOutput() );
    }
}



int init(void) {

    if (SDL_Init( SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL not initulized error:" << SDL_GetError( ) << std::endl;
    }
    
    window = SDL_CreateWindow("My Game Window",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  WIDTH, HEIGHT,
                                  SDL_WINDOW_SHOWN
                                  );
    //windowSurface = SDL_GetWindowSurface(window);
    
    if ( NULL == window) {
        std::cout << "couldent make window:" << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }    

        // Setup renderer
    renderer =  SDL_CreateRenderer( window, -1, 0);
    cube.setRenderer(renderer);

    draw();

    SDL_AudioSpec want;
    SDL_zero(want);
    
    want.freq = sampleRate;
    want.format = AUDIO_F32;
    want.channels = 2;
    want.samples = floatStreamLength;
    want.callback = audioCallback;
    
    AudioDevice = SDL_OpenAudioDevice(NULL, false, &want, &audioSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (AudioDevice == 0) {
        printf("\nFailed to open audio: %s\n", SDL_GetError());
        return 1;
    }

    printf("want:\n");
    logSpec(&want);
    printf("audioSpec:\n");
    logSpec(&audioSpec);
    
    if (audioSpec.format != want.format) {
        printf("\nCouldn't get Float32 audio format.\n");
        return 2;
    }
    
    sampleRate = audioSpec.freq;
    floatStreamLength = audioSpec.size / 4;
    samplesPerFrame = sampleRate / frameRate;
    msPerFrame = 1000 / frameRate;
    audioMainLeftOff = samplesPerFrame * 8;
    SDL_AtomicSet(&audioCallbackLeftOff, 0);
    
    if (audioBufferLength % samplesPerFrame)
        audioBufferLength += samplesPerFrame - (audioBufferLength % samplesPerFrame);
    audioBuffer = (float *)malloc( sizeof(float)*audioBufferLength );

    
    
    return 0;
}

void cleanUp(void) {
    free(audioBuffer);
    SDL_CloseAudioDevice(AudioDevice);
    //SDL_FreeSurface(windowSurface);
    //windowSurface = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
}

void draw(){
    SDL_UpdateWindowSurface(window);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); // fill the scene
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // for(int i = 0; i < WIDTH; i++) {
    //     float f = static_cast<float>(i);
    //     f /= (float)WIDTH * speed;
    //     float s = cosf(f);
    //     s *= 100.0f;
    //     int y = static_cast<int>(s);
    //     y += 200;
    //     SDL_RenderDrawPoint(renderer, i, y);
    // }

  
   v1.draw(renderer);
   v2.draw(renderer);
   v3.draw(renderer);
   v4.draw(renderer);
   v5.draw(renderer);
   cube.update(renderer, v1.getValue()*10.0, v2.getValue()*1000.0, v3.getValue()*1000.0, v4.getValue()*10.0, v5.getValue()*10.0);
    

    SDL_RenderPresent(renderer);
}

bool inRect(const SDL_Event &event, const PBView & v) {
    if(event.motion.x > v.mainRect.x && event.motion.x < v.mainRect.x+v.mainRect.w && event.motion.y > v.mainRect.y && event.motion.y < v.mainRect.y+v.mainRect.h) {
        return true;
    } else {
        return false;
    }

}

int main(int argc, const char * argv[]) {
    
    float  syncCompensationFactor = 0.0016;
    Sint32 mainAudioLead;
    Uint32 i;
    
    if (init())
        return 1;

    SDL_Delay(42);
    SDL_PauseAudioDevice(AudioDevice, 0);

    while (running) {
        

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (inRect(event, v1)) {
                    v1.setPressed(true);
                    std::cout << "inside f" << std::endl;
                    v1.setSelected(true);
                    mainAudioPipe_.editObject(1);
                    draw();
                }
                else if (inRect(event, v2)) { v2.setPressed(true); }
                else if (inRect(event, v3)) { v3.setPressed(true); }
                else if (inRect(event, v4)) { v4.setPressed(true); }
                else if (inRect(event, v5)) { v5.setPressed(true); }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                v1.setPressed(false);
                v2.setPressed(false);
                v3.setPressed(false);
                v4.setPressed(false);
                v5.setPressed(false);
                if(inRect(event, v1)) {
                    mainAudioPipe_.editObject(0);
                    draw();
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                if (inRect(event, v1)) {
                    float val = (float)(event.motion.y - v1.mainRect.y) / (float)v1.mainRect.h;
                    if (val >= 0.0 && val <= 1.0) {
                        v1.setValue(val);
                        //v1.draw(renderer);
                        //value01_ = val;
                        draw();   
                        mainAudioPipe_.setCv((double)(1 - val));
                    }
                }

                if (inRect(event, v2)) {
                    float val = (float)(event.motion.y - v2.mainRect.y) / (float)v2.mainRect.h;
                    if (val >= 0.0 && val <= 1.0) {
                        v2.setValue(val);
                        //v2.draw(renderer);
                        draw();   
                    }
                }

                if (inRect(event, v3)) {
                    float val = (float)(event.motion.y - v3.mainRect.y) / (float)v3.mainRect.h;
                    if (val >= 0.0 && val <= 1.0) {
                        v3.setValue(val);
                        //v3.draw(renderer);
                        draw();   
                    }
                }

                if (inRect(event, v4)) {
                    float val = (float)(event.motion.y - v4.mainRect.y) / (float)v4.mainRect.h;
                    if (val >= 0.0 && val <= 1.0) {
                        v4.setValue(val);
                        //v4.draw(renderer);
                        draw();   
                    }
                }

                if (inRect(event, v5)) {
                    float val = (float)(event.motion.y - v5.mainRect.y) / (float)v5.mainRect.h;
                    if (val >= 0.0 && val <= 1.0) {
                        v5.setValue(val);
                        //v5.draw(renderer);
                        draw();   
                    }
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym)
                {
                    //if(SDLK_LEFT)  lp->setCutoffHz(9999.0); std::cout << SDL_GetKeyName(event.key.keysym.sym);
                    //if(SDLK_RIGHT) lp->setCutoffHz(100.0); std::cout << SDL_GetKeyName(event.key.keysym.sym);
                    if (SDLK_k == event.key.keysym.sym) mainAudioPipe_.editObject(0);
                    if (SDLK_o == event.key.keysym.sym) mainAudioPipe_.editObject(1);
                    if (SDLK_f == event.key.keysym.sym) mainAudioPipe_.editObject(2);
                    if (SDLK_s == event.key.keysym.sym) mainAudioPipe_.editObject(3);
                    if (SDLK_p == event.key.keysym.sym) mainAudioPipe_.drawMap();
                    if (SDLK_UP == event.key.keysym.sym) speed += 0.01;
                    if (SDLK_DOWN == event.key.keysym.sym) speed -= 0.01;
                    if (SDLK_q == event.key.keysym.sym) running = SDL_FALSE;
                    // keyboard
                    if (SDLK_z == event.key.keysym.sym) mainAudioPipe_.sendNote(60);
                    if (SDLK_x == event.key.keysym.sym) mainAudioPipe_.sendNote(61);
                    if (SDLK_c == event.key.keysym.sym) mainAudioPipe_.sendNote(62);
                    if (SDLK_v == event.key.keysym.sym) mainAudioPipe_.sendNote(63);
                    if (SDLK_b == event.key.keysym.sym) mainAudioPipe_.sendNote(64);
                    if (SDLK_n == event.key.keysym.sym) mainAudioPipe_.sendNote(65);
                    if (SDLK_m == event.key.keysym.sym) mainAudioPipe_.sendNote(66);
                }
            }

            if (event.type == SDL_QUIT) {
                running = SDL_FALSE;
            }
        }
        
        SDL_Delay(mainAudioLead * syncCompensationFactor);
    }
    cleanUp();

    return 0;
}
