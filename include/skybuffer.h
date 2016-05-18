#ifndef SKYBUFFER_H
#define SKYBUFFER_H

#include <star.h>
#include "constellation.h"

class SkyBuffer {
public:
	SkyBuffer(Constellation * constellation,unsigned int lines, unsigned int width);
	void addPoint(unsigned int x, unsigned int y, double luminance);
	void turn();
	
protected:
	Constellation * constellation;
	Star *** buffer;
	Star *** buf;
	unsigned int lines;
	unsigned int width;
	
};

#endif