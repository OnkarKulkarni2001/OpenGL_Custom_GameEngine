// Developed by Onkar Parag Kulkarni, Nov 2024
// Just for portfolio purposes and had some time between some project deadlines that's why xD

#pragma once
#include <vector>

struct sColor32Bit {
	float r, g, b, a;

	sColor32Bit();
	sColor32Bit(float r, float g, float b, float a);
	~sColor32Bit();
};

struct sColor24Bit {
	float r, g, b;

	sColor24Bit();
	sColor24Bit(float r, float g, float b);
	~sColor24Bit();
};

class cBMPImage {
public:
	cBMPImage(int width, int height);
	~cBMPImage();

	int GetImageWidth();
	int GetImageHeight();

	sColor32Bit GetColor32Bit(int x, int y) const;
	sColor24Bit GetColor24Bit(int x, int y) const;
	void SetColor32Bit(const sColor32Bit& color, int x, int y);
	void SetColor24Bit(const sColor24Bit& color, int x, int y);

	void ReadBMP32Bit(const char* path);
	void ReadBMP24Bit(const char* path);
	void ExportBMP32Bit(const char* path) const;
	void ExportBMP24Bit(const char* path) const;

private:

	int imageWidth;
	int imageHeight;
	std::vector<sColor32Bit> imageColors32Bit;
	std::vector<sColor24Bit> imageColors24Bit;
};