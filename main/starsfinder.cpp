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
#include "searchstar.h"
#include "trisearchstar.h"
#include "matchstars.h"


int main(int argc, char ** argv) {
  Log::logger->setLevel(NONE);

  string config="config.cfg";
  Options options(argv[0], ".0.1", "Stars Finder");
  try {
    options.add('f', "config", "The configuration file to parse", true, false);
    options.add('c', "create", "Generate a sample config file", false, false);
    options.add('s', "source", "The source image to analyse", true, false);
    options.add('l', "level", "Luminance level to detect star", true, false);
    options.add('p', "precision", "Precision of the zone to search", true, false);
    options.add('g', "group", "Constellation or group of stars file", true, false);
    options.add('t', "test", "Run test", false, false);
    options.add('d', "debug", "Select the verbose mode : NONE, NOTICE, INFO, DEBUG", true, false);
    options.add('i', "incertitude", "Search incertitude to find matching stars", true, false);

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
          if (options.get("debug")->isAssign()) {
             Log::logger->setLevel(Log::logger->levelStr(options.get("debug")->asString()));
             std::cout << "Switch Debug to mode to "<<Log::logger->levelStr(options.get("debug")->asString())<<" "<<options.get("debug")->asString()<< endl;
          }
          if (options.get("test")->isAssign()) {
            Star * test1=new Star(617,1232,10);
            Star * test2=new Star(1020,1007,10);
            Star * test3=new Star(907,1489,10);
            SearchStar * tester=new SearchStar(test1, test2);
            tester->Target(new Star(860,149,10), new Star(1332,257,10));
            Star * find=tester->Calculate(test3);
            Log::logger->log("GLOBAL", NOTICE) << "Test Target\t"<<test3->x()<<"\t"<<test3->y()<<std::endl;
            Log::logger->log("GLOBAL", NOTICE) << "Test Find\t"<<find->x()<<"\t"<<find->y()<<std::endl;

          } 

          double level=10;
          if (options.get("level")->isAssign()) {
            level=options.get("level")->asDouble();
          } 
          unsigned int precision=1;
          if (options.get("precision")->isAssign()) {
            precision=options.get("precision")->asInt();
            if (precision <1) precision=1;
          } 
          unsigned int incertitude=30;
          if (options.get("incertitude")->isAssign()) {
            incertitude=options.get("incertitude")->asInt();
          } 
          if (options.get("group")->isAssign()) {
            try {
              pattern=Constellation::FromJson(options.get("group")->asString());
              std::cout << "Searched constellation" <<std::endl;
              std::cout << "===================================================================================================" <<std::endl;
              std::cout <<pattern->ToJson()<< endl;
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
              //sky->Sort(Star::SortType::LUMINANCE);
              std::cout << "Decoded Sky image" <<std::endl;
              std::cout << "===================================================================================================" <<std::endl;
              std::cout <<sky->ToJson()<< endl;
            } catch (ConstellationUnknownFile &e) {
              sky=NULL;
              Log::logger->log("GLOBAL", EMERGENCY) << "Unknown file " << source << endl;
            }
          }
          if (sky!=NULL && pattern!=NULL) {
            MatchStars * result=sky->Search(pattern, incertitude);
            std::cout << "Search Result" <<std::endl;
            std::cout << "===================================================================================================" <<std::endl;
            std::cout <<result->status() << endl;

            Log::logger->log("GLOBAL", NOTICE) << result->status() << endl;
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
