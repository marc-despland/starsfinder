#include "searchstar.h"
#include <cmath> 
#include "log.h"

SearchStar::SearchStar(Star * A, Star * B) {
	this->A=A;
	this->B=B;
	this->lengthAB=A->distance(B);
	this->lengthAB2=this->lengthAB*this->lengthAB;
}
void SearchStar::Target(Star * targetA, Star * targetB) {
	this->targetA=targetA;
	this->targetB=targetB;
	this->targetdx=targetB->x()-targetA->x();
	this->targetdy=targetB->y()-targetA->y();
	this->lengthTAB=targetA->distance(targetB);

	
}

double SearchStar::yc(Star * A, Star * B, Star *C) {
	double dividende=((A->y()-B->y())*C->x())+(B->y()*A->x())-(A->y()*B->x());
	double diviseur=(A->x()-B->x());
	return (dividende/diviseur);
}

double SearchStar::Angle(Star * A, Star * B, Star *C) {
	double angle = AlKashi(A->distance(B), C->distance(A), B->distance(C));
	if (C->x()>A->x() && B->x()>A->x()) {
		if (C->y()>yc(A, B, C)) angle*=-1;
	}
	if (C->x()<A->x() && B->x()<A->x()) {
		if (C->y()<yc(A, B, C)) angle*=-1;
	}
	if (C->x()<A->x() && B->x()>A->x()) {
		if (C->y()>yc(A, B, C)) angle=(2*M_PI-angle);
	}
	if (C->x()>A->x() && B->x()<A->x()) {
		if (C->y()<yc(A, B, C)) angle=(2*M_PI-angle);
	}
	return angle;
}



Star * SearchStar::Calculate(Star * C) {
	double lengthCA=C->distance(this->A);
	double angle=Angle(this->A, this->B, C);
	double cosa=cos(angle);
	double sina=sin(angle);
	double ratio=(this->lengthTAB*lengthCA)/(this->lengthAB2);
	double x=this->targetA->x()+((this->targetdx*cosa+this->targetdy*sina) *ratio);
	double y=this->targetA->y()+((this->targetdy*cosa-this->targetdx*sina) *ratio);
	return new Star(floor(x), floor(y), 0);
}


double SearchStar::AlKashi(double a, double b, double c) {
	return acos((a*a+b*b-c*c)/(2*a*b));
}
