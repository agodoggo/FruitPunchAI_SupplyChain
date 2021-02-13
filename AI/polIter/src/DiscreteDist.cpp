#include "DiscreteDist.h"
#include <math.h>

const double PI = std::atan(1.0) * 4;

bool DiscreteDist::IsProbMassFunction(std::vector<double> PMF)
{
    double TotalProb = 0.0;
    for(auto d : PMF)
    {
        TotalProb += d;
        if (d < 0.0)
        {
            return false;
        }
    }
    return std::abs(TotalProb - 1.0) < 1e-8;
}

DiscreteDist DiscreteDist::Add(const DiscreteDist& other)
{
    long minResult = this->min + other.min;
    long maxResult = this->max + other.max;
    std::vector<double> PMFResult(maxResult - minResult + 1l, 0.0);
    for (size_t i = 0; i < TranslatedPMF.size(); i++)
    {
        for (size_t j = 0; j < other.TranslatedPMF.size(); j++)
        {
            PMFResult[i + j] += this->TranslatedPMF[i] * other.TranslatedPMF[j];
        }
    }
    return DiscreteDist(PMFResult, minResult);
}
DiscreteDist DiscreteDist::TakeMaximumWith(long value)
{
    if (value <= min)
    {
        return DiscreteDist(TranslatedPMF, min);
    }
    if (value >= max)
    {
        return GetConstantDistribution(value);
    }
    long minResult = value;
    long delta = value - min;
    long maxResult = this->max;

    std::vector<double> PMFResult((maxResult - minResult) + 1l,0.0);
    for (size_t i = 0; i <= delta; i++)
    {
        PMFResult[0] += TranslatedPMF[i];
    }
    for (size_t i = delta+1l; i <= max-min ; i++)
    {
        PMFResult[i - delta] = this->TranslatedPMF[i];
    }
    return DiscreteDist(PMFResult, minResult);

}
DiscreteDist DiscreteDist::Invert()
{
    int minResult = -this->max;
    int l = this->max - this->min;
    std::vector<double> PMFResult(l + 1, 0.0);

    for (size_t i = 0; i < l; i++)
    {
        PMFResult[i] = this->TranslatedPMF[l - i];

    }
    return DiscreteDist(PMFResult,minResult);
}
void DiscreteDist::ToConsole()
{
    std::cout << "----------------" << std::endl;
    for (size_t i = min; i <= max; i++)
    {
        double prob = ProbabilityAt(i);
        std::cout << i << " : " << prob << std::endl;
    }
    std::cout << "----------------" << std::endl;

}
std::vector<double> DiscreteDist::ToPMF()
{
    std::vector<double> returnVal;
    for (size_t i = 0; i < min; i++)
    {
        returnVal.push_back(0.0);
    }
    for (size_t i = 0; i < TranslatedPMF.size(); i++)
    {
        returnVal.push_back(TranslatedPMF[i]);
    }
    return returnVal;
}


long DiscreteDist::GetFractile(double alpha)
{
    double resProb = 1.0;
    double resProbTarget = 1 - alpha;
    long i;
    for (i = min; i <= max; i++)
    {
        resProb -= ProbabilityAt(i);
        if (resProb < resProbTarget)
        {
            break;
        }
    }
    return i;
}
DiscreteDist DiscreteDist::GetConstantDistribution(long constant)
{
    return DiscreteDist({ 1.0 }, constant);
}
DiscreteDist DiscreteDist::GetZeroDistribution()
{
    return GetConstantDistribution(0);
}

std::vector<double> DiscreteDist::GetPoissonPMF(double rate)
{
    std::vector<double> returnVec;

    long uBound = (long)(rate + 7.0 + 7.0 * std::sqrt(rate));
    if (rate < 100.0)
    {
        returnVec.reserve(uBound);
        long i = 0;
        double factor = std::exp(-rate);
        while (i < uBound)
        {
            returnVec.push_back(factor);
            factor *= rate / (++i);
        }
    }
    else
    {//use normal approximation:
        long i = 0;
        while (i < uBound)
        {
            double next = 1.0 / std::sqrt(2 * PI * rate) *
                std::exp(-(i - rate) * (i - rate) / (2 * rate));
            returnVec.push_back(next);    
            i++;
        }
    }
    return returnVec;

}
/// <summary>
/// Zero-based geometric distribution
/// </summary>
std::vector<double> DiscreteDist::GetGeometricPMF(double mean)
{
    std::vector<double> returnVec;
    double p = 1.0 / (1.0 + mean);
    double prob = p;
    while (prob > epsilon)
    {
        returnVec.push_back(prob);
        prob *= (1 - p);
    }
    return returnVec;

}

double DiscreteDist::ProbabilityAt(long value)
{
	return TranslatedPMF[value - min];
}

double DiscreteDist::Expectation()
{
    double exp{ 0.0 };
    for (long i = min; i <= max; i++)
    {
        double prob = ProbabilityAt(i);
        exp += prob * i;
    }
    return exp;
}

DiscreteDist::DiscreteDist(const std::vector<double>& TranslatedProbMF, long offSet)
{
	if (!IsProbMassFunction(TranslatedProbMF))
	{
		std::string message = "Probabilities should be nonnegative and sum to 1.0";
		std::cout << message;
        throw message;
	}
    long iter = 0;
    while (TranslatedProbMF[iter] <= epsilon)
    {
        iter++;
    }
    long minPos = iter;
    iter = TranslatedProbMF.size() - 1;
    while (TranslatedProbMF[iter] < epsilon)
    {
        iter--;
    }
    long maxPos = iter;
    this->TranslatedPMF.reserve(maxPos + 1 - minPos);
    for (int i = minPos; i <= maxPos; i++)
    {
        this->TranslatedPMF.push_back(TranslatedProbMF[i]);
    }
    min = offSet + minPos;
    max = min + this->TranslatedPMF.size() - 1;
}
