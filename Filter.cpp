#ifndef Filter_cpp
#define Filter_cpp

#include <math.h>
#include <iostream>
#include "SoftClip.cpp"



class BaseFilter {
protected:
    double cutoffHz_ = 1000.0;
    double sampleRate_ = 44100.0;
    
    double resonance_ = 0.1;
public:
    double uniqueParameter_ = 1.0;
    double coefficient_;
    BaseFilter(double cutoffHz = 5000.0){
        cutoffHz_ = cutoffHz;
    }
    virtual ~BaseFilter() = default;
    virtual void kill() = 0;
    virtual void prepCutoffCoefficients(void) = 0;
    virtual void reset(void) = 0;
    virtual double getOutput(double input) = 0;
    virtual void prepResCoefficients() = 0;
    virtual void prepUniqueParameter() = 0;
    void setSampleRate(double sampleRate) {
        reset();
        sampleRate_ = sampleRate;
        setCutoffHz(cutoffHz_);
        
    }
    void setCutoffHz(double cutoffHz) {
        // std::cout << cutoffHz << std::endl;
        cutoffHz_ = cutoffHz;
        prepCutoffCoefficients();
    }
    double getCutoffHz() { return cutoffHz_; }
    
    void setResonance(double res){
        resonance_ = res;
        prepResCoefficients();
    }
    void setUniqueParameter(double value) {
        uniqueParameter_ = value;
        prepUniqueParameter();
    }
    
    
};


/*
 ███████████████████████████████████████████████████████████████████████████████
 ▒ VA first order low pass filter ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
 ███████████████████████████████████████████████████████████████████████████████
 *
 *
 *                       R                            --------------------
 *        V o──────────/\/\/─────────┬────> Vc        //
 *                                   |                //
 *                                  ———               //
 *                                  --- C             //
 *                                   |                //
 *                                   V                //
 *
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 *
 *           [input]   ┌───┐   ┌───┐ [V]  ┌───┐  [Y]    [output]
 *            x(n) ───>│ + ├──>│ G ├──o-─>│ + ├───o────> y(n)
 *                     └─┬─┘   └───┘  │   └─┬─┘   │
 *                       ▲-           │     ▲     │
 *                       │            │     │     │
 *                ┌──────┴──────────────────o[lastSample_]
 *                │                   │     │     │
 *                ▼                   │   ┌─┴─┐   │
 *           [feedback]               │   │z-1│   │
 *                                    │   └─┬─┘   │
 *                                    │     ▲[B]  │
 *                                    │     │     │
 *                                    │   ┌─┴─┐   │
 *                                    └──>│ + │<──┘
 *                                        └───┘
 *
 * ▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂
 */
class LowPassFilter : public BaseFilter
{
public:
    LowPassFilter(double cutoffHz = 5000.0) : BaseFilter(cutoffHz)  {

	}
    void kill() override {std::cout << "killed!" << std::endl; delete this;}

    ~LowPassFilter(){}
    
    void prepUniqueParameter() override {
        
    }
    
    void prepCutoffCoefficients(void) override {
        double cutoffRadians = 2 * M_PI * cutoffHz_;
        double sampleSecs = 1 / sampleRate_;
        double lolwut = (2 / sampleSecs) * tan(cutoffRadians * (sampleSecs / 2)) * (sampleSecs / 2);
        coefficient_ = lolwut / (1.0 + lolwut);
    }

	void reset() override {
		lastSample_ = 0.0;
	}
    
//    void setSampleRate(double sampleRate) {
//        sampleRate_ = sampleRate;
//        setCutoffHz(cutoffHz_);
//    }

	double getOutput(double input) override {
		double V = coefficient_ * (input - lastSample_);
		double Y = V + lastSample_;
		double B = V + Y;
		double output = Y;
		lastSample_ = B;
		return output;
	}
	double getFeedback(){
		return beta_ * lastSample_;
	}
    
    void prepResCoefficients() override {}
    
    double beta_ = 1.0;
protected:
	double lastSample_ = 0.0;
	

};

/*
 ███████████████████████████████████████████████████████████████████████████████
 ▒ VA first order high pass filter ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
 ███████████████████████████████████████████████████████████████████████████████
 *
 *                                    ┌───┐    [output]
 *         ┌─────────────────────────>│ + ├──> y(n)
 *         |                          └─┬─┘
 * [input] | ┌───┐   ┌───┐ [V]  ┌───┐   ▲-
 *  x(n) ──┴>│ + ├──>│ G ├──o-─>│ + ├───o
 *           └─┬─┘   └───┘  │   └─┬─┘   │
 *             ▲-           │     ▲     │
 *             │            │     │     │
 *      ┌──────┴──────────────────o[lastSample_]
 *      │                   │     │     │
 *      ▼                   │   ┌─┴─┐   │
 * [feedback]               │   │z-1│   │
 *                          │   └─┬─┘   │
 *                          │     ▲[B]  │
 *                          │     │     │
 *                          │   ┌─┴─┐   │
 *                          └──>│ + │<──┘
 *                              └───┘
 *
 * ▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂
 */
class HighPassFilter : public LowPassFilter
{
public:
	HighPassFilter(double cutoffHz = 5000.0) : LowPassFilter(cutoffHz) {}

	double getOutput(double input) override {
        float lpfOutput = LowPassFilter::getOutput(input);
		return input - lpfOutput;
	}
};



/*
 ███████████████████████████████████████████████████████████████████████████████
 ▒ LADDER FILTER ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
 ███████████████████████████████████████████████████████████████████████████████
 *
 *                                          z-1
 *                                      ┌───┐
 *           [akCo]   ┌─────────────────┤ K ├─────────────────┐
 *           1+aK     v-                └───┘                 │
 * [input]  ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌─┴─┐     [output]
 *  x(n) ──>│ k ├──>│ + ├──>│ S ├──>│ 1 ├──>│ 2 ├──>│ 3 ├──>│ 4 ├───> y(n)
 *          └───┘   └───┘   └───┘   └───┘   └───┘   └───┘   └───┘
 *                   ir      clp    -----------------------------
 *
 * ▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂
 */

class LadderFilter : public BaseFilter

{
public:
    LadderFilter(double cutoffHz = 200.0) {
        //cutoffHz_ = cutoffHz;
        BaseFilter::setCutoffHz(cutoffHz);
        BaseFilter::setResonance(1.0);
    }
    ~LadderFilter(){
    }
    void kill() override {std::cout << "killed!" << std::endl; delete this;}
    void prepUniqueParameter() override {
        drive_.setTone(uniqueParameter_);
    }
    
    double getOutput(double input) override {
        
        double driven = drive_.getOutput(input);
        
        double ir = (driven * akCo_) - resonanceBuffer_;
        double clip = softclipper_.getOutput(ir);
        
        double filterNetwork =
        lowPassFilter4_.getOutput(
                                  lowPassFilter3_.getOutput(
                                                            lowPassFilter2_.getOutput(
                                                                                      lowPassFilter1_.getOutput(clip)
                                                                                      )
                                                            )
                                  );
        
        //resonanceBuffer_ = filterNetwork * resonance_;
        resonanceBuffer_ = lowPassFilter4_.getFeedback() * resonance_;
        
        return filterNetwork;
    }
    
    void prepCutoffCoefficients() override {

        lowPassFilter1_.setCutoffHz(cutoffHz_);
        lowPassFilter2_.setCutoffHz(cutoffHz_);
        lowPassFilter3_.setCutoffHz(cutoffHz_);
        lowPassFilter4_.setCutoffHz(cutoffHz_);

    }
    
    double getResonance() {
        return resonance_;
    }

    void prepResCoefficients() override {
        akCo_ = 1.0 + 0.2 * resonance_;
    }
    

    void reset(void) override {
        lowPassFilter1_.reset();
        lowPassFilter2_.reset();
        lowPassFilter3_.reset();
        lowPassFilter4_.reset();
        resonanceBuffer_ = 0.0;
    }

    
    SoftClip softclipper_ = SoftClip(1.0, 1.1, 0.0, 1.0);
    SoftClip drive_ = SoftClip(1.0, 1.1, 0.0, 1.0);

private:
    
    LowPassFilter lowPassFilter1_ = LowPassFilter();
    LowPassFilter lowPassFilter2_ = LowPassFilter();
    LowPassFilter lowPassFilter3_ = LowPassFilter();
    LowPassFilter lowPassFilter4_ = LowPassFilter();

    
    double resonanceBuffer_ = 0.0;
    double akCo_;
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - //


class HighPass2PoleFilter : public BaseFilter
{
public:
	HighPass2PoleFilter(double cutoffHz = 10.0) {
		BaseFilter::setCutoffHz(cutoffHz);
	}

    
    void kill() override {std::cout << "killed!" << std::endl; delete this;}
    void prepUniqueParameter() override {}

    void prepResCoefficients() override {}
    void prepCutoffCoefficients(void) override {
        highPassFilter1_.setCutoffHz(cutoffHz_);
        highPassFilter2_.setCutoffHz(cutoffHz_);
        //printf("|yo - %f | ", cutoffHz_);
    }

	void reset() override {
		highPassFilter1_.reset();
		highPassFilter2_.reset();
	}

	double getOutput(double input) override {
		return highPassFilter2_.getOutput(highPassFilter1_.getOutput(input));
	}
private:
	HighPassFilter highPassFilter1_ = HighPassFilter();
	HighPassFilter highPassFilter2_ = HighPassFilter();
};



class LowPass2PoleFilter : public BaseFilter
{
public:
    LowPass2PoleFilter(double cutoffHz = 6000.0) {
        BaseFilter::setCutoffHz(cutoffHz);
    }
    ~LowPass2PoleFilter(){
    }
    void kill() override {std::cout << "killed!" << std::endl; delete this;}
    void prepUniqueParameter() override {}

    
    void prepCutoffCoefficients(void) override {
        lowPassFilter1_.setCutoffHz(cutoffHz_);
        lowPassFilter2_.setCutoffHz(cutoffHz_);
        printf("|yo - %f | ", cutoffHz_);
    }
    

    
    void reset() override {
        lowPassFilter1_.reset();
        lowPassFilter2_.reset();
    }
    
    double getOutput(double input) override {
        return lowPassFilter2_.getOutput(lowPassFilter1_.getOutput(input));
    }
    
private:
    LowPassFilter lowPassFilter1_ = LowPassFilter();
    LowPassFilter lowPassFilter2_ = LowPassFilter();
};

class HighPass4PoleFilter : public BaseFilter
{
public:
    HighPass4PoleFilter(double cutoffHz = 10.0) {
        BaseFilter::setCutoffHz(cutoffHz);
    }
    ~HighPass4PoleFilter(){
    }
    void kill() override {std::cout << "killed!" << std::endl; delete this;}
    void prepUniqueParameter() override {
        
    }
    
    void prepCutoffCoefficients(void) override {
        highPassFilter1_.setCutoffHz(cutoffHz_);
        highPassFilter2_.setCutoffHz(cutoffHz_);
        highPassFilter3_.setCutoffHz(cutoffHz_);
        highPassFilter4_.setCutoffHz(cutoffHz_);
        //printf("|yo - %f | ", cutoffHz_);
    }
    
    void reset() override {
        highPassFilter1_.reset();
        highPassFilter2_.reset();
        highPassFilter3_.reset();
        highPassFilter4_.reset();
    }
    
    double getOutput(double input) override {
        return highPassFilter4_.getOutput(highPassFilter3_.getOutput(highPassFilter2_.getOutput(highPassFilter1_.getOutput(input))));
    }
private:
    HighPassFilter highPassFilter1_ = HighPassFilter();
    HighPassFilter highPassFilter2_ = HighPassFilter();
    HighPassFilter highPassFilter3_ = HighPassFilter();
    HighPassFilter highPassFilter4_ = HighPassFilter();
};

/*
class BandPassFilter
{
public:
    BandPassFilter(double cutoffHz, double q) {
        cutoffHz_ = cutoffHz;
        q_ = q;

        calculatedQForHP_ = cutoffHz_ + q_;
        calculatedQForLP_ = cutoffHz_ - q_;

    }
    void setCutoffHz(double cutoffHz) {
        cutoffHz_ = cutoffHz;
        calculateQ();
        highPassFilter1_.setCutoffHz(calculatedQForHP_);
        lowPassFilter1_.setCutoffHz(calculatedQForLP_);
    }

    void calculateQ() {

        //           calulated Q
        //                |
        // (fre - q)──►|----|◄── (freq + q)
        //             |    |
        //             ▼    ▼
        //  | __________    __________
        //  | lp        ╲  ╱        hp
        //  |            ╲╱
        //  | ___________╱╲___________
        //  --------------------------
        //               ▲
        //                 |
        //              cutoff


        calculatedQForHP_ = cutoffHz_ + q_;
        calculatedQForLP_ = cutoffHz_ - q_;


    }

    void setQ(double q) {
        q_ = q;
        calculateQ();
    }
    double getOutput(double input) {
        return highPassFilter1_.getOutput(
               highPassFilter1_.getOutput(
                lowPassFilter1_.getOutput(
                lowPassFilter1_.getOutput(input) ) ) );
    }

private:
    HighPassFilter highPassFilter1_ = HighPassFilter();
    LowPassFilter  lowPassFilter1_ = LowPassFilter();

    double cutoffHz_;
    double q_;

    double calculatedQForHP_;
    double calculatedQForLP_;

};


class EQNode {
public:
    EQNode(double cutoffHz, double q, double boost) {

    }
    ~EQNode(){}



    void setCutoffHz(double cutoffHz) {
        cutoffHz_ = cutoffHz;
        bp1_.setCutoffHz(cutoffHz_);
        bp2_.setCutoffHz(cutoffHz_);
    }
    void setQ(double q) {
        q_ = q;
        bp1_.setQ(q_);
        bp2_.setQ(q_);
    }
    void setBoost(double boost) {
        boost_ = boost;
    }
    double getOutput(double input) {
        return (boost_ * bp2_.getOutput(bp1_.getOutput(input))) + input;
    }


private:
    double cutoffHz_, q_, boost_ = 1.0;
    BandPassFilter bp1_ = BandPassFilter(1000.0, 2.0);
    BandPassFilter bp2_ = BandPassFilter(1000.0, 2.0);
};

 ███████████████████████████████████████████████████████████████████████████████
 ▒ ParametricEQ (quidratic resonator) ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
 ███████████████████████████████████████████████████████████████████████████████
 *
 *
 * ▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂
 */
class ParametricEQ {
  
public:
    ParametricEQ(double cutoffHz, double q, double boost)  {
        cutoffHz_ = cutoffHz;
        q_ = q;
        boost_ = boost;
        calculateCoefficiants();
    }
    ~ParametricEQ() {}
    
    void reset() {
        z_1 = z_2 = z_3 = z_4 = 0.0;
    }
    void setCutoffHz(double cutoffHz) {
        cutoffHz_ = cutoffHz;
        calculateCoefficiants();
    }
    void setQ(double q) {
        q_ = q;
        calculateCoefficiants();
    }
    void setBoost(double boost) {
        boost_ = boost;
        calculateCoefficiants();
    }
    double getOutput(double input) {
        
        double filterOut =
        (a0 * input)
        + (a1 * z_1)
        + (a2 * z_2)
        - (b1 * z_3)
        - (b2 * z_4);
        z_4 = z_3;
        z_3 = filterOut;
        z_2 = z_1;
        z_1 = input;
        
        return (d0 * input) + (c0 * filterOut); // mix wet/dry
    }
    
    double getCutoffHz() {
        return cutoffHz_;
    }
    double getQ() {
        return q_;
    }
    double getBoost() {
        return boost_;
    }
    
    void setSampleRate(double sampleRate) {
        sampleRate_ = sampleRate;
        setCutoffHz(cutoffHz_);
    }
    
private:
    double cutoffHz_ = 110.0, q_ = 5.0, boost_ = 1.0;
    double z_1 = 0.0, z_2 = 0.0, z_3 = 0.0, z_4 = 0.0;
    double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0, c0 = 0.0, d0 = 0.0;
    double sampleRate_ = 44100.0;
    
    void calculateCoefficiants() {
        
        double clamp = M_PI * cutoffHz_ / sampleRate_ ;
        if (clamp >= M_PI/2) {
            clamp = M_PI/2 - 0.0001;
        }
        double K = tan(clamp);
        double V = pow(10, boost_ / 20.0);
        
        double d = 1 + (1 / q_) * K + (K * K);
        double e = 1 + (1/(V*q_)) * K + (K * K);
        
        double a = 1 + (V / q_) *K + (K * K);
        double B = 2 * (K*K - 1.0);
        double Y = 1 - (V / q_) * K + (K * K);
        double S = 1 - (1 / q_) * K + (K * K);
        double n = 1 - (1/(V*q_)) * K + (K * K);
        
        if (boost_ >= 0.0) {    // boost
            a0 = a/d;
            a1 = B/d;
            a2 = Y/d;
            b1 = B/d;
            b2 = S/d;
        }
        else {    // cut
            a0 = d/e;
            a1 = B/e;
            a2 = S/e;
            b1 = B/e;
            b2 = n/e;
        }
        
        c0 = 1.0;
        d0 = 0.0;
        
    }
    
};

/*
 ███████████████████████████████████████████████████████████████████████████████
 ▒ VA first order low pass filter ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
 ███████████████████████████████████████████████████████████████████████████████
 *
 *
 *                       R                            --------------------
 *        V o──────────/\/\/─────────┬────> Vc        //
 *                                   |                //
 *                                  ———               //
 *                                  --- C             //
 *                                   |                //
 *                                   V                //
 *
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 *
 *           [input]   ┌───┐   ┌───┐ [V]  ┌───┐  [Y]    [output]
 *            x(n) ───>│ + ├──>│ G ├──o-─>│ + ├───o────> y(n)
 *                     └─┬─┘   └───┘  │   └─┬─┘   │
 *                       ▲-           │     ▲     │
 *                       │            │     │     │
 *                ┌──────┴──────────────────o[lastSample_]
 *                │                   │     │     │
 *                ▼                   │   ┌─┴─┐   │
 *           [feedback]               │   │z-1│   │
 *                                    │   └─┬─┘   │
 *                                    │     ▲[B]  │
 *                                    │     │     │
 *                                    │   ┌─┴─┐   │
 *                                    └──>│ + │<──┘
 *                                        └───┘
 *
 * ▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂
 */
class DiodeOnePoleFilter {
public:
    DiodeOnePoleFilter(double cutoffHz = 5000.0) {
        setCutoffHz(cutoffHz);
    }

    void setCutoffHz(double cutoffHz) {
        cutoffHz_ = cutoffHz;
        double cutoffRadians = 2 * M_PI * cutoffHz;
        double sampleSecs = 1 / 44100.0; // TODO: SampleRate
        double lolwut = (2 / sampleSecs) * tan(cutoffRadians * (sampleSecs / 2)) * (sampleSecs / 2);
        coefficient_ = lolwut / (1.0 + lolwut);
    }
    double getCutoffHz() {
        return cutoffHz_;
    }

    void reset() {
        lastSample_ = 0.0;
    }


    double getOutput(double input) {
        double V = coefficient_ * (input - lastSample_);
        double Y = V + lastSample_;
        double B = V + Y;
        double output = Y;
        lastSample_ = B;
        return output;
    }
    double getFeedback(){
        return lastSample_;
    }

private:
    double lastSample_ = 0.0;
    double coefficient_;
    double cutoffHz_;
};



class DiodeFilter : public BaseFilter {
    
public:
    
    DiodeFilter(double cutoffHz = 6000.0) {
        BaseFilter::setCutoffHz(cutoffHz);
    }
    

    void kill() override {std::cout << "killed!" << std::endl; delete this;}
    void prepUniqueParameter() override {
        drive_.setTone(uniqueParameter_);
    }
    
    double getOutput(double input) override {
        
        double driven = drive_.getOutput((input - (k_*lastSample_)));
        
        double lpf1 = lpf1_.getOutput( driven + lpf2_.getFeedback() );
        double lpf2 = lpf2_.getOutput(e_*(lpf1  + lpf3_.getFeedback()));
        double lpf3 = lpf3_.getOutput(e_*(lpf2  + lpf4_.getFeedback()));
        double lpf4 = lpf4_.getOutput(e_*(lpf3));
        
        lastSample_ = lpf4;
        
        return lastSample_;
        
    }
    
    void prepResCoefficients() override {
        k_ = resonance_;
    }
    
    void prepCutoffCoefficients(void) override {
        lpf1_.setCutoffHz(cutoffHz_);
        lpf2_.setCutoffHz(cutoffHz_);
        lpf3_.setCutoffHz(cutoffHz_);
        lpf4_.setCutoffHz(cutoffHz_);
    }
    void reset(void) override {
        lpf1_.reset();
        lpf2_.reset();
        lpf3_.reset();
        lpf4_.reset();
    }
    

    
    
    
    
    SoftClip drive_ = SoftClip(1.0, 5.0, 0.0, 1.0);
private:

    double k_ = 0.1;
    double e_ = 0.5;
    double lastSample_ = 0.0;
    DiodeOnePoleFilter lpf1_ = DiodeOnePoleFilter();
    DiodeOnePoleFilter lpf2_ = DiodeOnePoleFilter();
    DiodeOnePoleFilter lpf3_ = DiodeOnePoleFilter();
    DiodeOnePoleFilter lpf4_ = DiodeOnePoleFilter();
    
    

};

class Ms20LowPass : public BaseFilter
{
public:
    Ms20LowPass(double cutoffHz = 6000.0) {
        BaseFilter::setCutoffHz(cutoffHz);
    }
    ~Ms20LowPass(){}
    
    void kill() override {std::cout << "killed!" << std::endl; delete this;}
    void prepUniqueParameter() override {
        drive_.setTone(uniqueParameter_);
    }
    
    void prepCutoffCoefficients(void) override {
        
        
        double wd = 2.0*M_PI*cutoffHz_;
        double T = 1.0/(double)sampleRate_;
        double wa = (2.0/T)*tan(wd*T/2.0);
        double g = wa*T/2.0;
        // roots when R1C1 = R2C2 - these can be pre-calculated in the constructor!
        double highMult = -0.5*(-3.0 - pow((double)5.0, (double)0.5));
        double lowMult = -0.5*(pow((double)5.0, (double)0.5) - 3.0);
        

        double gLowEdge = lowMult*wa*T/2.0;
        double gHighEdge = highMult*wa*T/2.0;
        
        double GLow = gLowEdge/(1.0 + gLowEdge);
        double GHigh = gHighEdge/(1.0 + gHighEdge);
        // simulates lossy RC-CR BPF
        double L = (1.0/3.0)*(highMult + lowMult)/highMult;
        
        lop1_.coefficient_ = GLow;
        lop2_.coefficient_ = GHigh;
        hip_.coefficient_ = GLow;
        
        hip_.beta_ = -L/(1.0 + gLowEdge);
        lop2_.beta_ = (L*k_ - L*k_*GLow)/(1.0 + gHighEdge);
        
        // set alpha0 variable
        a0 = 1.0/(1.0 - L*k_*GHigh + L*k_*GHigh*GLow);
        
        
        
        //lop1_.setCutoffHz(cutoffHz_);
        //lop2_.setCutoffHz(cutoffHz_);
        //hip_.setCutoffHz(cutoffHz_);
    }
    void prepResCoefficients() override {
        k_ = resonance_;
        
        double wd = 2.0*M_PI*cutoffHz_;
        double T = 1.0/(double)sampleRate_;
        double wa = (2.0/T)*tan(wd*T/2.0);
        double g = wa*T/2.0;
        // roots when R1C1 = R2C2 - these can be pre-calculated in the constructor!
        double highMult = -0.5*(-3.0 - pow((double)5.0, (double)0.5));
        double lowMult = -0.5*(pow((double)5.0, (double)0.5) - 3.0);
        
        
        double gLowEdge = lowMult*wa*T/2.0;
        double gHighEdge = highMult*wa*T/2.0;
        
        double GLow = gLowEdge/(1.0 + gLowEdge);
        double GHigh = gHighEdge/(1.0 + gHighEdge);
        // simulates lossy RC-CR BPF
        double L = (1.0/3.0)*(highMult + lowMult)/highMult;
        
        lop1_.coefficient_ = GLow;
        lop2_.coefficient_ = GHigh;
        hip_.coefficient_ = GLow;
        
        hip_.beta_ = -L/(1.0 + gLowEdge);
        lop2_.beta_ = (L*k_ - L*k_*GLow)/(1.0 + gHighEdge);
        
        // set alpha0 variable
        a0 = 1.0/(1.0 - L*k_*GHigh + L*k_*GHigh*GLow);
    }
    void reset(void) override {
        lop1_.reset();
        lop2_.reset();
        hip_.reset();
    }
    double getOutput(double input) override {
        
        double preDrive = a0 * (lop1_.getOutput(input) +
                                (hip_.getFeedback()) +
                                (lop2_.getFeedback())
                                );
        
        double drive = drive_.getOutput(preDrive);

        double y = k_ * lop2_.getOutput(drive);
        hip_.getOutput(y);

        return y * (1.0/k_);
        
        //return lop2_.getOutput(lop1_.getOutput(input));
    }
    
    SoftClip drive_ = SoftClip(1.0, 1.5, 0.0, 1.177);
private:
    LowPassFilter lop1_ = LowPassFilter();
    LowPassFilter lop2_ = LowPassFilter();
    HighPassFilter hip_ = HighPassFilter();
    
    double lastSample_ = 0.0;
    double k_ = 0.3;
    double a0 = 0.7;
    double b2 = 0.6;
    double b3 = 0.5;
    
};

class Ms20HiPass : public BaseFilter {
public:
    Ms20HiPass(double cutoffHz = 6000.0){
        BaseFilter::setCutoffHz(cutoffHz);
    }
    ~Ms20HiPass(){}
    
    void kill() override {std::cout << "killed!" << std::endl; delete this;}
    void prepUniqueParameter() override {
        //drive_.setTone(uniqueParameter_);
        saturation_ = uniqueParameter_;
    }
    
    void reset(void) override {
        lpf1_.reset();
        hpf1_.reset();
        hpf3_.reset();
        prepCutoffCoefficients();
    }
    double getOutput(double input) override {
        double y1 = hpf1_.getOutput(input);
        
        double S35H = hpf3_.getFeedback() + lpf1_.getFeedback();
        
        double u = a0_ * (y1 + S35H);
        
        double y =  k_ * u;
        
        
        
        // Normalized Version
        if(y >= 0) //positivehalffirst:
            y = (1.0/tanh(saturation_))*tanh(saturation_*y);
        else
                // lower half adds 1.25x the saturation
                // uncomment to try asymmetrical clipping
            y = (1.0/tanh(1.25*saturation_))*tanh(1.25*saturation_*y);
                // default is symmetrical
                    //y = (1.0/tanh(saturation_))*tanh(saturation_*y);
        
         // Regular Version
         //if(y>=0) //positivehalffirst:
         //y = tanh(saturation_*y);
         //else
          // lower half adds 1.25x the saturation]  // uncomment to try asymmetrical clipping //! y = tanh(1.25*m_dSaturation*y);
        // default is symmetrical y = tanh(m_dSaturation*y);
         
        
        lpf1_.getOutput(hpf3_.getOutput(y));
        
        if (k_ > 0.0)
            y *= 1.0/k_;
        
        return y;
        
    }
    void prepCutoffCoefficients(void) override {
        double wd = 2*M_PI*cutoffHz_;
        double T = 1/(double)sampleRate_;
        double wa = (2/T)*tan(wd*T/2);
        double g = wa*T/2;
        
        double G = g/(1.0 + g);
        
        hpf1_.coefficient_ = G;
        lpf1_.coefficient_ = G;
        hpf3_.coefficient_ = G;
        
        hpf3_.beta_ = -1.0*G/(1.0 + g);
        lpf1_.beta_ = 1.0/(1.0 + g);
        
        a0_ = 1.0/(1.0 - k_*G + k_*G*G);
    }
    void prepResCoefficients() override {
        k_ = resonance_;
        prepCutoffCoefficients();
    }
    
    SoftClip       drive_ = SoftClip(1.0, 1.5, 0.0, 1.0);
private:
    LowPassFilter  lpf1_ = LowPassFilter();
    HighPassFilter hpf1_ = HighPassFilter();
    HighPassFilter hpf3_ = HighPassFilter();
    
    double saturation_ = 1.5;
    double k_ = 0.3;
    double a0_ = 0.7;
};


class SemHighPassFilter : public LowPassFilter
{
public:
    SemHighPassFilter(double cutoffHz = 5000.0) : LowPassFilter(cutoffHz) {}
    
    double getOutput(double input) override {
        double V = coefficient_ * (input - lastSample_);
        double Y = V + lastSample_;
        double B = V + Y;
        double output = drive_.getOutput(Y);
        lastSample_ = B;
        return output;
    }
private:
    SoftClip drive_ = SoftClip(1.0, 1.7, 0.0, 1.0);
};



class SemFilter : public BaseFilter {
public:
    SemFilter(double cutoffHz = 6000.0){
        BaseFilter::setCutoffHz(cutoffHz);
        lpf2_.beta_ = 1.0;
    }

    void kill() override { delete this; }
    void reset(void) override {
        lpf1_.reset();
        lpf2_.reset();
    }
    double getOutput(double input) override {
        
        double hp = a0_ * (input - lpf1_.getFeedback() - lpf2_.getFeedback());
        double bp = lpf1_.getOutput(hp);
        double lp = lpf2_.getOutput(bp);
        
        if (selector_ <= 0.001)
            return bp;
        else
            return (cos(selector_)*lp + sin(selector_)*hp);
        
    }
    void prepCutoffCoefficients(void) override {
        double wd = 2*M_PI*cutoffHz_;
        double T = 1/(double)sampleRate_;
        double wa = (2/T)*tan(wd*T/2);
        double g = wa*T/2;
        
        dampen_ = 1.0/(2.0*k_);
        
        a0_ = 1.0/(1.0 + 2.0 * dampen_ * g + g*g);
        lpf1_.coefficient_ = g;
        lpf2_.coefficient_ = g;
        lpf1_.beta_ = 2.0*dampen_ + g;
    }
    void prepResCoefficients() override {
        k_ = resonance_;
        prepCutoffCoefficients();
    }
    void prepUniqueParameter() override {}
private:
    SoftClip drive_ = SoftClip();
    SemHighPassFilter lpf1_ = SemHighPassFilter();
    LowPassFilter lpf2_ = LowPassFilter();
    double k_ = 0.3;
    double a0_ = 0.7;
    double selector_ = 0.0;
    double dampen_ = 0.0;
};

#endif /* Filter_cpp */
