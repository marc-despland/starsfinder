#ifndef _MATCHSTARS_H
#define _MATCHSTARS_H

#include "constellation.h"
#include <map>

class MatchStars {
public:
	MatchStars(Constellation * pattern);
	unsigned long match(unsigned long target);
	unsigned long unmatch();
	unsigned long next();
	unsigned long current();
	unsigned long lastTarget();
	bool known(unsigned long search);
	std::string status();
protected:
	std::map<unsigned long,unsigned long>::iterator position;
	std::map<unsigned long,unsigned long> patternstars;
	std::map<unsigned long,unsigned long> searchstars;
};

#endif