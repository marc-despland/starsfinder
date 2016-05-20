#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cmath> 
#include "star.h"
#include "log.h"
#include <math.h>



double AlKashi(double a, double b, double c) {
	return acos((a*a+b*b-c*c)/(2*a*b));
}

double yc(Star * A, Star * B, Star *C) {
	double dividende=((A->y()-B->y())*C->x())+(B->y()*A->x())-(A->y()*B->x());
	double diviseur=(A->x()-B->x());
	return (dividende/diviseur);
}

double Angle(Star * A, Star * B, Star *C) {
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



Star * Calculate(Star * A, Star * B, Star * C) {
	double length=A->distance(B);
	double length2=C->distance(A);
	double angle=Angle(A, B, C);
	double cosa=cos(angle);
	double sina=sin(angle);
	double dx=B->x()-A->x();
	double dy=B->y()-A->y();
	double ratio=(A->distance(B)*length2)/(length*length);
	double x=A->x()+((dx*cosa+dy*sina) *ratio);
	double y=A->y()+((dy*cosa-dx*sina) *ratio);
	return new Star(floor(x), floor(y), 0);
}



int main(int argc, char ** argv) {
	Log::logger->setLevel(NOTICE);
	Star * A;
	Star * B;
	Star * C;
	Star * D;
	double angle=0;


	A=new Star(20,20,10);

	B=new Star(30,30,10);
	C=new Star(25,10,10);
	angle = Angle(A, B, C);
	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(25,10,10);
	C=new Star(30,30,10);
	angle = Angle(A, B, C);
	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(15,30,10);
	C=new Star(10,10,10);
	angle = Angle(A, B, C);
	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(10,10,10);
	C=new Star(15,30,10);
	angle = Angle(A, B, C);
	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;


	B=new Star(30,30,10);
	C=new Star(10,10,10);
	angle = Angle(A, B, C);
	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(30,30,10);
	C=new Star(10,15,10);
	angle = Angle(A, B, C);
	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(30,30,10);
	C=new Star(10,30,10);
	angle = Angle(A, B, C);
	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;


	B=new Star(10,10,10);
	C=new Star(30,30,10);
 	angle = Angle(A, B, C);
 	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(20,30,10);
	C=new Star(30,25,10);
 	angle = Angle(A, B, C);
 	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(10,30,10);
	C=new Star(30,5,10);
 	angle = Angle(A, B, C);
 	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\t\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;

	B=new Star(10,30,10);
	C=new Star(30,15,10);
 	angle = Angle(A, B, C);
 	D=Calculate(A,B,C);
	std::cout << "A("<<A->x()<<","<<A->y()<<")\tB("<<B->x()<<","<<B->y()<<")\tC("<<C->x()<<","<<C->y()<<")\tangle="<<(angle*180/3.14116)<<"\tD("<<D->x()<<","<<D->y()<<")"<<std::endl;
	return 0;
}