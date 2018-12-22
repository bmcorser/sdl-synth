#pragma once

#include <vector>
#include "Modules.cpp"

float midiToFreq(int x)
{
   return (440 / 32) * (pow(2, (((float)x - 9) / 12)));
}

class MainAudioPipe 
{
private:
    vector<SynthBaseClass*> v;
    
public:
    void editObject(unsigned int i){
        if (i == 0)
        {
            if (v.size() > 0) 
            {
                delete v.back();
                v.pop_back();
            }
        } 
        if (i == 1) v.push_back(new Osc(v.size()+1));
        if (i == 2) v.push_back(new Lpf(v.size()+1));
        if (i == 3) v.push_back(new Seq(v.size()+1));
    }
    void drawMap() {
        for (int i = 0; i < v.size(); i++) { 
            v[i]->drawIcon();
        }
    }
    double getOutput() 
    {
        double stream = 0.0; 
        double cvStream = 0.0; 
        for (int i = 0; i < v.size(); i++) { 
            // v[i]->setCv(cvStream);
            v[i]->getOutput(stream);
        }
        // clip
        if (stream > 1.0) stream = 1.0;
        if (stream < -1.0) stream = -1.0;
        return stream;
    }
    void sendNote(int note) {
        for (SynthBaseClass* component: v) {
            Osc* osc = dynamic_cast<Osc*>(component);
            if (osc != nullptr) {
                float hz = midiToFreq(note);
                // std::cout << hz << std::endl;
                osc->setFrequencyHz(hz);
            }
        }
    }

    void setCv(double val) {
        for (SynthBaseClass* component: v) {
            Lpf* filter = dynamic_cast<Lpf*>(component);
            if (filter != nullptr) {
                double value = val * 9000;
                filter->setCv(value);
            }
        }
    }
};





// consepts 
// note on
// seq();  [value    bang]
// adsr(double &z0, bool bang);
// adsr(); [in       trig]
//         [audio      cv]
//         [out       out]