#include "constellation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <jpeglib.h>
#include "jsoncpp.h"
#include "log.h"
#include "skybuffer.h"
#include "pixel.h"
#include "log.h"


Constellation::Constellation(std::string name) {
	this->stars=new std::vector<Star *>();
	this->name=name;
}
void Constellation::Add(Star * star) {
	this->stars->push_back(star);
}
void Constellation::Sort(enum Star::SortType type) {
	std::vector<Star *> * sorted=new std::vector<Star *>();
	for (std::vector<Star *>::iterator it = this->stars->begin() ; it != this->stars->end(); ++it) {
		std::vector<Star *>::iterator itsearch = sorted->begin();
		bool found=false;
		while (!found && itsearch != sorted->end()) {
			if (((*itsearch)->x()<(*it)->x()) && (type==Star::SortType::X)) {
				found=true;
				sorted->insert(itsearch, (*it));
			}
			itsearch++;
		}
		if (!found) {
			sorted->push_back((*it));
		}
	}
	std::vector<Star *> * tmp=this->stars;
	this->stars=sorted;
	delete tmp;	
}
unsigned int Constellation::Size() {
	return this->stars->size();
}
std::vector<Star *> * Constellation::Stars() {
	return this->stars;
}

std::string Constellation::GetName(){
	return this->name;
}


std::string Constellation::ToJson() {
	std::stringstream result;
	result << "{"<<endl;
	result << "\t\"name\": \""<<this->name<<"\","<<endl;
	result << "\t\"stars\": ["<<endl;
	for (std::vector<Star *>::iterator it = this->stars->begin() ; it != this->stars->end(); ++it) {
		if (it == this->stars->begin()) {
			result << "\t\t{"<<endl;
		} else {
			result << ", {"<<endl;
		}
		result << "\t\t\t\"x\": "<<(*it)->x()<<","<<endl;
		result << "\t\t\t\"y\": "<<(*it)->y()<<","<<endl;
		result << "\t\t\t\"brightness\": "<<(*it)->luminance()<<endl;
		result << "\t\t}";
	}
	result << endl << "\t]"<<endl;
	result << "}"<<endl;
	return result.str();
}


Constellation * Constellation::FromJson(std::string filename) throw(ConstellationUnknownFile) {
	Json::Value json;   // will contains the root value after parsing.
	Json::Reader reader;
	std::ifstream jsonfile(filename, std::ifstream::binary);
	bool parsingSuccessful = reader.parse( jsonfile, json , false);
	if ( !parsingSuccessful ){
		Log::logger->log("CONSTELLATION", EMERGENCY) << "Failed to parse constellation file " << filename<< endl << reader.getFormattedErrorMessages();
		throw ConstellationUnknownFile();
	}
	std::string name=json.get("name","unknown").asString();
	Log::logger->log("CONSTELLATION", DEBUG) << "Read constellation name " << name<< endl;

	Constellation * result=new Constellation(name);
	const Json::Value stars = json["stars"];
	for ( int index = 0; index < stars.size(); ++index ) {
		const Json::Value star = stars[index];
		Log::logger->log("CONSTELLATION", DEBUG) << "\tstar " << index<< endl;
		Log::logger->log("CONSTELLATION", DEBUG) << "\t\tX\t\t: " << star["x"].asInt()<< endl;
		Log::logger->log("CONSTELLATION", DEBUG) << "\t\tY\t\t: " << star["y"].asInt()<< endl;
		Log::logger->log("CONSTELLATION", DEBUG) << "\t\tBrightness\t: " << star["brightness"].asDouble()<< endl;
		result->Add(new Star(star["x"].asInt(), star["y"].asInt(), star["brightness"].asDouble()));
	}
	return result;
}


Constellation * Constellation::FromJpeg(std::string source, double level, unsigned int precision)  throw(ConstellationUnknownFile) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE * infile;

	if ((infile = fopen(source.c_str(), "rb")) == NULL) {
		Log::logger->log("CONSTELLATION", EMERGENCY) << "Can't read image file " << source<< endl;
		throw ConstellationUnknownFile();
	}
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	Log::logger->log("CONSTELLATION", DEBUG) << "Output width \t:" << cinfo.output_width << std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Output height \t:" << cinfo.output_height << std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Number of color\t:" << cinfo.actual_number_of_colors << std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Color cmps \t:" << cinfo.out_color_components << std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Components \t:" << cinfo.output_components << std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Color Space \t:" << cinfo.out_color_space << std::endl;
	int row_stride;     
	unsigned char *out;

	row_stride = cinfo.output_width * cinfo.output_components;
	out = (unsigned char *)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

	unsigned long pixels=0;
	double min=3*255, max=0, sum=0;
	unsigned long stars=0;
	Log::logger->log("CONSTELLATION", DEBUG) << "Star level \t\t: " << level <<std::endl;
	Constellation * result=new Constellation(source);
	SkyBuffer * skybuffer=new SkyBuffer(result, precision, cinfo.output_width);
	
	while (cinfo.output_scanline < cinfo.output_height) {
		unsigned char *rowp[1];
		rowp[0] = (unsigned char *) out + row_stride * cinfo.output_scanline;
		jpeg_read_scanlines(&cinfo, rowp, 1);
		for(unsigned int i=0; i<cinfo.output_width; i++) {
			Pixel * pixel=new Pixel(rowp[0][i*3],rowp[0][i*3+1],rowp[0][i*3+2]);
			double lum=pixel->Lightness();
			if (lum<min) min=lum;
			if (max<lum) max=lum;
			if (lum>level) stars++;
			sum+=lum;
			pixels++;
			if (lum>level) {
				skybuffer->addPoint(i, cinfo.output_scanline, lum);
			}
		}
		skybuffer->turn();	
	}

	Log::logger->log("CONSTELLATION", DEBUG) << "Max Luminance \t\t: " << max <<std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Min Luminance \t\t: " << min <<std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Median Luminance \t: " << sum/pixels <<std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Stars pixels \t\t: " << stars <<std::endl;
	Log::logger->log("CONSTELLATION", DEBUG) << "Stars found \t\t: " << result->Size() <<std::endl;

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);
	return result;
}