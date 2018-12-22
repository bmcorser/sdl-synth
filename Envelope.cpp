//#ifndef Envelope_cpp
//#define Envelope_cpp
//
//#include <math.h>
//
//class Envelope {
//public:
//    Envelope(double peak, double a, double d, double s, double r) {
//        // TODO: SampleRate
//        sampleRate_ = 44100.0;
//        setAttackMs(a);
//        setDecayMs(d);
//        setReleaseMs(r);
//        setPeakAmp(peak);
//        setSustainAmp(s);
//    };
//    void setAttackMs(double ms) {
//        attackSamples_ = millisecondsToSamples(ms);
//    }
//    void setDecayMs(double ms) {
//        decaySamples_ = millisecondsToSamples(ms);
//    }
//    void setReleaseMs(double ms) {
//        releaseSamples_ = millisecondsToSamples(ms);
//    }
//    void setPeakAmp(double amp) {
//        peakAmp_ = amp;
//    }
//    void setSustainAmp(double amp) {
//        sustainAmp_ = amp;
//    }
//    void setNoteOn() {
//        noteOn_ = true;
//    }
//    void setNoteOff() {
//        noteOn_ = false;
//    }
//    bool getNoteOn() {
//        return noteOn_;
//    }
//    void reset() {
//        state_ = O;
//        attackPosition_ = decayPosition_ = releasePosition_ = currentAmp_ = 0;
//
//    }
//
//    double getOutput() {
//        if (noteOn_) {
//            switch (state_) {
//                case O:
//                    // URHO3D_LOGINFO("attack");
//                    state_ = A;
//                    difference_ = fabs(currentAmp_ - peakAmp_);
//                    startAmp_ = currentAmp_;
//                case A:
//                    // attack
//                    // URHO3D_LOGINFO(String("attack ") + String(attackPosition_) + String(" / ") + String(attackSamples_));
//                    if (attackPosition_ == attackSamples_) {
//                        // URHO3D_LOGINFO("decay");
//                        state_ = D;
//                        decayPosition_ = 0;
//                        difference_ = fabs(currentAmp_ - sustainAmp_);
//                        startAmp_ = currentAmp_;
//                    } else {
//                        proportion_ = (float)attackPosition_ / (float)attackSamples_;
//                        currentAmp_ = startAmp_ + (proportion_ * difference_);
//                        // if (attackPosition_ % 100 == 0) URHO3D_LOGINFO(String(currentAmp_));
//                        attackPosition_++;
//                        return currentAmp_;
//                    }
//                case D:
//                    // decay
//                    // URHO3D_LOGINFO(String("decay ") + String(decayPosition_) + String(" / ") + String(decaySamples_));
//                    if (decayPosition_ == decaySamples_) {
//                        // URHO3D_LOGINFO("sustain");
//                        state_ = S;
//                    } else {
//                        proportion_ = (float)decayPosition_ / (float)decaySamples_;
//                        currentAmp_ = startAmp_ - (proportion_ * difference_);
//                        // if (decayPosition_ % 100 == 0) URHO3D_LOGINFO(String(currentAmp_));
//                        decayPosition_++;
//                        return currentAmp_;
//                    }
//                case S:
//                    return currentAmp_;
//                case R:
//                    // do nothing. silence warning
//                    break;
//            }
//        } else {
//            attackPosition_ = 0;
//            switch (state_) {
//                case O:
//                    return 0;
//                case A:
//                case D:
//                case S:
//                    // URHO3D_LOGINFO("release");
//                    state_ = R;
//                    releasePosition_ = 0;
//                    startAmp_ = currentAmp_;
//                case R:
//                    // URHO3D_LOGINFO(String("release ") + String(releasePosition_) + String(" / ") + String(releaseSamples_));
//                    if (releasePosition_ == releaseSamples_) {
//                        // URHO3D_LOGINFO("off");
//                        state_ = O;
//                    } else {
//                        proportion_ = (float)releasePosition_ / (float)releaseSamples_;
//                        currentAmp_ = startAmp_ - (proportion_ * startAmp_);
//                        // if (releasePosition_ % 100 == 0) URHO3D_LOGINFO(String(currentAmp_));
//                        releasePosition_++;
//                        return currentAmp_;
//                    }
//            }
//        }
//        return 0;
//    }
//    void setSampleRate(double sampleRate) {
//        sampleRate_ = sampleRate;
//    }
//
//private:
//    int millisecondsToSamples(double ms) {
//        return ((float)sampleRate_ / 1000.0) * ms;
//    }
//    int attackSamples_, decaySamples_, releaseSamples_;
//    int attackPosition_ = 0, decayPosition_ = 0, releasePosition_ = 0;
//    double peakAmp_ = 0.0, sustainAmp_ = 0.0, currentAmp_ = 0.0;
//    double proportion_, difference_, startAmp_;
//    unsigned int sampleRate_;
//    bool noteOn_;
//    enum State {A, S, D, R, O};
//    State state_ = O;
//};
//
//
//#endif /* Envelope_cpp */
