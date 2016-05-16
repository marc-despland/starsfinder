#ifndef _ANALYZER_H
#define _ANALYZER_H

#include "log.h"
#include <jpeglib.h>
#include "star.h"
#include "skybuffer.h"

class AnalyzerUnknownFile : public exception {
	const char* what();
};


class Analyzer {
public:
	Analyzer();
	void analyze(string source, double level, unsigned int precision)  throw(AnalyzerUnknownFile);
	static double luminance(unsigned char R, unsigned char G, unsigned B);

};
#endif