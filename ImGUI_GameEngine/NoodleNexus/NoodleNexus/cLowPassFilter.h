#pragma once

#include <vector>

// This is being used to get an average frame time
class cLowPassFilter
{
public:
	cLowPassFilter();
	void setNumSamples(unsigned int newNumSamples);
	double getAverage(void);
	void addSample(double sampleValue);

private:
	std::vector< double > m_vecSamples;
	// points to the next sample location
	std::vector< double >::iterator m_itNextSample;
	unsigned int m_numSamplesKept;
	// If the numebr of samples changes, call this
	void m_ResizeSampleVector(void);
};