#include "skybuffer.h"
#include <stdlib.h>
#include "log.h"

SkyBuffer::SkyBuffer(Constellation * constellation,unsigned int lines, unsigned int width) {
	this->lines=lines;
	this->width=width;
	this->buffer=new Star**[lines+1];
	this->constellation=constellation;
	for (unsigned int i=0; i<lines+1; i++) {
		this->buffer[i]=new Star*[width+(2*lines)];
		for (unsigned int j=0; j<width+(2*lines); j++) {
			this->buffer[i][j]=NULL;
		}
	}
}

void SkyBuffer::turn() {
	for (unsigned int j=0; j<width+(2*this->lines); j++) {
		for (unsigned int i=0; i<this->lines; i++) {
			this->buffer[i][j]=this->buffer[i+1][j];
		}
		this->buffer[this->lines][j]=NULL;
	}
}

void SkyBuffer::addPoint(unsigned int x, unsigned int y, double luminance) {
	bool found=false;
	Star * star=NULL;
	unsigned int level=1;
	//Log::logger->log("ANALYZER", DEBUG) << "Level " << level << " Search star near " << x << "," << y <<std::endl;
	while(!found && level<=this->lines) {
		unsigned int i=0;
		while (!found && i<level) {
			//Log::logger->log("ANALYZER", DEBUG) << "Level " << level << " Round 1 look at " << lines-i << "," << x-level <<std::endl;
			if (buffer[lines-i][lines+x-level]!=NULL) {
				found=true;
				star=buffer[lines-i][lines+x-level];
			}
			i++;
		}
		i=x-level;
		while (!found && i<x+level) {
			//Log::logger->log("ANALYZER", DEBUG) << "Level " << level << " Round 2 look at " << lines-level << "," << i <<std::endl;
			if (buffer[lines-level][lines+i]!=NULL) {
				found=true;
				star=buffer[lines-level][lines+i];
			}
			i++;
		}
		i=lines-level;
		while (!found && i<lines) {
			//Log::logger->log("ANALYZER", DEBUG) << "Level " << level << " Round 3 look at " << i << "," << x+level <<std::endl;
			if (buffer[i][lines+x+level]!=NULL) {
				found=true;
				star=buffer[i][lines+x+level];
			}
			i++;
		}
		level++;		
	}
	if (found) {
		star->add(x, y, luminance);
	} else {
		star=new Star(x, y, luminance);
		this->constellation->Add(star);
	}
	buffer[lines][lines+x]=star;
}
