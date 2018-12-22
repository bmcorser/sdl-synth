#ifndef Oscillator_cpp
#define Oscillator_cpp

#include <iostream>
#include <random>
#include "Filter.cpp"
// #include "Line.cpp"

#define EXTRACT_BITS(the_val, bits_start, bits_len) ((the_val >> (bits_start-1)) & ((1 << bits_len) -1))

#define MAX_FREQ 7350.0
#define MIN_FREQ 0.1

//#define M_TWO_DEV_PI 0.6366197724
//#define M_TWO_PI 6.2831853072

static const double M_TWO_PI = 2 * M_PI;
static const double M_TWO_DEV_PI = 2 / M_PI;
enum WAVETYPE {
    SAWTOOTH,
    SQUARE,
    TRIANGLE,
    NOISE,
    SINE
};

class Oscillator
{
public:

   Oscillator(double frequencyHz = 440.0, WAVETYPE waveType = TRIANGLE, double amp = 1.0) { //
       frequency_ = frequencyHz;
       waveType_ = waveType;
       amplitude_ = amp;

       if (waveType_ == TRIANGLE) {
           frequencyRadians_ = M_TWO_PI / sampleRate_;
           phaseIncrement_ = frequencyRadians_ * frequencyHz;
           phase_ = 0;
       }
       if (waveType_ == SAWTOOTH) {
       }
   }

   virtual double getOutput() {
       if (waveType_ == TRIANGLE) {
           double triValue = (phase_ * M_TWO_DEV_PI);
           if (phase_ < 0) {
               triValue = 1.0 + triValue;
           } else {
               triValue = 1.0 - triValue;
           }
           if ((phase_ += phaseIncrement_) >= M_PI) {
               phase_ -= M_TWO_PI;
           }
           return triValue * amplitude_;
       }

       if (waveType_ == SAWTOOTH) {
           if (phase_ >= 1.0) phase_ -= 1.0;
           double sample = 2.0 * phase_ -1.0;
           phase_ += phaseIncrement_;
           return sample * amplitude_;
       }
       return 0;
   }

   void setFrequency(double frequencyHz) {

       frequency_ = frequencyHz;

       frequencyRadians_ = M_TWO_PI / sampleRate_;
       phaseIncrement_ = frequencyRadians_ * frequencyHz;

   }
   void setWaveform(WAVETYPE waveType) {
       waveType_ = waveType;

   }
   double getFrequency() {
       return frequency_;
   }
   void setAmp(double amp) {
       amplitude_ = amp;
   }
   double getAmp() {
       return amplitude_;
   }
   void setPhase(double phase) {
       phase_ = phase;
   }

   void setSampleRate(double sampleRate) {
       sampleRate_ = sampleRate;
       setFrequency(frequency_);
   }

protected:

   WAVETYPE waveType_;
   double sampleRate_ = 44100.0; // TODO: SampleRate
   double phase_;
   double frequencyRadians_;
   double phaseIncrement_;

   double frequency_ = 1.0;
   double amplitude_ = 1.0;
   double maxAmp_ = 1.0;


};
///////////////////////////////////////////


//class Noise
//{
//public:
//
//    LowPassFilter lowPassFilter_ = LowPassFilter(3000.0);
//    HighPass2PoleFilter HiPassFilter_  = HighPass2PoleFilter(50.0);
//
//    Noise(double hpCutoff, double lpCutoff, double amp) {
//
//        hpCutoff_ = hpCutoff;
//        lpCutoff_ = lpCutoff;
//        amp_ = amp;
//
//        HiPassFilter_.setCutoffHz(lpCutoff_);
//        lowPassFilter_.setCutoffHz(lpCutoff_);
//
//    }
//
//    void setAmp(float amp){
//        amp_ = amp;
//    }
//    double getAmp() {
//        return amp_;
//    }
//
//     double getOutput() {
//         double value = rand();
//         value -= (RAND_MAX/2);
//         value /= (RAND_MAX/2);
//         return (HiPassFilter_.getOutput( lowPassFilter_.getOutput(value) )) * amp_;
//     }
//private:
//    double hpCutoff_, lpCutoff_, amp_;
//};

class PNNoise {
public:
    
    LowPassFilter lowPassFilter_ = LowPassFilter(3000.0);
    HighPass2PoleFilter hiPassFilter_  = HighPass2PoleFilter(50.0);
    
    PNNoise(double hpCutoff, double lpCutoff, double amp) {
        
        hpCutoff_ = hpCutoff;
        lpCutoff_ = lpCutoff;
        amp_ = amp;
        
        hiPassFilter_.setCutoffHz(lpCutoff_);
        lowPassFilter_.setCutoffHz(lpCutoff_);
        
    }
    ~PNNoise(){}
    
    void reset() {
        hiPassFilter_.reset();
        lowPassFilter_.reset();
    }
    
    double getOutput() {
        
        uint32_t b0 = EXTRACT_BITS(registerNum, 1, 1);
        uint32_t b1 = EXTRACT_BITS(registerNum, 2, 1);
        uint32_t b27 = EXTRACT_BITS(registerNum, 28, 1);
        uint32_t b28 = EXTRACT_BITS(registerNum, 29, 1);
        
        uint32_t b31 = b0^b1^b27^b28;
        
        if (b31 == 1) b31 = 0x10000000;
        
        registerNum >>= 1;
        
        registerNum |= b31;
        
        float out = (float)(registerNum)/((pow((float)2.0, (float)32.0))/16.0);
        
        out -= 1.0;
        
        
        return (hiPassFilter_.getOutput( lowPassFilter_.getOutput((double)out) )) * amp_;
        //return hiPassFilter_.getOutput(out) * amp_;
    }
    void setAmp(double amp){
        amp_ = amp;
    }
    double getAmp() {
        return amp_;
    }
    
    
private:
    double hpCutoff_, lpCutoff_, amp_;
    uint32_t registerNum = 3; //
};



//class OscSquare : public Oscillator
//{
//public:
//    void setPulseWidth(double pulsewidth) {
//        if (pulsewidth >= 100.0) pulsewidth = 100.0;
//        if (pulsewidth <= 0.0) pulsewidth = 0.0;
//        pulsewidth_ = pulsewidth;
//    }
//    double getOutput() {
//        if (phase_ >= 1.0) phase_ -= 1.0;
//        double sample = phase_ > pulsewidth_ / 100.0 ? - 1.0 : + 1.0;
//        phase_ += phaseIncrement_;
//        return sample * amplitude_;
//    }
//private:
//    double pulsewidth_ = 50.0;
//};



class WTOscillator {
public:
    WTOscillator(WAVETYPE waveType = SINE, double amplitude = 1.0){
        
        amplitude_ = amplitude;
        
        waveType_ = waveType;
        
        double pi_x_2 = 2.0 * M_PI;
        
        switch (waveType_) {
            case SINE:
                for (int i = 0; i < 1024; i++) {
                    sinArray_[i] = (double)sinf( ((float)i/1024.0) * pi_x_2 );
                }
                break;
            case TRIANGLE:
                for (int i = 0; i < 1024; i++) {
                    double j = (double)i;
                    triArray_[i] =  1.6894*(0.5*((double)sinf( (j/1024.0) * pi_x_2 )) +
                                            (-0.0556)*((double)sinf(3.0 * (j/1024.0) * pi_x_2 )) +
                                            0.02*((double)sinf(5.0 * (j/1024.0) * pi_x_2 )) +
                                            (-0.0102)*((double)sinf(7.0 * (j/1024.0) * pi_x_2 )) +
                                            0.00615*((double)sinf(11.0 * (j/1024.0) * pi_x_2 )));
                }
                break;
            case SAWTOOTH:
                for (int i = 0; i < 1024; i++) {
                    double j = (double)i;
                    sawArray_[i] =  0.49454*(1.0*((double)sinf( (j/1024.0) * pi_x_2 )) +
                                            (0.5)*((double)sinf(2.0 * (j/1024.0) * pi_x_2 )) +
                                            0.333333*((double)sinf(3.0 * (j/1024.0) * pi_x_2 )) +
                                            (0.25)*((double)sinf(4.0 * (j/1024.0) * pi_x_2 )) +
                                            0.2*((double)sinf(5.0 * (j/1024.0) * pi_x_2 )) +
                                            (0.166667)*((double)sinf(6.0 * (j/1024.0) * pi_x_2 )) +
                                            0.142857*((double)sinf(7.0 * (j/1024.0) * pi_x_2 )) +
                                            (0.125)*((double)sinf(8.0 * (j/1024.0) * pi_x_2 )) +
                                            0.111111*((double)sinf(9.0 * (j/1024.0) * pi_x_2 ))
                                            
                                            
                                            );
                }
                break;
                
            default:
                break;
        }
        
        
        cookFrequency();
    }
    ~WTOscillator(){}
    
    void setFrequency(double frequency_Hz) {
        // clamp freq to 22050 11025
        if (frequency_Hz > MAX_FREQ) frequency_Hz = MAX_FREQ;
        if (frequency_Hz < MIN_FREQ) frequency_Hz = MIN_FREQ;
        frequency_Hz_ = frequency_Hz;
        cookFrequency();
    }
    double getFrequency() {
        return frequency_Hz_;
    }
    void setAmp(double amp) {
        amplitude_ = amp;
    }
    double getAmp() {
        return amplitude_;
    }
    
    
    double getOutput(void) {
        
        // our output value for this cycle
        double outSample = 0;
        
        // get INT part
        int readIndex = (int)readIndex_;
        
        // get FRAC part ñ NOTE no Quad Phase frac is needed because it will be the same!
        double frac = readIndex_ - readIndex;
        
        // setup second index for interpolation; wrap the buffer if needed
        int readIndexNext = readIndex + 1 > 1023 ? 0 :  readIndex + 1;
        
        // interpolate the output
        
        
        switch (waveType_) {
            case SINE:
                outSample = linTerp(0, 1, sinArray_[readIndex], sinArray_[readIndexNext], frac);
                break;
            case TRIANGLE:
                outSample = linTerp(0, 1, triArray_[readIndex], triArray_[readIndexNext], frac);
                break;
            case SAWTOOTH:
                outSample = linTerp(0, 1, sawArray_[readIndex], sawArray_[readIndexNext], frac);
                break;
                
            default:
                outSample = 0.0;
                break;
        }
        
        //outSample = linTerp(0, 1, sinArray_[readIndex], sinArray_[readIndexNext], frac);
        //outSample = sineArray_[readIndex];
        
        
        // add the increment for next time
        readIndex_ += inc_;
        
        // check for wrap
        if(readIndex_ >= 1024.0)
            readIndex_ = readIndex_ - 1024.0;
        
        
        return outSample * amplitude_;
        
    }
    
    void reset() {readIndex_ = 0.0;}
    
    void setSampleRate(double sampleRate) {
        sampleRate_ = sampleRate;
        cookFrequency();
    }
    
    void setPhase(double phase) {
        readIndex_ = 1024.0 * phase;
    }
private:
    WAVETYPE waveType_;
    double sinArray_[1024];
    double triArray_[1024];
    double sawArray_[1024];
    double readIndex_ = 0.0;
    double sampleRate_ = 44100.0;
    
    
    double inc_;
    
    double frequency_Hz_ = 440.0;
    double amplitude_ = 1.0;
    
    
    void cookFrequency() {
        inc_ = 1024.0 * frequency_Hz_ / sampleRate_;
    }
    
    inline double linTerp(double x1, double x2, double y1, double y2, double x)
    {
        double denom = x2 - x1;
        if(denom == 0)
            return y1; // should not ever happen
        
        // calculate decimal position of x
        double dx = (x - x1)/(x2 - x1);
        
        // use weighted sum method of interpolating
        double result = dx*y2 + (1-dx)*y1;
        
        return result;
        
    }
    
};

class FMOscillator
{
public:

   double ratio_ = 1.0,
          index_ = 1.0,
          feedbackIndex_ = 0.0,
          frequencyHz_ = 220.0,
          previousSample_ = 0.0;

   FMOscillator() {
       phasor_ = WTOscillator(SAWTOOTH, 1.0);
       //modulator_ = Oscillator(440.0, SAWTOOTH, 1.0);
       //modulator_.setWaveform(WAVETYPE::TRIANGLE);

       setFrequencyHz(frequencyHz_);
   }
   ~FMOscillator() {

   }


   void setFeedbackIndex(double feedbackIndex) {
       feedbackIndex_ = feedbackIndex;
   }

   void setFrequencyHz(double frequencyHz) {
       frequencyHz_ = frequencyHz;
       phasor_.setFrequency(frequencyHz * ratio_);
       //modulator_.setFrequency(frequencyHz * ratio_);
   }


   void setRatio(double ratio) {
       ratio_ = ratio;
       phasor_.setFrequency(frequencyHz_ * ratio_);
   }


   double getOutput(double modulator) {
       //double modulation = modulator_.getOutput();
       double modulationAmount = (index_ * modulator);
       double feedbackAmount = feedbackIndex_ * previousSample_;
    //    phasor_.setFrequency(
    //        frequencyHz_ + modulationAmount + feedbackAmount
    //    );
       double phasor = phasor_.getOutput();
       double carrierOut = sin(phasor + modulationAmount + feedbackAmount);
       previousSample_ = carrierOut;
       return carrierOut;
   }


private:
   WTOscillator phasor_;
   //Oscillator modulator_;
};






#endif /* Oscillator_cpp */

