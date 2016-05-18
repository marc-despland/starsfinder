#ifndef TRISEARCHSTAR_H 
#define TRISEARCHSTAR_H
#include "star.h"

class TriSearchStar {
public:
	TriSearchStar(Star * stars[3]);
	Star * CalculateThird(Star * first, Star * second);

protected:
	static double AlKashi(double a, double b, double c);
	Star * stars[3];
	double angle[3];
	double length[3];

};

#endif