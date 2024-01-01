/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       generate_pulses.hpp
*   @desc       Header library to generate an exponential pulse dataset compatible with the UCR Archive 2018
*   @author     Graham N. Power
*   @date       2023-12-17
*   @version    1.1.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-16 	Ported code from old python files over to c++
*   1.1.0   gnp     2023-12-17  Added file writing for URC Archive
*	
*
*
*   0.0.0   -   Initial release
*   0.0.1   -   Patched some bug
*   0.1.0   -   Incremental change
*   1.0.0   -   First major release
*
*	</pre>
*
*******************************************************************************/


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#ifndef DTW_GENERATE_PULSES_H_
#define DTW_GENERATE_PULSES_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <sstream>
#include <fstream>


/*******************************************************************************
*   Preprocessor Macros
*******************************************************************************/
#define M_PI 3.14159265358979323846


/*******************************************************************************
*   Datatype Definitions
*******************************************************************************/


/*******************************************************************************
*   Constant Definitions
*******************************************************************************/


/*******************************************************************************
*   Function Protoypes
*******************************************************************************/


/*******************************************************************************
*   Global Variables
*******************************************************************************/


/*******************************************************************************
*   Classes
*******************************************************************************/
class Signal {
    private:
        int length;
        double stride;
        std::vector<double> x;

        double randn()
        {
            double v1 = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
            double v2 = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
            return cos(2*M_PI*v2)*sqrt(-2.*log(v1));
        };

        std::vector<double> noise(double std)
        {
            std::vector<double> noise(this->getXLength());
            for (unsigned int i = 0; i < this->getXLength(); i++)
            {
                noise[i] = std*this->randn();
            }
            return noise;
        };

    public:        
        explicit Signal(int length, double stride) 
        {
            this->length = length;
            this->stride = stride;
            this->x.resize((length/stride)+1);
            std::generate(this->x.begin(), this->x.end(), [n = 0, &stride] () mutable { return stride * n++; });
        };

        std::vector<double> getX()
        {
            return this->x;
        };

        unsigned int getXLength()
        {
            return this->x.size();
        };

        virtual double evaluate(double x) = 0;

        std::vector<double> getfExact()
        {
            std::vector<double> func(this->getXLength());
            for (unsigned int i = 1; i < this->getXLength(); i++)
            {
                func[i] = this->evaluate(this->x[i]);
            }
            return func;
        };

        std::vector<double> getfGaussianY(double std)
        {
            std::vector<double> noise = this->noise(std);
            std::vector<double> func(this->getXLength());
            for (unsigned int i = 1; i < this->getXLength(); i++)
            {
                func[i] = this->evaluate(this->x[i]) + noise[i];
            }
            return func;
        };

        std::vector<double> getfGaussianX(double std)
        {
            std::vector<double> noise = this->noise(std);
            std::vector<double> func(this->getXLength());
            for (unsigned int i = 1; i < this->getXLength(); i++)
            {
                func[i] = this->evaluate(this->x[i]+noise[i]);
            }
            return func;
        };

        std::vector<double> getfGaussianXY(double std)
        {
            std::vector<double> noisex = this->noise(std);
            std::vector<double> noisey = this->noise(std);
            std::vector<double> func(this->getXLength());
            for (unsigned int i = 1; i < this->getXLength(); i++)
            {
                func[i] = this->evaluate(this->x[i]+noisex[i]) + noisey[i];
            }
            return func;
        };
};


class ExponentialPulse : public Signal {
    private:
        bool isNegative;
        double E0;
        double alpha;
        double betaOverAlpha;
        double pulseDelay;
        double beta;
        double k;

    public:
        ExponentialPulse(
            bool isNegative,
            double E0,
            double alpha,
            double betaOverAlpha,
            double pulseDelay,
            int length,
            double stride
        ) : Signal(length, stride) 
        {
            this->isNegative = isNegative;
            this->E0 = E0;
            this->alpha = alpha;
            this->betaOverAlpha = betaOverAlpha;
            this->pulseDelay = pulseDelay;
            this->beta = alpha * betaOverAlpha;
            this->k = exp(-alpha * ( (log(alpha) - log(this->beta)) / (alpha - this->beta) )) - exp(-this->beta * ((log(alpha) - log(this->beta)) / (alpha - this->beta) ));
        };

        ~ExponentialPulse() {}; 

        double evaluate(double x)
        {
            double val = this->E0 * this->k * ( exp(-this->alpha * (x-this->pulseDelay)) - exp(-this->beta * (x-this->pulseDelay)) );
            if (this->isNegative)
            {
                return -val;
            }
            return val;
        };

        void setIsNegative(bool isNegative)
        {
            this->isNegative = isNegative;
        };

        void setE0(double E0)
        {
            this->E0 = E0;
        };
        
        void setAlpha(double alpha)
        {
            this->alpha = alpha;
            this->beta = alpha * this->betaOverAlpha;
            this->k = exp(-alpha * ( (log(alpha) - log(this->beta)) / (alpha - this->beta) )) - exp(-this->beta * ((log(alpha) - log(this->beta)) / (alpha - this->beta) ));
        };

        void setPulseDelay(double pulseDelay)
        {
            this->pulseDelay = pulseDelay;
        };
};


class DoubleExponentialPulse : public Signal {
    private:
        bool isNegative;
        double E0;
        double alpha;
        double betaOverAlpha;
        unsigned int pulse2Delay;
        double pulse2RelE0;
        double beta;
        double k;
    
    public:
        DoubleExponentialPulse(
            bool isNegative,
            double E0,
            double alpha,
            double betaOverAlpha,
            unsigned int pulse2Delay,
            double pulse2RelE0,
            int length,
            double stride
        ) : Signal(length, stride)
        {
            this->isNegative = isNegative;
            this->E0 = E0;
            this->alpha = alpha;
            this->betaOverAlpha = betaOverAlpha;
            this->beta = alpha * betaOverAlpha;
            this->pulse2Delay = pulse2Delay;
            this->pulse2RelE0 = pulse2RelE0;
            this->k = exp(-alpha * ( (log(alpha) - log(this->beta)) / (alpha - this->beta) )) - exp(-this->beta * ((log(alpha) - log(this->beta)) / (alpha - this->beta) ));
        };

        ~DoubleExponentialPulse() {};

        double evaluate(double x)
        {
            double val = this->E0 * this->k * ( exp(-this->alpha * x) - exp(-this->beta * x) );
            if (x > this->pulse2Delay)
            {
                val += this->pulse2RelE0 * this->E0 * this->k * ( exp(-this->alpha * (x - this->pulse2Delay)) - exp(-this->beta * (x - this->pulse2Delay)));
            }
            if (this->isNegative)
            {
                return -val;
            }
            return val;
        };

        void setIsNegative(bool isNegative)
        {
            this->isNegative = isNegative;
        };

        void setE0(double E0)
        {
            this->E0 = E0;
        };
        
        void setAlpha(double alpha)
        {
            this->alpha = alpha;
            this->beta = alpha * this->betaOverAlpha;
            this->k = exp(-alpha * ( (log(alpha) - log(this->beta)) / (alpha - this->beta) )) - exp(-this->beta * ((log(alpha) - log(this->beta)) / (alpha - this->beta) ));
        };

        void setPulse2Delay(double pulse2Delay)
        {
            this->pulse2Delay = pulse2Delay;
        };

        void setPulse2RelE0(double pulse2RelE0)
        {
            this->pulse2RelE0 = pulse2RelE0;
        };
};


class InvertingExponentialPulse : public Signal {
    private:
        bool isNegative;
        double E0;        
        double pulseDelay;
        double pulseWidth;
    
    public:
        InvertingExponentialPulse(
            bool isNegative,
            double E0,
            double pulseDelay,
            double pulseWidth,
            int length,
            double stride
        ) : Signal(length, stride)
        {
            this->isNegative = isNegative;
            this->E0 = E0;
            this->pulseDelay = pulseDelay;
            this->pulseWidth = pulseWidth;
        };

        ~InvertingExponentialPulse() {};

        double evaluate(double x)
        {
            double val = this->E0*((x-this->pulseDelay)/(this->pulseWidth))*exp( -6*M_PI * (pow((x-this->pulseDelay)/(this->pulseWidth), 2)) );
            if (this->isNegative)
            {
                return -val;
            }
            return val;
        };

        void setIsNegative(bool isNegative)
        {
            this->isNegative = isNegative;
        };

        void setE0(double E0)
        {
            this->E0 = E0;
        };

        void setPulseDelay(double pulseDelay)
        {
            this->pulseDelay = pulseDelay;
        };

        void setPulseWidth(double pulseWidth)
        {
            this->pulseWidth = pulseWidth;
        };
};


class SpikePulse : public Signal {
    private:
        bool isNegative;
        double E0;
        double alpha;
        double betaOverAlpha;
        unsigned int pulseDelay;
        double beta;
        double k;
    
    public:
        SpikePulse(
            bool isNegative,
            double E0,
            double alpha,
            double betaOverAlpha,
            unsigned int pulseDelay,
            int length,
            double stride
        ) : Signal(length, stride)
        {
            this->isNegative = isNegative;
            this->E0 = E0;
            this->alpha = alpha;
            this->betaOverAlpha = betaOverAlpha;
            this->beta = alpha * betaOverAlpha;
            this->pulseDelay = pulseDelay;
            this->k = exp(-alpha * ( (log(alpha) - log(this->beta)) / (alpha - this->beta) )) - exp(-this->beta * ((log(alpha) - log(this->beta)) / (alpha - this->beta) ));
        };

        ~SpikePulse() {};

        double evaluate(double x)
        {
            double val = this->E0*this->k*(1/sqrt(this->alpha*this->beta*2*M_PI))*exp(-0.5*(pow(x-this->pulseDelay,2))/(this->alpha*this->beta));
            if (this->isNegative)
            {
                return -val;
            }
            return val;
        };

        void setIsNegative(bool isNegative)
        {
            this->isNegative = isNegative;
        };

        void setE0(double E0)
        {
            this->E0 = E0;
        };
        
        void setAlpha(double alpha)
        {
            this->alpha = alpha;
            this->beta = alpha * this->betaOverAlpha;
            this->k = exp(-alpha * ( (log(alpha) - log(this->beta)) / (alpha - this->beta) )) - exp(-this->beta * ((log(alpha) - log(this->beta)) / (alpha - this->beta) ));
        };

        void setPulseDelay(double pulseDelay)
        {
            this->pulseDelay = pulseDelay;
        };
};


/*******************************************************************************
*   Functions
*******************************************************************************/
template <typename T>
std::vector<T> normalizeVector(std::vector<T> vec)
{
    std::vector<T> result(vec.size());
    double mag = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        mag += vec[i] * vec[i];
    }
    mag = sqrt(mag);
    for (int i = 0; i < vec.size(); i++)
    {
        result[i] = vec[i] / mag;
    }

    return result;
};

template <typename T>
std::string getVectorAsString(std::vector<T> vec)
{
    std::ostringstream oss;
    for (int i = 0; i < vec.size(); i++)
    {
        oss << vec[i] << "\t";
    }

    return oss.str();
};


void generatePulseDataset(int length, double stride, std::string trainFileName, std::string testFileName)
{
    std::ostringstream train;
    std::ostringstream test;
    double std = 0.1;
    unsigned int classNum = 1;
    
    // 1: Exponential Pulse
    ExponentialPulse e_sig(
        false, // isNegative
        10, // E0
        0.3, // alpha
        10, // betaOverAlpha
        0, // pulseDelay
        length, // length
        stride // stride
    );

    for (int i = 0; i < 10; i++)
    {
        train << classNum << "\t" << getVectorAsString(normalizeVector(e_sig.getfGaussianXY(std))) << "\n";
    }

    double alpha = 0;
    for(int i = 1; i < 11; i++)
    {
        alpha += 0.05; // 0.05 -> 0.5
        e_sig.setAlpha(alpha);

        double pulseDelay = 0;
        for (int j = 0; j < 5; j++)
        {
            pulseDelay += 10;
            e_sig.setPulseDelay(pulseDelay);

            test << classNum << "\t" << getVectorAsString(normalizeVector(e_sig.getfGaussianXY(std))) << "\n";
        }
    }
    classNum++;


    // 2: Double Exponential Pulse
    DoubleExponentialPulse d_sig(
        false, // isNegative
        10, // E0
        0.1, // alpha
        10, // betaOverAlpha
        10, // pulse2Delay
        0.2, // pulse2RelE0
        length, // length,
        stride // stride
    );

    for (int i = 0; i < 10; i++)
    {
        train << classNum << "\t" << getVectorAsString(normalizeVector(d_sig.getfGaussianXY(std))) << "\n";
    }

    alpha = 0;
    for(int i = 1; i < 11; i++)
    {
        alpha += 0.05; // 0.05 -> 0.5
        d_sig.setAlpha(alpha);

        double pulse2RelE0 = 0;
        for (int j = 0; j < 5; j++)
        {
            pulse2RelE0 += 0.1; // 0.1 -> 0.4        
            d_sig.setPulse2RelE0(pulse2RelE0);
                                    
            test << classNum << "\t" << getVectorAsString(normalizeVector(d_sig.getfGaussianXY(std))) << "\n";            
        }
    }
    classNum++;


    // 3: Inverting Exponential Pulse
    InvertingExponentialPulse i_sig( 
        false, // isNegative
        50, // E0            
        10, // pulseDelay
        15, // pulseWidth
        length, // length
        stride // stride
    );

    for (int i = 0; i < 10; i++)
    {
        train << classNum << "\t" << getVectorAsString(normalizeVector(i_sig.getfGaussianXY(std))) << "\n";
    }

    double pulseWidth = 9;
    for(int i = 1; i < 11; i++)
    {
        pulseWidth += 1; // 10 -> 20
        i_sig.setPulseWidth(pulseWidth);

        double pulseDelay = 0;
        for (int j = 0; j < 5; j++)
        {
            pulseDelay += 10;
            i_sig.setPulseDelay(pulseDelay);

            test << classNum << "\t" << getVectorAsString(normalizeVector(i_sig.getfGaussianXY(std))) << "\n";
        }
    }
    classNum++;


    // 4: Spike Pulse
    SpikePulse s_sig(
        false, // isNegative
        50, // E0
        1, // alpha
        10, // betaOverAlpha
        15, // pulseDelay
        length, // length
        stride // stride
    );

    for (int i = 0; i < 10; i++)
    {
        train << classNum << "\t" << getVectorAsString(normalizeVector(s_sig.getfGaussianXY(std))) << "\n";
    }

    alpha = 0;
    for(int i = 1; i < 11; i++)
    {
        alpha += 0.05; // 0.05 -> 0.5
        s_sig.setAlpha(alpha);

        double pulseDelay = 0;
        for (int j = 0; j < 5; j++)
        {
            pulseDelay += 10;
            s_sig.setPulseDelay(pulseDelay);

            test << classNum << "\t" << getVectorAsString(normalizeVector(s_sig.getfGaussianXY(std))) << "\n";
        }
    }
    classNum++;

    
    std::string trainString = train.str();
    std::string testString = test.str();

    std::ofstream trainFile(trainFileName);
    trainFile << trainString;
    trainFile.close();

    std::ofstream testFile(testFileName);
    testFile << testString;
    testFile.close();
};


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_GENERATE_PULSES_H_ */