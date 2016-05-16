#ifndef SKYBUFFER_H
#define SKYBUFFER_H

#include <star.h>


class SkyBuffer {
public:
	SkyBuffer(unsigned int lines, unsigned int width);
	void addPoint(unsigned int x, unsigned int y, double luminance);
	void turn();
	
protected:
	Star *** buffer;
	Star *** buf;
	unsigned int lines;
	unsigned int width;
	
};

#endif