#include "cLowPassFilter.h"

cLowPassFilter::cLowPassFilter()
{
	// Assume there's 60 samples unless otherwise set
	this->m_numSamplesKept = 60;
	// Reset the vector
	this->m_ResizeSampleVector();
	
}

void cLowPassFilter::addSample(double sampleValue)
{
	// Add this value to the vector
	(*this->m_itNextSample) = sampleValue;
	// move the iterator to the "next" location
	this->m_itNextSample++;
	// Off the end of the buffer
	if (this->m_itNextSample == this->m_vecSamples.end())
	{
		// Yes, so point it to the start
		this->m_itNextSample = this->m_vecSamples.begin();
	}
	return;
}


void cLowPassFilter::m_ResizeSampleVector(void)
{
	// TODO:Make a copy of the existing samples, if any
	// 	std::vector< double > vecTempSamples = this->m_vecSamples;
	//
	// Replaces the old vector with an empty one
	// (shrink won't actually reduce memory allocation of vector)
	this->m_vecSamples = std::vector<double>();

	// Add n samples
	for (unsigned int count = 0; count != this->m_numSamplesKept; count++)
	{
		this->m_vecSamples.push_back(0.0);
	}
	// Point the new sample location to the start of the vector
	this->m_itNextSample = this->m_vecSamples.begin();

	return;
}

void cLowPassFilter::setNumSamples(unsigned int newNumSamples)
{
	this->m_numSamplesKept = 60;
	// Reset the vector
	this->m_ResizeSampleVector();
	return;
}
double cLowPassFilter::getAverage(void)
{
	// Get the average of the entire vector
	double total = 0.0;
	for (double curVal : this->m_vecSamples)
	{
		total += curVal;
	}
	// 
	return total / (double)this->m_numSamplesKept;
}


