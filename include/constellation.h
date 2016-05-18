#ifndef CONSTELLATION_H
#define CONSTELLATION_H
#include "star.h"
#include <string>
#include <stdexcept>



class ConstellationUnknownFile : public std::exception {
	const char* what();
};


class Constellation {

public:
	Constellation(std::string name);
	void Add(Star * star);
	void Sort(enum Star::SortType type);
	unsigned int Size();
	std::vector<Star *> * Stars();
	std::string GetName();

	std::string ToJson();
	static Constellation * FromJson(std::string filename) throw(ConstellationUnknownFile);
	static Constellation * FromJpeg(std::string source, double level, unsigned int precision)  throw(ConstellationUnknownFile);
protected:
	std::vector<Star *> * stars;
	std::string name;
};

#endif