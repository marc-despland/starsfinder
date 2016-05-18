#include "star.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


//std::vector<Star *> * Star::stars=new std::vector<Star *>();

unsigned long Star::counter=0;

Star::Star(unsigned int x, unsigned int y, double luminance) {
	this->lum=luminance;
	this->top=y;
	this->bottom=y;
	this->left=x;
	this->right=x;
	this->id=(Star::counter++);
	//stars->push_back(this);
}

Star::Star() {
	this->lum=0;
	this->top=0;
	this->bottom=0;
	this->left=0;
	this->right=0;
	this->id=(Star::counter++);
}

/*int Star::count() {
	return Star::stars->size();
}*/

void Star::add(unsigned int x, unsigned int y, double luminance) {
	this->lum+=luminance;
	if (y>this->top) this->top=y;
	if (y<this->bottom) this->bottom=y;
	if (x<this->left) this->left=x;
	if (x>this->right) this->right=x;

}
double Star::x() {
	return (this->left+this->right)/2;
}
double Star::y() {
	return (this->top+this->bottom)/2;
}
double Star::luminance() {
	return this->lum;
}


bool Star::isNear(unsigned int x, unsigned int y) {
	return (y>=(this->top-1)) && (y<=(this->bottom+1)) && (x>=(this->left-1)) && (x<=(this->right+1));
}


/*void Star::SortList() {
	std::vector<Star *> * sorted=new std::vector<Star *>();
	for (std::vector<Star *>::iterator it = Star::stars->begin() ; it != Star::stars->end(); ++it) {
		std::vector<Star *>::iterator itsearch = sorted->begin();
		bool found=false;
		while (!found && itsearch != sorted->end()) {
			if ((*itsearch)->x()<(*it)->x()) {
				found=true;
				sorted->insert(itsearch, (*it));
			}
			itsearch++;
		}
		if (!found) {
			sorted->push_back((*it));
		}
	}
	std::vector<Star *> * tmp=Star::stars;
	Star::stars=sorted;
	delete tmp;

}

void Star::print() {
	unsigned i=0;
	for (std::vector<Star *>::iterator it = Star::stars->begin() ; it != Star::stars->end(); ++it) {
		std::cout << "Star \t" << i << "\t" << (*it)->x() << "\t" << (*it)->y() << "\t" << (*it)->luminance()<< std::endl;
		i++;
	}
}*/



/*void Star::list(int size) {
	int * ordered
	unsigned i=0;
	for (std::vector<Star *>::iterator it = Star::stars.begin() ; it != Star::stars.end(); ++it) {
		std::cout << "Star \t" << i << "\t" << (*it)->x() << "\t" << (*it)->y() << "\t" << (*it)->luminance()<< std::endl;
		i++;
	}
}*/