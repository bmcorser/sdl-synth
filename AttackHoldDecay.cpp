//
//  AttackHoldDecay.cpp
//  WaveSynthFramework
//
//  Created by Paul Betowski on 07/11/2017.
//  Copyright © 2017 Paul Betowski. All rights reserved.
//



#include "AttackHoldDecay.hpp"


AttackHoldDecay::AttackHoldDecay(double attackMS, double holdMS, double decayMS) {
    setAttack(attackMS);
    setHold(holdMS);
    setDecay(decayMS);
}
AttackHoldDecay::~AttackHoldDecay(){
}

void AttackHoldDecay::setAttackHoldDecay(double attackMS, double holdMS, double decayMS) {
    setAttack(attackMS);
    setHold(holdMS);
    setDecay(decayMS);
}

double AttackHoldDecay::getOutput() {
    
    //double temp = 0.0;
    
    if (started_) {
        
        if (currentPhase_ == PREATTACK) {
            output_ = line_.getOutput();
            if (line_.isDone() ) { //|| bypassPreAttack
                //bypassPreAttack = false;
                atNotebegainFlag = true;
                currentPhase_ = ATTACK;
                line_.setLine(0.0, 1.0, attack_);
                line_.bang();
            }
        }
        if (currentPhase_ == ATTACK) {
            output_ = line_.getOutput();
            if (line_.isDone()) {
                currentPhase_ = HOLD;
                line_.setLine(1.0, 1.0, hold_);
                line_.bang();
            }
        }
        if (currentPhase_ == HOLD) {
            output_ = line_.getOutput();
            if (line_.isDone()) {
                currentPhase_ = DECAY;
                line_.setLine(1.0, 0.0, decay_);
                line_.bang();
            }
        }
        if (currentPhase_ == DECAY) {
            // analog decay
            //    curve     flip   offset to 0    make up to 1.0
            //      V        V          V         V
            // ((e^-4.95 * (1-x)) - 0.007083) * 1.0071
            
            output_ = 1.0071 * ((pow(2.7182818285, -4.95 * (1.0-line_.getOutput() ) ) ) - 0.007083);
            //output_ = line_.getOutput();
            if (line_.isDone()) {
                currentPhase_ = FINISHED;
            }
        }
        if (currentPhase_ == FINISHED) {
            output_ = 0.0;
            noteFinished_ = true;
        }
    }

    //output_ = clamp(output_, 0.0, 1.0);
    return output_;
}

void AttackHoldDecay::bang(void){
    reset();
    //line_.setLine(getOutput(), 0.0, 4.0);
    line_.setLine(output_, 0.0, 4.0);
    started_ = true;
    line_.bang();
    
}
void AttackHoldDecay::setAttack(double attackMS){
    attack_ = attackMS;
    if (currentPhase_ == ATTACK) line_.setSpeed(attack_);
}
void AttackHoldDecay::setHold(double holdMS){
    hold_ = holdMS;
    if (currentPhase_ == DECAY) line_.setSpeed(hold_);
}
void AttackHoldDecay::setDecay(double decayMS){
    decay_ = decayMS;
    if (currentPhase_ == DECAY) line_.setSpeed(decay_);
}

void AttackHoldDecay::setAtNotebeginFlag(bool flag) {
    atNotebegainFlag = flag;
}

bool AttackHoldDecay::getAtNotebeginFlag() {
    return atNotebegainFlag;
}
bool AttackHoldDecay::getAtNoteFinishedFlag() { return noteFinished_; }


double AttackHoldDecay::getAttack() {return attack_;}
double AttackHoldDecay::getHold() {return hold_;}
double AttackHoldDecay::getDecay() {return decay_;}




void AttackHoldDecay::reset() {
    noteFinished_ = false;
    started_ = false;
    currentPhase_ = PREATTACK;
}





