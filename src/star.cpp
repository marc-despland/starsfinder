#include "star.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h> 
#include "log.h"

//std::vector<Star *> * Star::stars=new std::vector<Star *>();
std::map<unsigned long,Star *> Star::catalog;
unsigned long Star::counter=0;

Star::Star(unsigned int x, unsigned int y, double luminance) {
	this->lum=luminance;
	this->top=y;
	this->bottom=y;
	this->left=x;
	this->right=x;
	this->id=(++Star::counter);
	Star::catalog[this->id]=this;
	//stars->push_back(this);
}

Star::Star() {
	this->lum=0;
	this->top=0;
	this->bottom=0;
	this->left=0;
	this->right=0;
	this->id=(Star::counter++);
	Star::catalog[this->id]=this;
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


double Star::distance(Star * star) {
	double dx=this->x()-star->x();
	double dy=this->y()-star->y();
	return sqrt((dx*dx)+(dy*dy));
}

void Star::precision(unsigned int p) {
	unsigned int x=this->x();
	unsigned int y=this->y();
	this->left=x-p;
	this->right=x+p;
	this->top=y-p;
	this->bottom=y+p;
}

bool Star::include(Star * target) {
	bool result=((target->x()>this->left) && (target->x()<this->right) && (target->y()>this->top) && (target->y()<this->bottom));
	if (result) {
		Log::logger->log("STAR", DEBUG) << "Include this("<<this->left<<","<<this->right<<","<<this->top<<","<<this->bottom<<")   Target ("<<target->x()<<","<<target->y()<<") TRUE"<<std::endl;
	} else {
		Log::logger->log("STAR", DEBUG) << "Include this("<<this->left<<","<<this->right<<","<<this->top<<","<<this->bottom<<")   Target ("<<target->x()<<","<<target->y()<<") FALSE"<<std::endl;
	}
	return result;
}

unsigned long Star::Id() {
	return this->id;
}

Star * Star::at(unsigned long id) {
	return Star::catalog[id];
}