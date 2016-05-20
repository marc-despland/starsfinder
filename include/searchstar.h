#ifndef _SEARCHSTAR_H 
#define _SEARCHSTAR_H
#include "star.h"

class SearchStar {
public:
	SearchStar(Star * A, Star * B);
	void Target(Star * targetA, Star * targetB);
	Star * Calculate(Star * C);

protected:
	static double AlKashi(double a, double b, double c);
	double yc(Star * A, Star * B, Star *C);
	double Angle(Star * A, Star * B, Star *C);
	Star * A;
	Star * B;
	Star * targetA;
	Star * targetB;
	double lengthAB;
	double lengthAB2;
	double lengthTAB;
	double targetdx;
	double targetdy;

};

#endif