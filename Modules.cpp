// a wraper to modulerize summer synth


#pragma once

#include "Filter.cpp"
#include "Osc.cpp"
#include "StepSequencer.cpp"

using namespace std;

class SynthBaseClass 
{
protected:
    unsigned int id_ = 0;
public:
    SynthBaseClass(){}
    virtual ~SynthBaseClass() = default;
    virtual void drawIcon() = 0;
    virtual void NameAndID() = 0;
    virtual void getOutput(double& z0) = 0;
    virtual void setCv(double& cv1){}
    virtual double getCv(){ return 0.0;}
};

class Osc : public SynthBaseClass {
private:
    FMOscillator carrier_ = FMOscillator();
    FMOscillator modulator_ = FMOscillator();
    WTOscillator lfo_ = WTOscillator(TRIANGLE, 1.0);

    double freq_ = 55.0;
    double modIndex = 1.0;
    double z1_ = 0.0;
public:
    Osc(unsigned int id) {
        id_ = id;
        carrier_.setFrequencyHz(freq_);
        modulator_.setFrequencyHz(freq_ * 2.0);
        lfo_.setFrequency(0.01);
    }
    void setFrequencyHz(float freq) {
        carrier_.setFrequencyHz(freq);
        modulator_.setFrequencyHz(freq * 2.0);
    }
    ~Osc() {}
    void drawIcon() override {
        cout << "┌─┴─┴─┐" << endl;
        cout << "│ osc │" << endl;
        cout << "└─┬───┘" << endl;
    }
    void NameAndID() override {
        cout << "seq" << "     " << id_ << endl;
    }
    void getOutput(double& z0) override {
        //carrier_.setFrequency(freq_ + (      (2.0+(lfo_.getOutput()*0.5)) * modulator_.getOutput() ) );
        z0 += carrier_.getOutput(modulator_.getOutput(0.0)*lfo_.getOutput());
        z1_ = z0;
    }
    void setCv(double& cv1) override
    {
        //freq_ = cv1;
        //carrier_.setFrequencyHz(freq_);
        //modulator_.setFrequencyHz(freq_ * 2.0);
        modulator_.setRatio(cv1);
    }
};


class Lpf : public SynthBaseClass {
private:
    Ms20LowPass lpf = Ms20LowPass(1000.0);
public:
    Lpf(unsigned int id) {
        lpf.setResonance(0.5);
    }
    ~Lpf() {}
    void drawIcon() override {
        cout << "┌─┴─┴─┐" << endl;
        cout << "│ Lpf │" << endl;
        cout << "└─┬───┘" << endl;
    }
    void NameAndID() override {
        cout << "Lpf" << "     " << id_ << endl;
    }
    void getOutput(double& z0) override {
        //carrier_.setFrequency(freq_ + (      (2.0+(lfo_.getOutput()*0.5)) * modulator_.getOutput() ) );
        z0 = lpf.getOutput(z0);
    }
    void setCv(double& cv1) override
    {
        std::cout << cv1 << std::endl;
        lpf.setCutoffHz(cv1);
    }
};

class Seq : public SynthBaseClass
{
private:
    StepSequencer seq_ = StepSequencer(16, 120.0);
public:
    Seq(unsigned int id){
        seq_.setStepValue(1.0, 1-1);
        seq_.setStepValue(1.0, 2-1);
        seq_.setStepValue(1.0, 3-1);
        seq_.setStepValue(1.0, 4-1);

        seq_.setStepValue(2.0, 5-1);
        seq_.setStepValue(2.0, 6-1);
        seq_.setStepValue(3.0, 7-1);
        seq_.setStepValue(3.0, 8-1);

        seq_.setStepValue(4.0, 9-1);
        seq_.setStepValue(4.0, 10-1);
        seq_.setStepValue(5.0, 11-1);
        seq_.setStepValue(5.0, 12-1);

        seq_.setStepValue(6.0, 13-1);
        seq_.setStepValue(7.0, 14-1);
        seq_.setStepValue(8.0, 15-1);
        seq_.setStepValue(9.0, 16-1);

        seq_.setPlaying(true);
    }
    ~Seq(){}
    void drawIcon() override {
        cout << "┌─────┐" << endl;
        cout << "│ seq │" << endl;
        cout << "└───┬─┘" << endl;
    }
    void NameAndID() override {
        cout << "Lpf" << "     " << id_ << endl;
    }
    void getOutput(double& z0) override
    {
        
    }
    void setCv(double& cv1) override
    {
        seq_.updateMetronome();
        cv1 = seq_.getOutput();
    }
};

class amp : public SynthBaseClass
{
private:
public:
    void getOutput(double& z0) override
    {

    }
    void setCv(double& cv1) override
    {
        
    }
};