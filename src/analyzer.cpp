#include "analyzer.h"


Analyzer::Analyzer() {

}

double Analyzer::luminance(unsigned char R, unsigned char G, unsigned B) {
	return (0.2126*(double) R + 0.7152*(double) G + 0.0722*(double) B);
}

void Analyzer::analyze(string source)  throw(AnalyzerUnknownFile) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE * infile;

	if ((infile = fopen(source.c_str(), "rb")) == NULL) {
		Log::logger->log("ANALYZER", EMERGENCY) << "Can't read image file " << source<< endl;
		throw AnalyzerUnknownFile();
	}
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	Log::logger->log("ANALYZER", DEBUG) << "Output width \t:" << cinfo.output_width << std::endl;
	Log::logger->log("ANALYZER", DEBUG) << "Output height \t:" << cinfo.output_height << std::endl;
	Log::logger->log("ANALYZER", DEBUG) << "Number of color\t:" << cinfo.actual_number_of_colors << std::endl;
	Log::logger->log("ANALYZER", DEBUG) << "Color cmps \t:" << cinfo.out_color_components << std::endl;
	Log::logger->log("ANALYZER", DEBUG) << "Components \t:" << cinfo.output_components << std::endl;
	int row_stride;     
	unsigned char *out;

	row_stride = cinfo.output_width * cinfo.output_components;
	out = (unsigned char *)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

	unsigned long pixels=0;
	double min=3*255, max=0, sum=0;
	unsigned long stars=0;
	double star=Analyzer::luminance(24, 40, 30);
	Log::logger->log("ANALYZER", DEBUG) << "Star level \t\t: " << star <<std::endl;

	while (cinfo.output_scanline < cinfo.output_height) {
		unsigned char *rowp[1];
		rowp[0] = (unsigned char *) out + row_stride * cinfo.output_scanline;
		int nb=jpeg_read_scanlines(&cinfo, rowp, 1);
		for(int i=0; i<cinfo.output_width; i++) {
			double lum=Analyzer::luminance(rowp[0][i*3],rowp[0][i*3+1],rowp[0][i*3+2]);
			if (lum<min) min=lum;
			if (max<lum) max=lum;
			if (lum>star) stars++;
			sum+=lum;
			pixels++;
		}		
	}

	Log::logger->log("ANALYZER", DEBUG) << "Max Luminance \t\t: " << max <<std::endl;
	Log::logger->log("ANALYZER", DEBUG) << "Min Luminance \t\t: " << min <<std::endl;
	Log::logger->log("ANALYZER", DEBUG) << "Median Luminance \t: " << sum/pixels <<std::endl;
	Log::logger->log("ANALYZER", DEBUG) << "Stars pixels \t\t: " << stars <<std::endl;



	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

}