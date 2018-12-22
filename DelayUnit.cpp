#ifndef DelayUnit_cpp
#define DelayUnit_cpp

#include "Filter.cpp"
#include "SoftClip.cpp"

//class DelayUnit
//{
//public:
//    DelayUnit(int delayLengthMs = 500, float feedback = 0.3) {
//        counter_ = 0;
//        setDelayLengthMs(delayLengthMs);
//        setFeedback(feedback);
//        for (int i = 0; i < maxDelayLengthSamples_; ++i) {
//            delayBuffer_[i] = 0.0;
//        }
//        hpf.setCutoffHz(600.0);
//        lpf.setCutoffHz(1000.0);
//        //lpf->setResonance(2.5);
//        clipper.setPregain(1.0);
//        clipper.setTone(1.0);
//    }
//    ~DelayUnit() {
//    }
//
//    double getOutput(double input) {
//        ++counter_;
//        if (counter_ >= delayLengthSamples_) {
//            counter_ = 0;
//        }
//        double output = delayBuffer_[counter_];
//        delayBuffer_[counter_] = input + lpf.getOutput( hpf.getOutput(feedback_ * clipper.getOutput(output)) );
//        //delayBuffer_[counter_] = input + (feedback_ * output );
//        return output;
//    }
//
//    void setDelayLengthMs(float ms) {
//        ms_ = ms;
//        int samples = (int)(ms * (sampleRate_ / 1000.0));
//        //URHO3D_LOGINFO(String(samples) + " > " + String(maxDelayLengthSamples_));
//        delayLengthSamples_ = samples;
//
//        // fill array with 0s
//
//
//    }
//
//    double getDelayLengthMs() {return ms_;}
//
//    void setFeedback(float feedback) {
//        if (feedback >= 0.9999)
//        {
//            feedback_ = 0.9999;
//        } else {
//            feedback_ = feedback;
//        }
//    }
//    float getFeedback() {
//        return feedback_;
//    }
//
//private:
//    double ms_; // for use in saving and loading au
//    int sampleRate_ = 44100; // TODO: SampleRate
//    long int counter_, delayLengthSamples_;
//    static const long int maxDelayLengthSamples_ = 441000; // TODO: SampleRate
//    float feedback_;
//    float delayBuffer_[maxDelayLengthSamples_];
//    HighPass2PoleFilter hpf = HighPass2PoleFilter();
//    LowPassFilter       lpf = LowPassFilter();
//    SoftClip            clipper = SoftClip();
//
//};











#endif /* DelayUnit_cpp */
