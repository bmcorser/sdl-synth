//
//  AttackHoldDecay.hpp
//  WaveSynthFramework
//
//  Created by Paul Betowski on 07/11/2017.
//

#ifndef AttackHoldDecay_hpp
#define AttackHoldDecay_hpp

#include <stdio.h>
#include "Line.cpp"
#include <cmath>

enum CURRENT_PHASES {
    PREATTACK   = 0,
    ATTACK      = 1,
    HOLD        = 2,
    DECAY       = 3,
    FINISHED    = 4,
};


class AttackHoldDecay {
 
public:
    
    Line line_ = Line(0.0, 0.0, 5.0);
    
    AttackHoldDecay(double attackMS, double holdMS, double decayMS);
    ~AttackHoldDecay();
    
    double getOutput(void);
    void setAttackHoldDecay(double attackMS, double holdMS, double decayMS);
    void bang(void);
    
    void setAttack(double attackMS);
    void setHold(double holdMS);
    void setDecay(double decayMS);
    double getAttack();
    double getHold();
    double getDecay();
    
    void setAtNotebeginFlag(bool flag);
    bool getAtNotebeginFlag();
    bool getAtNoteFinishedFlag();
    
    void reset(void);
    
    /*
     amp envelopes maybe retrigered when the envelope is running resulting in
     jumping from the current amp to 0. Clicking is mittigated by ramping
     down to 0 befor starting the attack.
     setAtNotebeginFlag and getAtNotebeginFlag should be used :
     if (line_A_.getAtNotebeginFlag() == true) {
     osc_.setPhase(0.25);
     osc_modulator_.setPhase(0.25);
     line_P_.bang();
     line_D_.bang();
     line_C_.bang();
     line_A_.setAtNotebeginFlag(false);
     }
     */
   
    
    
private:
    double attack_ = 100.0, hold_ = 100.0, decay_ = 100.0;
    double inc_ = 0.0;
    double currentValue_ = 0.0;
    bool started_ = false;
    bool atNotebegainFlag = false;
    bool analogEnvelopes_ = true;
    double output_ = 0.0;
    bool noteFinished_ = false;
    
    CURRENT_PHASES currentPhase_ = PREATTACK;
    
    
    
    
    
    
    
    static inline double clamp(double value, double min, double max) {
        if (value >= min && value <= max) {
            return value;
        } else if (value <= min) {
            return min;
        } else if (value >= max) {
            return max;
        } else {
            return 0.0;
        }
    }
    
    
    
};






#endif /* AttackHoldDecay_hpp */
