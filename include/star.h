#ifndef _STAR_H
#define _STAR_H
#include <vector>


class Star{
public:
	Star(unsigned int x, unsigned int y, double luminance);
	Star();
	void add(unsigned int x, unsigned int y, double luminance);
	bool isNear(unsigned int x, unsigned int y);
	
//	static int count();
//	static void print();
	
	double x();
	double y();
	double luminance();

	enum SortType { X, Y, LUMINANCE };
//	static void SortList();
protected:
	unsigned long id;
	static unsigned long counter;
//	static std::vector<Star *> * stars;  
	unsigned int left, top, bottom, right;
	double lum;
};

#endif