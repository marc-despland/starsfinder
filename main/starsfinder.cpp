#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#include "options.h"
#include "parameters.h"
#include <unistd.h>
#include "constellation.h"
#include "jsoncpp.h"
#include "skybuffer.h"
#include "pixel.h"
#include "trisearchstar.h"


int main(int argc, char ** argv) {
  Log::logger->setLevel(DEBUG);

  string config="config.cfg";
  Options options(argv[0], ".0.1", "Stars Finder");
  try {
    options.add('f', "config", "The configuration file to parse", true, false);
    options.add('c', "create", "Generate a sample config file", false, false);
    options.add('s', "source", "The source image to analyse", true, false);
    options.add('l', "level", "Luminance level to detect star", true, false);
    options.add('p', "precision", "Precision of the zone to search", true, false);
    options.add('g', "group", "Constellation or group of stars file", true, false);

  } catch(ExistingOptionException &e ) {
  }
  try {
    options.parse(argc, argv);
    if (options.get("config")->isAssign()) {
      config=options.get("config")->asString();
    }
    /*Parameters * params=new Parameters(config);
    params->add("source", "The source image to analyse", false);*/

    /*if (options.get("create")->isAssign()) {
      try {
        params->create();
      } catch(CantCreateFileException &e) {
        Log::logger->log("GLOBAL", EMERGENCY) << "Can't create file " << config<< endl;
      }
      exit(0);
    }*/
      Constellation * sky=NULL;
      Constellation * pattern=NULL;
     try {
      try {
        //params->parse();
        try {
          double level=10;
          if (options.get("level")->isAssign()) {
            level=options.get("level")->asDouble();
          } 
          unsigned int precision=1;
          if (options.get("precision")->isAssign()) {
            precision=options.get("precision")->asInt();
            if (precision <1) precision=1;
          } 
          if (options.get("group")->isAssign()) {
            try {
              pattern=Constellation::FromJson(options.get("group")->asString());
              Log::logger->log("GLOBAL", DEBUG) <<pattern->ToJson();
            }catch (ConstellationUnknownFile &e) {
              pattern=NULL;
              Log::logger->log("GLOBAL", EMERGENCY) << "Unknown Constellation file " << options.get("group")->asString() << endl;
            }
          }
          string source="";
          if (options.get("source")->isAssign()) {
            source=options.get("source")->asString();
            try {
              sky=Constellation::FromJpeg(source,level, precision);
              Log::logger->log("GLOBAL", DEBUG) <<sky->ToJson();
            } catch (ConstellationUnknownFile &e) {
              sky=NULL;
              Log::logger->log("GLOBAL", EMERGENCY) << "Unknown file " << source << endl;
            }
          }
          if (sky!=NULL && pattern!=NULL) {
            sky->Search(pattern);
          }
        } catch (UnknownParameterNameException &e) {
        	Log::logger->log("GLOBAL", EMERGENCY) << "Not defined parameter " << endl;
        }
      }catch(FileNotFoundException &e) {
        Log::logger->log("GLOBAL", EMERGENCY) << "Can't open file " << config<< endl;
      }catch(InvalidConfigFileException &e) {
        Log::logger->log("GLOBAL", EMERGENCY) << "Invalid configuration file " << config<< endl;
      }
    }catch(FileNotFoundException &e) {
      Log::logger->log("GLOBAL", EMERGENCY) << "Can't open file  " << config<< endl;
    }
  } catch (OptionsStopException &e) {
  } catch (UnknownOptionException &e) {
    Log::logger->log("MAIN",NOTICE) << "Unknown option requested" << endl;
  }
  return 0;
}
