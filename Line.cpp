//
//  Line.cpp
//  AUInstrument
//
//  Created by Paul Betowski on 20/09/2017 @ 4.00 in the morning.
//

#ifndef Line_cpp
#define Line_cpp

#include <stdio.h>


class Line {
    
    /*  I just want to get from A to B.
     *
     *      x [A]
     *       \
     *        \
     *         \
     *          \
     *           \
     *            x [B]
     *      |_____|
     *      [speed]
     *
    */
    
    /*  if A == B Line works as a timer.
     *  will out puts A || B for "speed" amount of time
     *
     *
     *
     *
     */
    
public:
    
    Line(double start, double end, double speed) {
        A_ = start;
        B_ = end;
        setSpeed(speed);
    }
    
    ~Line() {}
    
    void setLine(double start, double end, double speed) {
        A_ = start;
        B_ = end;
        setSpeed(speed);
    }

    
    bool isDone(void) {
        return done_;
    }
    
    
    void setStart(double start) {
        A_ = start;
        calculateIncSpeed();
    }
    double getStart() {return A_;}
    
    void setEnd(double end) {
        B_ = end;
        calculateIncSpeed();
    }
    double getEnd() {return B_;}
    
    
    
    
    void setSpeed(double speedMs) {
        speedMsSave_ = speedMs;
        speedMS_ = (sampleRate_/1000.0) * speedMs;
        calculateIncSpeed();
    }
    double getSpeed() {
        return speedMsSave_;
    }
    
    void stop() {
        
    }
    

    
    void bang() {
        // load any new values from cached
        A_o_ = A_;
        B_o_ = B_;
        
        if (A_o_ == B_o_) {
            // set work as timer
            workAsTimer_ = true;
            A_o_ += 100.0;
        } else {
            workAsTimer_ = false;
        }
        
        calculateIncSpeed();
        incSpeed_o_ = incSpeed_;
        
        currentSample_ = A_o_;
        done_ = false;
    }
    
    
    
    double getOutput() {
        
        if (A_o_ > B_o_) {
            if (currentSample_ <= B_o_ ) done_ = true;
        } else {
            if (currentSample_ >= B_o_ ) done_ = true;
        }
        
        
        if (!done_) {
            currentSample_ -= incSpeed_o_;
        } else {
            currentSample_ = B_o_;
        }
        
        
        if (!workAsTimer_) {
            return currentSample_;
        } else {
            return B_o_; // work as timer
        }
    }
    
    void setSampleRate(double sampleRate) {
        sampleRate_ = sampleRate;
    }
    
private:
    bool done_ = false;
    bool workAsTimer_ = false;
    double A_= 0.0, B_= 0.0, incSpeed_= 0.0, speedMS_= 0.0, currentSample_ = 0.0;
    double A_o_= 0.0, B_o_= 0.0, incSpeed_o_ = 0.0; // used in output. this is so cashed values can be loaded in on bang
    double speedMsSave_ = 0.0; // for save and load in au
    
    double sampleRate_ = 44100.0;
    
    void calculateIncSpeed() {
            incSpeed_ = (A_o_ - B_o_) / speedMS_;
    }

    
};

#endif /* Line_cpp */
