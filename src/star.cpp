#include "star.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


std::vector<Star *> Star::stars;

Star::Star(unsigned int x, unsigned int y, double luminance) {
	this->lum=luminance;
	this->top=y;
	this->bottom=y;
	this->left=x;
	this->right=x;
	stars.push_back(this);
}

Star::Star() {
	this->lum=0;
	this->top=0;
	this->bottom=0;
	this->left=0;
	this->right=0;
}

int Star::count() {
	return Star::stars.size();
}

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


void Star::print() {
	unsigned i=0;
	for (std::vector<Star *>::iterator it = Star::stars.begin() ; it != Star::stars.end(); ++it) {
		std::cout << "Star \t" << i << "\t" << (*it)->x() << "\t" << (*it)->y() << "\t" << (*it)->luminance()<< std::endl;
		i++;
	}
}