#ifndef StepSequencer_cpp
#define StepSequencer_cpp

#include <vector>

using namespace std;

class StepSequencer 
{
private:
	long long samplesPerBeat_ = 44100;
	long long sampleCounter_ = 0;
	unsigned int stepCounter_ = 0;
	unsigned int numberOfSteps_ = 16;

    vector<double> stepsArray1_;

	double bpm_ = 120.0;
	bool isPlaying_ = false;

public:
	StepSequencer(unsigned int numberOfSteps, double bpm = 120.0) 
	{
		numberOfSteps_ = numberOfSteps; 
		setBPM(bpm);

		for (int i = 0; i < numberOfSteps_; i++)
		{
			stepsArray1_.push_back(0.0);
		}
	}
	void setBPM(double bpm) 
	{
		bpm_ = bpm;
		//	how meny samples in one beat
		//  case study:
		//  60 bpm -> 1 beat per second -> 44100 samples
		//  140/60 -> 2.33333333        -> 44100.0 / 2.3333333333333335  =  18900
        // TODO: SampleRate
		samplesPerBeat_ = static_cast<unsigned int>(44100.0 / (bpm_ / 60.0) );
	}

	void setPlaying(bool flag) 
	{
		if (flag) {
			isPlaying_ = true;
		} else {
			isPlaying_ = false;
		}
	}
	unsigned int getStep() 
	{
		return static_cast<unsigned int>(stepCounter_);
	}

	// must be called in callback
	void updateMetronome() 
	{
		if (isPlaying_) {
			sampleCounter_++;
			if(sampleCounter_ >= samplesPerBeat_) {
				sampleCounter_ = 0;
				stepCounter_++;
				if (stepCounter_ >= numberOfSteps_) stepCounter_ = 0;
			}
		} else {
			if (sampleCounter_ != 0) sampleCounter_  = 0;
			if (stepCounter_   != 0) stepCounter_    = 0;
		}
	}


	double getOutput() 
	{
		return stepsArray1_[stepCounter_];
	}

	void setStepValue(double value, unsigned int step) 
	{
    		stepsArray1_[step] = value;
	}

	void setArrayToStepNumber(double passedArray[]) 
	{
		for (int n = 0; n < numberOfSteps_; n++) 
		{
			stepsArray1_[n] = passedArray[n];
    	}
	}



};

#endif /* StepSequencer_cpp */
