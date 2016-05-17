#ifndef PIXEL_H
#define PIXEL_H

class Pixel {
public:
	Pixel(unsigned int R, unsigned int G, unsigned int B);
	unsigned int R();
	unsigned int G();
	unsigned int B();
	double Lightness();
	double Saturation();
	double Hue();
	void Brighter(double x);



protected:
	void RGBtoHSL();
	void HSLtoRGB();
	static double HUEtoRGB(double v1, double v2, double vh);
	unsigned int r;
	unsigned int g;
	unsigned int b;
	double h;
	double s;
	double l;
};


#endif