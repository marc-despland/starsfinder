#include "trisearchstar.h"
#include <cmath> 
#include "log.h"


TriSearchStar::TriSearchStar(Star * stars[3]) {
	for(int i=0; i<3; i++) {
		this->stars[i]=stars[i];
	}
	this->length[0]=this->stars[0]->distance(this->stars[1]);
	this->length[1]=this->stars[1]->distance(this->stars[2]);
	this->length[2]=this->stars[2]->distance(this->stars[0]);

	this->angle[0]=AlKashi(this->length[0], this->length[2], this->length[1]);
	this->angle[1]=AlKashi(this->length[1], this->length[0], this->length[2]);
	this->angle[2]=AlKashi(this->length[2], this->length[1], this->length[0]);

}

Star * TriSearchStar::CalculateThird(Star * first, Star * second) {
	double length=first->distance(second);
	double ratio=length/this->length[0];
	double rx=first->x()+((second->x()-first->x())*cos(this->angle[0])-(second->y()-first->y())*sin(this->angle[0]));
	double ry=first->y()+((second->x()-first->x())*sin(this->angle[0])+(second->y()-first->y())*cos(this->angle[0]));
	double x=first->x()+(((rx-first->x())*ratio*this->length[2]))/this->length[0];
	double y=first->y()+(((ry-first->y())*ratio*this->length[2]))/this->length[0];
	return new Star(x, y, 0);
}


double TriSearchStar::AlKashi(double a, double b, double c) {
	return acos((a*a+b*b-c*c)/(2*a*b));
}