#ifndef _STAR_H
#define _STAR_H
#include <vector>
#include <map>


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
	void precision(unsigned int p);
	bool include(Star * target);

	enum SortType { X, Y, LUMINANCE };
	double distance(Star * star);
//	static void SortList();

	unsigned long Id();
	static Star * at(unsigned long id);
protected:
	static std::map<unsigned long,Star *> catalog;

	unsigned long id;
	static unsigned long counter;
//	static std::vector<Star *> * stars;  
	unsigned int left, top, bottom, right;
	double lum;
	double sx, sy;
};

#endif