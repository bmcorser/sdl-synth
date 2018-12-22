#ifndef SoftClip_cpp
#define SoftClip_cpp

#include <iostream>
#include <math.h> // is this it?

class SoftClip 
{
public:
    SoftClip(double pregain = 1.0, double tone = 1.0, double dc = 0.0, double asymmetry = 1.0) {
        dc_ = dc;
		pregain_ = pregain;
		tone_ = tone;
        asymmetry_ = asymmetry;
        setTone(tone);
	}

	void setPregain(double pregain = 1.0) {
		pregain_ = pregain;
	}
    void setPostgain(double postgain = 1.0) {
        postgain_ = postgain;
    }
	void setTone(double tone = 1.0) {
		tone_ = tone;
        driveAmountHi_ = tone;
        driveAmountLow_ = tone * asymmetry_;
        std::cout << tone << std::endl;
	}
    
    double getPregain() {return pregain_;}
    double getPostgain() {return pregain_;}
    double getTone() {return tone_;}

	double getOutput(double input){

		double output = (input * pregain_) + dc_;
		//            1. <-------------------------- max out
		//  out = ---------- X arctan(k*input)
		//        archtan(k)
        
        double tempTone = 5.0;
        if (output >= 0.0) { tempTone = driveAmountHi_; }
        else { tempTone = driveAmountLow_; }
        
        double tanh = ((maxOut_/atan(tempTone))*atan(tempTone*output));
        
        
		return (postgain_ * tanh);
	}

private:
	double pregain_ = 1.0, postgain_ = 1.0;
    double dc_ = 0.0;
	double tone_;
    double maxOut_ = 1.0;
    double asymmetry_ = 0.0;
    double driveAmountHi_;
    double driveAmountLow_;
};

#endif /* SoftClip_cpp */
