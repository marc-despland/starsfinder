#include "matchstars.h"
#include <sstream>
#include "log.h"

MatchStars::MatchStars(Constellation * pattern) {
	for(unsigned int i=0; i<pattern->Size(); i++) {
		this->patternstars[pattern->Stars()->at(i)->Id()]=0;
	}
	this->position=this->patternstars.begin();
}


unsigned long MatchStars::match(unsigned long target) {
	this->patternstars[this->position->first]=target;
	this->searchstars[target]=this->position->first;
	this->position++;
	if (this->position==this->patternstars.end()) return 0;
	return this->position->first;
}

unsigned long MatchStars::unmatch() {
	this->position--;
	if (this->position==this->patternstars.begin()) return 0;
	this->searchstars.erase(this->position->second);
	this->patternstars[this->position->first]=0;
	if (this->position==this->patternstars.begin()) return 0;
	return this->position->first;
}
unsigned long MatchStars::current() {
	return this->position->first;
}

unsigned long MatchStars::lastTarget() {
	std::map<unsigned long,unsigned long>::iterator it=this->position;
	if (it!=this->patternstars.begin()) it--;
	return it->second;
}

unsigned long MatchStars::next() {
	this->position++;
	if (this->position==this->patternstars.end()) return 0;
	return this->position->first;
}

bool MatchStars::known(unsigned long search) {
	return (this->searchstars.count(search)!=0);

//	return (this->searchstars.find(search)!=this->searchstars.end());
}


std::string MatchStars::status() {
	std::stringstream result;
	std::map<unsigned long,unsigned long>::iterator it;
	for (it=this->patternstars.begin();it!=this->patternstars.end(); it++) {
		Star * searched=Star::at(it->first);
		if (it->second>0) {
			Star * find=Star::at(it->second);
			result << "Search star "<< it->first<<"\t ("<<searched->x()<<","<<searched->y()<<") \t Found star "<< it->second<<"\t ("<<find->x()<<","<<find->y()<<")"<<std::endl;
		} else {
			result << "Search star "<< it->first<<"\t ("<<searched->x()<<","<<searched->y()<<") \t no star "<<std::endl;			
		}
	}
	return result.str();
}