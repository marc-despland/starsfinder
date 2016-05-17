#include "pixel.h"
#include "log.h"

Pixel::Pixel(unsigned int red, unsigned int green, unsigned int blue) {
	this->r=red;
	this->g=green;
	this->b=blue;
	this->h=0;
	this->s=0;
	this->l=0;
	this->RGBtoHSL();

}
unsigned int Pixel::R() {
	return this->r;
}
unsigned int Pixel::G() {
	return this->g;
}

unsigned int Pixel::B() {
	return this->b;
}

double Pixel::Lightness() {
	return this->l*100;		
}
double Pixel::Saturation() {
	return this->s*100;		
}
double Pixel::Hue() {
	return this->h*360;		
}

void Pixel::Brighter(double x) {
	this->l=this->l*x;
	if (this->l>1) this->l=1;
	this->HSLtoRGB();
}

void Pixel::RGBtoHSL() {
	double rp=(double) this->r/255; //0.607843137
	double gp=(double) this->g/255; //0.780392157
	double bp=(double) this->b/255; //0.690196078
	double cmax=rp; //0.780392157
	if (gp>cmax) cmax=gp;
	if (bp>cmax) cmax=bp;
	double cmin=rp; // 0.607843137
	if (gp<cmin) cmin=gp;
	if (bp<cmin) cmin=bp;
	double delta=cmax-cmin; //0.17254902	
	this->l=(cmax+cmin)/2; //0.694117647

	if (delta==0) {
		this->h=0;
		this->s=0;
	} else {
		if (this->l<0.5) {
			this->s=delta/(cmax+cmin);
		} else {
			this->s=delta/(2-cmax-cmin); // 0.282051283
		}
		double deltar=(((cmax-rp)/6)+(delta/2))/delta; //0.666666667
		double deltag=(((cmax-gp)/6)+(delta/2))/delta; //0.5
		double deltab=(((cmax-bp)/6)+(delta/2))/delta; // 0.587121212
		if (rp==cmax) {
			this->h=deltab-deltag;
		} else {
			if (gp == cmax) {
				this->h=(0.333333333)+deltar-deltab; //0.412878788
			} else {
				if (bp==cmax) {
					this->h=(0.666666667)+deltag-deltar;
				}
			}
		}
		if (this->h<0) {
			this->h+=1;
		}
		if (this->h>1) {
			this->h-=1;
		}
		
	}
}


void Pixel::HSLtoRGB() {
	if (this->s==0) {
		this->r=this->l*255;
		this->g=this->l*255;
		this->b=this->l*255;
	} else {
		double h2 = this->h + 0.5;
		if (h2 > 1) {
			h2 -= 1;
		}
		double var_2=0;
		if (this->l < 0.5){
			var_2 = this->l * (1 + this->s);
        } else {
        	var_2 = (this->l + this->s) - (this->s * this->l);
        }

        double var_1 = 2 * this->l - var_2;
        this->r = 255 * HUEtoRGB(var_1,var_2,h2 + (1 / 3));
        this->g = 255 * HUEtoRGB(var_1,var_2,h2);
        this->b = 255 * HUEtoRGB(var_1,var_2,h2 - (1 / 3));		
	}
}


	double Pixel::HUEtoRGB(double v1, double v2, double vh) {
		if (vh<0) {
			vh +=1;
		}
		if (vh>1) {
			vh-=1;
		}
		if ((6 * vh) < 1){
			return (v1 + (v2 - v1) * 6 * vh);
	    }

	    if ((2 * vh) < 1) {
			return (v2);
		}

		if ((3 * vh) < 2){
			return (v1 + (v2 - v1) * ((2 / 3 - vh) * 6));
		}

		return (v1);
	}
