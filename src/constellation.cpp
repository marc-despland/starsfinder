#include "constellation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <jpeglib.h>
#include "jsoncpp.h"
#include "log.h"
#include "skybuffer.h"
#include "pixel.h"
#include "searchstar.h"
#include "matchstars.h"

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
			if (((*itsearch)->luminance()<(*it)->luminance()) && (type==Star::SortType::LUMINANCE)) {
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
		result << "\t\t\t\"id\": "<<(*it)->Id()<<","<<endl;
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


/*bool Constellation::Search(Constellation * pattern) {
	pattern->Sort(Star::SortType::LUMINANCE);
	Star * triple[3];
	triple[0]=pattern->Stars()->at(0);
	triple[1]=pattern->Stars()->at(1);
	triple[2]=pattern->Stars()->at(2);
	for (int i=0; i<3;i++) {
		Log::logger->log("CONSTELLATION", DEBUG) << "STAR "<< i << "\t " << triple[i]->x() << "\t" << triple[i]->y() << endl;
	}

	SearchStar * engine=new SearchStar();
	engine->Reference(pattern->Stars()->at(0), pattern->Stars()->at(1));
	for (int i=0;i<this->Size(); i++) {
		for (int j=0;j<this->Size(); j++) {
			if (i!=j) {
				Star * target=engine->Calculate(pattern->Stars()->at(2),this->stars->at(i),this->stars->at(j));
				target->precision(2);
				for (int k=0;k<this->Size(); k++) {
					if ((i!=k) && (j!=k)) {
						if (target->include(this->stars->at(k))) {
							Log::logger->log("CONSTELLATION", DEBUG) << endl<< "Found Match \t\t: " << this->stars->at(i)->Id()<<", "<<this->stars->at(j)->Id()<<", "<<this->stars->at(k)->Id() <<std::endl;
							Log::logger->log("CONSTELLATION", DEBUG) << "STAR "<< this->stars->at(i)->Id() << "\t " << Star::at(this->stars->at(i)->Id())->x() << "\t" << Star::at(this->stars->at(i)->Id())->y() << endl;
							Log::logger->log("CONSTELLATION", DEBUG) << "STAR "<< this->stars->at(j)->Id() << "\t " << Star::at(this->stars->at(j)->Id())->x() << "\t" << Star::at(this->stars->at(j)->Id())->y() << endl;
							Log::logger->log("CONSTELLATION", DEBUG) << "STAR "<< this->stars->at(k)->Id() << "\t " << Star::at(this->stars->at(k)->Id())->x() << "\t" << Star::at(this->stars->at(k)->Id())->y() << endl;
						}
					}
				}
			}
		}
	}
	return false;

}*/


MatchStars * Constellation::Search(Constellation * pattern, unsigned int tolerance) {
	

	MatchStars * matches=new MatchStars(pattern);
	SearchStar * engine=new SearchStar(pattern->Stars()->at(0), pattern->Stars()->at(1));
	//engine->Reference(pattern->Stars()->at(0), pattern->Stars()->at(1));
	unsigned int i=0;
	bool matched=false;
	unsigned long second=0;
	unsigned long next=0;


	while (i<this->Size() && !matched) {
		second=matches->match(this->stars->at(i)->Id());
		Log::logger->log("CONSTELLATION", DEBUG) << "Loop : i=" << i<< "\t matched="<< matched<<"\t second="<<second<<std::endl;
		unsigned int j=0;
		while (j<this->Size() && !matched) {
			if (i!=j) {
				//second=this->stars->at(j)->Id();
				next=matches->match(this->stars->at(j)->Id());
				engine->Target(this->stars->at(i),this->stars->at(j));
				matched=true;
				Log::logger->log("CONSTELLATION", DEBUG) << "\tLoop : i=" << i<<"\t j="<<j<<"\t second="<<second<< "\t next=" << next<<"\t matched="<< matched<<std::endl;
				Log::logger->log("CONSTELLATION", DEBUG) << "Starting" << std::endl << "==============================================="<<std::endl<<matches->status();
				while (next>0 && matched && next!=second) {
					Log::logger->log("CONSTELLATION", DEBUG) << "\t\tLoop : next=" << next<< "\t matched="<< matched<<"\t second="<<second<<std::endl;
					Log::logger->log("CONSTELLATION", DEBUG) << "\t\t\tSearched star : "<<next<<"("<<Star::at(next)->x()<<","<<Star::at(next)->y()<<")"<<std::endl;

					Star * target=engine->Calculate(Star::at(next));
					target->precision(tolerance);
					Log::logger->log("CONSTELLATION", DEBUG) << "\t\tTarget\t:\t("<<target->x()<<","<<target->y()<<")"<<std::endl;
					matched=false;
					std::vector<Star *>::iterator it=this->stars->begin();
					while (it!=this->stars->end() && next!=second && next!=0) {
						//Log::logger->log("CONSTELLATION", DEBUG) << "\t\tTarget\t:\t("<<target->x()<<","<<target->y()<<")\t ID:"<<(*it)->Id()<<" ("<<(*it)->x()<<","<<(*it)->y()<<")"<<std::endl;
						if (!matches->known((*it)->Id())) {
							//Log::logger->log("CONSTELLATION", DEBUG) << "\t\tTest the Target"<<std::endl;
							matched=target->include((*it));
						}
						if (matched) {
							next=matches->match((*it)->Id());
							Log::logger->log("CONSTELLATION", DEBUG) << "We have a match next :"<<next << std::endl << "==============================================="<<std::endl<<matches->status();
							it=this->stars->begin();
							if (next>0) {
								target=engine->Calculate(Star::at(next));
								target->precision(tolerance);
								matched=false;
							}
						} else {
							it++;
							if (it==this->stars->end()) {
								Star * current=Star::at(matches->lastTarget());
								it=this->stars->begin();
								while(it!=this->stars->end() && (*it)!=current) it++;
								if (it!=this->stars->end()) {
									it++;
								}
								next=matches->unmatch();
								Log::logger->log("CONSTELLATION", DEBUG) << "Unmatch next :"<<next << std::endl << "==============================================="<<std::endl<<matches->status();

							}
						}
					}
					Log::logger->log("CONSTELLATION", DEBUG) << "Search status" << std::endl << "==============================================="<<std::endl<<matches->status();
					Log::logger->log("CONSTELLATION", DEBUG) << "\t\tEnd  : next=" << next<< "\t matched="<< matched<<"\t second="<<second<<std::endl;
				}
				/*if (!matched) {
					matches->unmatch();
				}*/

			} 
			if (!matched) {
				j++;
			}
			Log::logger->log("CONSTELLATION", DEBUG) << "\tEnd  : i=" << i<<"\t j="<<j<<"\t second="<<second<< "\t next=" << next<<"\t matched="<< matched<<std::endl;				
		}
		if (!matched) {
			matches->unmatch();
			i++;
		}
		Log::logger->log("CONSTELLATION", DEBUG) << "End  : i=" << i<< "\t matched="<< matched<<"\t second="<<second<<std::endl;
	}
	Log::logger->log("CONSTELLATION", DEBUG) << "Finish search with result : " << matched<<std::endl;
	return matches;

}